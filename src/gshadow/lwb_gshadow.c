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

#ifndef _XOPEN_SOURCE
#define _XOPEN_SOURCE
#endif

#include "gshadow.h"

void __wrap_setsgent(void)
{
#warning "setsgent not yet implemented"
}

void __wrap_endsgent(void)
{
#warning "endsgent not yet implemented"
}

struct sgrp *__wrap_getsgent(void)
{
#warning "getsgent not yet implemented"
        return NULL;
}

struct sgrp *__wrap_getsgnam(const char *__name)
{
#warning "getsgnam not yet implemented"
        return NULL;
}

struct sgrp *__wrap_sgetsgent(const char *__string)
{
#warning "sgetsgent not yet implemented"
        return NULL;
}

struct sgrp *__wrap_fgetsgent(FILE *__stream)
{
#warning "fgetsgent not yet implemented"
        return NULL;
}

int __wrap_putsgent(const struct sgrp *__g, FILE *__stream)
{
#warning "putsgent not yet implemented"
        return -1;
}

int __wrap_getsgent_r(struct sgrp *result_buf, char *buffer, size_t buflen, struct sgrp **result)
{
#warning "getsgent_r not yet implemented"
        return -1;
}

int __wrap_getsgnam_r(const char *name, struct sgrp *result_buf, char *buffer, size_t buflen,
                      struct sgrp **result)
{
#warning "getsgnam_r not yet implemented"
        return -1;
}

int __wrap_sgetsgent_r(const char *string, struct sgrp *result_buf, char *buffer, size_t buflen,
                       struct sgrp **result)
{
#warning "sgetsgent_r not yet implemented"
        return -1;
}

int __wrap_fgetsgent_r(FILE *stream, struct sgrp *result_buf, char *buffer, size_t buflen,
                       struct sgrp **result)
{
#warning "fgetsgent_r not yet implemented"
        return -1;
}
