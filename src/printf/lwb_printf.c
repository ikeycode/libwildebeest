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

#include "printf.h"

static inline void __assign_argtypes(int *__restrict __argtypes, size_t pos, size_t max, int value)
{
        if (pos < max)
                __argtypes[pos] = value;
}

size_t __wrap_parse_printf_format(const char *__restrict __fmt, size_t __n,
                                  int *__restrict __argtypes)
{
        const char *p = NULL;
        int type = 0;
        int in_format = 0;
        size_t res = 0;

        if (!__fmt)
                return 0;

        for (p = __fmt; *p != 0; p++) {
                if (in_format == 0 && *p == '%') {
                        in_format = 1;
                        type = 0;
                } else if (in_format == 1 && *p == '%') {
                        in_format = 0;
                } else if (in_format) {
                        switch (*p) {
                        /* Conversion specifiers */
                        case 'd':
                        case 'i':
                        case 'o':
                        case 'u':
                        case 'x':
                        case 'X':
                                /* Looks like integers can't have PA_FLAG_LONG_LONG */
                                if ((type & PA_FLAG_LONG_LONG) != 0)
                                        type ^= PA_FLAG_LONG_LONG;

                                if (type == PA_CHAR)
                                        __assign_argtypes(__argtypes, res++, __n, PA_CHAR);
                                else
                                        __assign_argtypes(__argtypes, res++, __n, type | PA_INT);
                                in_format = 0;
                                break;
                        case 'f':
                        case 'F':
                        case 'e':
                        case 'E':
                        case 'g':
                        case 'G':
                        case 'a':
                        case 'A':
                                if ((type & PA_FLAG_LONG_LONG) != 0)
                                        __assign_argtypes(__argtypes,
                                                          res++,
                                                          __n,
                                                          PA_FLAG_LONG_LONG | PA_DOUBLE);
                                else
                                        __assign_argtypes(__argtypes, res++, __n, PA_DOUBLE);
                                in_format = 0;
                                break;
                        case 'c':
                                __assign_argtypes(__argtypes, res++, __n, PA_CHAR);
                                in_format = 0;
                                break;
                        case 's':
                                __assign_argtypes(__argtypes, res++, __n, PA_STRING);
                                in_format = 0;
                                break;
                        case 'p':
                                __assign_argtypes(__argtypes, res++, __n, PA_POINTER);
                                in_format = 0;
                                break;
                        case 'n':
                                __assign_argtypes(__argtypes, res++, __n, PA_FLAG_PTR);
                                in_format = 0;
                                break;

                        /* Length modifiers */
                        case 'L':
                                type |= PA_FLAG_LONG_LONG;
                                break;
                        case 'j':
                        case 'z':
                        case 't':
                                type |= PA_FLAG_LONG;
                                break;
                        case 'l':
                                if ((type & PA_FLAG_LONG) != 0 || (type & PA_FLAG_LONG_LONG) != 0)
                                        type |= PA_FLAG_LONG_LONG;
                                else
                                        type |= PA_FLAG_LONG;
                                break;
                        case 'h':
                                if ((type & PA_FLAG_SHORT) != 0)
                                        type = PA_CHAR;
                                else
                                        type |= PA_FLAG_SHORT;
                                break;

                        /* Numbers and flags */
                        case '0':
                        case '1':
                        case '2':
                        case '3':
                        case '4':
                        case '5':
                        case '6':
                        case '7':
                        case '8':
                        case '9':
                        case '.':
                        case '#':
                        case '+':
                        case '-':
                        case '\'':
                                break;
                        case '*':
                                __assign_argtypes(__argtypes, res++, __n, PA_INT);
                                break;

                        default:
                                in_format = 0;
                                break;
                        }
                } else
                        in_format = 0;
        }

        return res > 0 ? res - 1 : res;
}
