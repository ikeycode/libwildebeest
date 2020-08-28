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
#ifndef _LWB_GLOB_H
#define _LWB_GLOB_H

/* Override musl's glob header completely. */
#ifndef _GLOB_H
#define _GLOB_H

#include "lwb_common.h"

#include <stddef.h>
#include <sys/stat.h>

/**
 * GNU-compatible glob_t structure
 */
typedef struct {
        size_t gl_pathc;
        char **gl_pathv;
        size_t gl_offs;
        int gl_flags;

        void (*gl_closedir)(void *);
        struct dirent *(*gl_readdir)(void *);
        void *(*gl_opendir)(const char *);
        int (*gl_lstat)(const char *__restrict, struct stat *__restrict);
        int (*gl_stat)(const char *__restrict, struct stat *__restrict);
} glob_t;

int glob(const char *__restrict, int, int (*)(const char *, int), glob_t *__restrict);
void globfree(glob_t *);

#if defined(_LARGEFILE64_SOURCE) || defined(_GNU_SOURCE)
#define glob64 glob
#define globfree64 globfree
#define glob64_t glob_t
#endif

#endif /* _GLOB_H */
#endif /* _LWB_GLOB_H */
