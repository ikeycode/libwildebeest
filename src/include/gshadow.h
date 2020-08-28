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
#ifndef _LWB_GSHADOW_H
#define _LWB_GSHADOW_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lwb_common.h"

#include <stddef.h>
#include <stdio.h>

/**
 * Group structure
 */
struct sgrp {
        char *sg_namp;   /**<Group name.  */
        char *sg_passwd; /**<Password hash  */
        char **sg_adm;   /**<Group admins  */
        char **sg_mem;   /**<Group members  */
};

void setsgent(void);
void endsgent(void);
struct sgrp *getsgent(void);
struct sgrp *getsgnam(const char *__name);
struct sgrp *sgetsgent(const char *__string);
struct sgrp *fgetsgent(FILE *__stream);
int putsgent(const struct sgrp *__g, FILE *__stream);

#ifdef __cplusplus
}
#endif

#endif /* _LWB_GSHADOW_H */
