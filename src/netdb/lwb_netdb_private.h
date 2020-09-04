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

#ifndef __LWB_NETDB_PRIVATE_H
#define __LWB_NETDB_PRIVATE_H

#define MAX_LINE_SIZE 1024
#define MAX_ALIASES 32
#define ENT_ADDRESS_LIST_SIZE (2 * sizeof(char *))
#define ENT_ADDRESS_ENTRY_SIZE sizeof(struct in6_addr)
#define ENT_ADDRESS_SIZE (ENT_ADDRESS_LIST_SIZE + ENT_ADDRESS_ENTRY_SIZE)
#define MAX_BUFFER_SIZE (MAX_LINE_SIZE + MAX_ALIASES * sizeof(char *) + ENT_ADDRESS_SIZE)
#define ALIASES_MAX(S, C, E) ((S - C - E - 1) / sizeof(char *))

int __str_to_addr(const char *str, void *addr, int *family, int *addr_length);
int __parse_address_list(char *line, char **addr_list, int *family, int *add_length, char **saveptr,
                         char **free_pos);
int __parse_address(char *line, uint32_t *addr, char **saveptr);
int __parse_name(char *line, char **dst, char **saveptr);
void __parse_aliases(char **aliases, char **saveptr, size_t aliases_max);
void __parse_comment(char *line);

#endif /* __LWB_NETDB_PRIVATE_H */
