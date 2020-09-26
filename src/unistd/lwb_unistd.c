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

#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

#include "lwb_unistd.h"
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <unistd.h>

#define HAS_MODE(M, U, G, O, UID, GID)                                                             \
        ((UID == M.st_uid && ((M.st_mode) & U)) || (GID == M.st_gid && ((M.st_mode) & G)) ||       \
         ((M.st_mode) & O))

int __wrap_faccessat(int fd, const char *filename, int amode, int flag)
{
        if (flag) {
                struct stat statbuf;
                int res;
                uid_t uid;
                uid_t gid;
                if (flag & AT_EACCESS) {
                        uid = geteuid();
                        gid = getegid();
                } else {
                        uid = getuid();
                        gid = getgid();
                }
                /* Filter out AT_EACCESS here and just pass AT_SYMLINK_NOFOLLOW if specified */
                res = fstatat(AT_FDCWD, filename, &statbuf, flag & AT_SYMLINK_NOFOLLOW);
                if (res)
                        return -1;
                /* Special case, root has always access */
                if (getuid() == 0)
                        return 0;
                switch (amode) {
                case F_OK:
                        return 0;
                case X_OK:
                        return HAS_MODE(statbuf, S_IXUSR, S_IXGRP, S_IXOTH, uid, gid) ? 0 : -1;
                case W_OK:
                        return HAS_MODE(statbuf, S_IWUSR, S_IWGRP, S_IWOTH, uid, gid) ? 0 : -1;
                case R_OK:
                        return HAS_MODE(statbuf, S_IRUSR, S_IRGRP, S_IROTH, uid, gid) ? 0 : -1;
                }
                return -1;
        }

        return (int)syscall(SYS_faccessat, fd, filename, amode);
}
