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

/* We might be used with old compiler settings. */
#ifndef _LWB_PRINTF_H
#define _LWB_PRINTF_H

#include "lwb_common.h"

#include <stddef.h>

enum { PA_INT, PA_CHAR, PA_WCHAR, PA_STRING, PA_WSTRING, PA_POINTER, PA_FLOAT, PA_DOUBLE, PA_LAST };

#define PA_FLAG_MASK 0xff00
#define PA_FLAG_LONG_LONG (1 << 8)
#define PA_FLAG_LONG_DOUBLE PA_FLAG_LONG_LONG
#define PA_FLAG_LONG (1 << 9)
#define PA_FLAG_SHORT (1 << 10)
#define PA_FLAG_PTR (1 << 11)

/**
 * Provide the standard parse_printf_format signature
 */

size_t parse_printf_format(const char *__restrict __fmt, size_t __n, int *__restrict __argtypes);

#endif /* _LWB_PRINTF_H */
