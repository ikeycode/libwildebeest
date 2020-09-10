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

#include "lwb_qsort.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_ELEM_SIZE 64
#define ELEM(X, I, S) ((X) + (I * S))
#define INVALID_ELEMENT_SIZE ((size_t)-1)

static inline int __swap_items(unsigned char *a, unsigned char *b, size_t size)
{
        unsigned char buf[MAX_ELEM_SIZE];
        unsigned char *tmp = buf;

        /* Avoid malloc if possible, however support bigger elements as well */
        if (size > MAX_ELEM_SIZE) {
                tmp = malloc(size);
                if (tmp == NULL)
                        return 1;
        }

        memcpy(tmp, a, size);
        memcpy(a, b, size);
        memcpy(b, tmp, size);

        if (tmp != buf)
                free(tmp);

        return 0;
}

/* This is Hoare partition scheme */
static size_t __partition(unsigned char *base, size_t lo, size_t hi, size_t size,
                          __compar_d_fn_t cmp, void *arg)
{
        unsigned char *pivotptr = ELEM(base, lo, size);

        while (1) {
                while (cmp(ELEM(base, lo, size), pivotptr, arg) < 0)
                        lo++;
                while (cmp(ELEM(base, hi, size), pivotptr, arg) > 0)
                        hi--;
                if (lo >= hi)
                        return hi;
                if (__swap_items(ELEM(base, lo, size), ELEM(base, hi, size), size) == 1)
                        return INVALID_ELEMENT_SIZE;
        }
}

static void __qsort_range(unsigned char *base, size_t lo, size_t hi, size_t size,
                          __compar_d_fn_t cmp, void *arg)
{
        size_t pivot;

        if (lo >= hi)
                return;
        pivot = __partition(base, lo, hi, size, cmp, arg);
        if (pivot == INVALID_ELEMENT_SIZE)
                return;
        __qsort_range(base, lo, pivot, size, cmp, arg);
        __qsort_range(base, pivot + 1, hi, size, cmp, arg);
}

void __wrap_qsort_r(void *base, size_t nmemb, size_t size, __compar_d_fn_t compar, void *arg)
{
        if (nmemb <= 1)
                return;
        __qsort_range(base, 0, nmemb - 1, size, compar, arg);
}
