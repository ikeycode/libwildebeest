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
#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>

static const char *delims = " \t\n";

int __str_to_addr(const char *str, void *addr, int *family, int *addr_length)
{
        if (inet_aton(str, (struct in_addr *)addr) == 1) {
                if (family != NULL)
                        *family = AF_INET;
                if (addr_length != NULL)
                        *addr_length = sizeof(struct in_addr);
                return 1;
        }
        if (family != NULL && inet_pton(AF_INET6, str, (struct in6_addr *)addr) == 1) {
                *family = AF_INET6;
                *addr_length = sizeof(struct in6_addr);
                return 1;
        }
        return 0;
}

int __parse_address_list(char *line, char **addr_list, int *family, int *add_length, char **saveptr,
                         char **free_pos)
{
        char *item = strtok_r(line, delims, saveptr);
        if (item == NULL)
                return 0;
        *free_pos += ENT_ADDRESS_LIST_SIZE;
        addr_list[0] = *free_pos;
        addr_list[1] = NULL;
        *free_pos += ENT_ADDRESS_ENTRY_SIZE;
        if (__str_to_addr(item, addr_list[0], family, add_length) != 1)
                return 0;
        return 1;
}

int __parse_address(char *line, uint32_t *addr, char **saveptr)
{
        char *item = strtok_r(line, delims, saveptr);
        if (item == NULL)
                return 0;
        if (__str_to_addr(item, addr, NULL, NULL) != 1)
                return 0;
        return 1;
}

int __parse_name(char *line, char **dst, char **saveptr)
{
        char *item = strtok_r(line, delims, saveptr);

        if (item == NULL) {
                *dst = "";
                return 0;
        }
        *dst = item;
        return 1;
}

void __parse_aliases(char **aliases, char **saveptr, size_t aliases_max)
{
        char *item = NULL;
        size_t aliases_cnt = 0;

        item = strtok_r(NULL, delims, saveptr);
        for (aliases_cnt = 0; item != NULL && aliases_cnt < aliases_max; aliases_cnt++) {
                aliases[aliases_cnt] = item;
                item = strtok_r(NULL, delims, saveptr);
        }
        aliases[aliases_cnt] = NULL;
}

void __parse_comment(char *line)
{
        char *c = NULL;

        if ((c = strchr(line, '#'))) {
                *c++ = '\n';
                *c = 0;
        }
}
