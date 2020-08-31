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
#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#define MAX_LINE_SIZE 1024
#define MAX_ALIASES 32
#define ENT_ADDRESS_LIST_SIZE (2 * sizeof(char *))
#define ENT_ADDRESS_ENTRY_SIZE sizeof(struct in6_addr)
#define ENT_ADDRESS_SIZE (ENT_ADDRESS_LIST_SIZE + ENT_ADDRESS_ENTRY_SIZE)
#define MAX_BUFFER_SIZE (MAX_LINE_SIZE + MAX_ALIASES * sizeof(char *) + ENT_ADDRESS_SIZE)
static FILE *__file = NULL;
static const char *delims = " \t\n";

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

int __str_to_addr(const char *str, void *addr, int *family, int *addr_length)
{
        if (inet_aton(str, (struct in_addr *)addr) == 1) {
                *family = AF_INET;
                *addr_length = sizeof(struct in_addr);
                return 1;
        }
        if (inet_pton(AF_INET6, str, (struct in6_addr *)addr) == 1) {
                *family = AF_INET6;
                *addr_length = sizeof(struct in6_addr);
                return 1;
        }
        return 0;
}

static struct hostent *__gethostent(FILE *f, struct hostent *ent, char **line, size_t *size,
                                    struct hostent **res, int *errnoptr)
{
        for (;;) {
                ssize_t l = 0;
                char *item = NULL;
                char *saveptr = NULL;
                char *free_pos = NULL;
                size_t aliases_max = 0;
                size_t aliases_cnt = 0;

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
                aliases_max = (*size - (size_t)l - 1 - ENT_ADDRESS_SIZE) / sizeof(char *);
                ent->h_name = 0;
                ent->h_aliases = 0;
                if ((item = strchr(*line, '#'))) {
                        *item++ = '\n';
                        *item = 0;
                }
                item = strtok_r(*line, delims, &saveptr);
                if (item == NULL)
                        continue;
                ent->h_addr_list = (char **)free_pos;
                free_pos += ENT_ADDRESS_LIST_SIZE;
                ent->h_addr_list[0] = free_pos;
                ent->h_addr_list[1] = NULL;
                free_pos += ENT_ADDRESS_ENTRY_SIZE;
                if (__str_to_addr(item, ent->h_addr_list[0], &ent->h_addrtype, &ent->h_length) != 1)
                        continue;
                item = strtok_r(NULL, delims, &saveptr);
                if (item == NULL) {
                        ent->h_name = "";
                        break;
                }
                ent->h_name = item;
                ent->h_aliases = (char **)free_pos;
                item = strtok_r(NULL, delims, &saveptr);
                for (aliases_cnt = 0; item != NULL && aliases_cnt < aliases_max; aliases_cnt++) {
                        ent->h_aliases[aliases_cnt] = item;
                        item = strtok_r(NULL, delims, &saveptr);
                }
                ent->h_aliases[aliases_cnt] = NULL;
                break;
        }
        if (res)
                *res = ent;
        return ent;
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
