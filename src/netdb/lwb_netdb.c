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

static const char *delims = " \t\n";

static int __str_to_addr(const char *str, void *addr, int *family, int *addr_length)
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

#define ENTNAME hostent
#define ENTFILE "/etc/hosts"
#define ENTPREFIX h_
#define ENT_ADDRLIST 1
#include "getent.c"
#undef ENT_ADDRLIST
#undef ENTPREFIX
#undef ENTFILE
#undef ENTNAME

#define ENTNAME netent
#define ENTFILE "/etc/networks"
#define ENTPREFIX n_
#define ENT_ADDRLIST 0
#include "getent.c"
#undef ENT_ADDRLIST
#undef ENTPREFIX
#undef ENTFILE
#undef ENTNAME
