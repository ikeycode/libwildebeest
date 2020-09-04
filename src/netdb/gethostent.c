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

static struct hostent *__gethostent(FILE *f, struct hostent *ent, char **line, size_t *size,
                                    struct hostent **res, int *errnoptr)
{
        for (;;) {
                ssize_t l = 0;
                char *saveptr = NULL;
                char *free_pos = NULL;
                size_t aliases_max = 0;

                if ((l = getline(line, size, f)) < 0) {
                        *errnoptr = NO_RECOVERY;
                        ent = NULL;
                        break;
                }
                if (l == 0)
                        continue;
                if (*size <= (size_t)l + 1 + ENT_ADDRESS_SIZE) {
                        *errnoptr = NO_RECOVERY;
                        ent = NULL;
                        break;
                }
                free_pos = *line + l + 1;
                aliases_max = ALIASES_MAX(*size, (size_t)l, ENT_ADDRESS_SIZE);
                ent->h_name = 0;
                ent->h_aliases = 0;
                __parse_comment(*line);
                ent->h_addr_list = (char **)free_pos;
                if (__parse_address_list(*line,
                                         ent->h_addr_list,
                                         &ent->h_addrtype,
                                         &ent->h_length,
                                         &saveptr,
                                         &free_pos) != 1)
                        continue;
                if (__parse_name(NULL, &ent->h_name, &saveptr) != 1)
                        break;
                ent->h_aliases = (char **)free_pos;
                __parse_aliases(ent->h_aliases, &saveptr, aliases_max);
                break;
        }
        if (res)
                *res = ent;
        return ent;
}

void __wrap_sethostent(int stayopen __attribute__((unused)))
{
        if (__file != NULL)
                fclose(__file);
        __file = NULL;
}

void __wrap_endhostent(void)
{
        __wrap_sethostent(0);
}

static inline FILE *__open_hosts_file(void)
{
        if (__file == NULL)
                __file = fopen("/etc/hosts", "r");
        return __file;
}

int __wrap_gethostent_r(struct hostent *ret, char *buf, size_t buflen, struct hostent **res,
                        int *h_errnop)
{
        if (__open_hosts_file() == NULL)
                return 1;
        if (__gethostent(__file, ret, &buf, &buflen, res, h_errnop) == NULL)
                return 1;
        return 0;
}

struct hostent *__wrap_gethostent(void)
{
        static char *__buf = NULL;
        static size_t __size = 0;
        static struct hostent __ent;
        struct hostent *res = &__ent;
        if (__buf == NULL) {
                __buf = calloc(1, MAX_BUFFER_SIZE);
                __size = MAX_BUFFER_SIZE;
        }
        if (__wrap_gethostent_r(&__ent, __buf, __size, &res, &errno) == 1)
                return NULL;
        return res;
}
