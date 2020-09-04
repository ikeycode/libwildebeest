/*
 * This file is part of libwildebeest
 *
 * Copyright © 2020 Serpent OS Developers
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#define _GNU_SOURCE

#include "lwb_netdb.h"
#include "lwb_netdb_private.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

static FILE *__file = NULL;

static struct netent *__getnetent(FILE *f, struct netent *ent, char **line, size_t *size,
                                  struct netent **res, int *errnoptr)
{
        for (;;) {
                ssize_t l = 0;
                char *saveptr = NULL;
                char *free_pos = NULL;

                if ((l = getline(line, size, f)) < 0) {
                        *errnoptr = NO_RECOVERY;
                        ent = NULL;
                        break;
                }
                if (l == 0)
                        continue;
                if (*size <= (size_t)l + 1) {
                        *errnoptr = NO_RECOVERY;
                        ent = NULL;
                        break;
                }
                free_pos = *line + l + 1;
                ent->n_name = 0;
                ent->n_aliases = 0;
                __parse_comment(*line);
                if (__parse_name(*line, &ent->n_name, &saveptr) != 1)
                        continue;
                if (__parse_address(NULL, &ent->n_net, &saveptr) != 1)
                        continue;
                ent->n_net = htonl(ent->n_net);
                ent->n_aliases = (char **)free_pos;
                __parse_aliases(ent->n_aliases, &saveptr, ALIASES_MAX(*size, (size_t)l, 0));
                break;
        }
        if (res)
                *res = ent;
        return ent;
}

void __wrap_setnetent(int stayopen __attribute__((unused)))
{
        if (__file != NULL)
                fclose(__file);
        __file = NULL;
}

void __wrap_endnetent(void)
{
        __wrap_setnetent(0);
}

static inline FILE *__open_hosts_file(void)
{
        if (__file == NULL)
                __file = fopen("/etc/networks", "r");
        return __file;
}

int __wrap_getnetent_r(struct netent *ret, char *buf, size_t buflen, struct netent **res,
                       int *h_errnop)
{
        if (__open_hosts_file() == NULL)
                return 1;
        if (__getnetent(__file, ret, &buf, &buflen, res, h_errnop) == NULL)
                return 1;
        return 0;
}

struct netent *__wrap_getnetent(void)
{
        static char *__buf = NULL;
        static size_t __size = 0;
        static struct netent __ent;
        struct netent *res = &__ent;
        if (__buf == NULL) {
                __buf = calloc(1, MAX_BUFFER_SIZE);
                __size = MAX_BUFFER_SIZE;
        }
        if (__wrap_getnetent_r(&__ent, __buf, __size, &res, &errno) == 1)
                return NULL;
        return res;
}
