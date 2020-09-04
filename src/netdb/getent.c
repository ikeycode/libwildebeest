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

#ifndef ENTNAME
#error "ENTNAME must be defined"
#endif
#ifndef ENTFILE
#error "ENTFILE must be defined"
#endif
#ifndef ENTPREFIX
#error "ENTPREFIX must be defined"
#endif

#define MAX_LINE_SIZE 1024
#define MAX_ALIASES 32
#define ENT_ADDRESS_LIST_SIZE (2 * sizeof(char *))
#define ENT_ADDRESS_ENTRY_SIZE sizeof(struct in6_addr)
#define ENT_ADDRESS_SIZE (ENT_ADDRESS_LIST_SIZE + ENT_ADDRESS_ENTRY_SIZE)
#define MAX_BUFFER_SIZE (MAX_LINE_SIZE + MAX_ALIASES * sizeof(char *) + ENT_ADDRESS_SIZE)

#define CONCAT(X, Y) X##Y
#define CONCAT2(X, Y, Z) X##Y##Z
#define PREFIX(X, Y) CONCAT(X, Y)
#define PREPOSTFIX(X, Y, Z) CONCAT2(X, Y, Z)
#define VARDEF(X) PREFIX(X, ENTNAME)
#define VARDEF_R(X) PREPOSTFIX(X, ENTNAME, _r)
#define ENTRY(X) PREFIX(ENTPREFIX, X)

static FILE *VARDEF(__file) = NULL;

static struct ENTNAME *VARDEF(__get)(FILE *f, struct ENTNAME *ent, char **line, size_t *size,
                                     struct ENTNAME **res, int *errnoptr)
{
        for (;;) {
                ssize_t l = 0;
                char *item = NULL;
                char *saveptr = NULL;
                char *free_pos = NULL;
                size_t aliases_max = 0;
                size_t aliases_cnt = 0;

                if ((l = getline(line, size, f)) < 0) {
                        *errnoptr = NO_RECOVERY;
                        ent = NULL;
                        break;
                }
                if (l == 0)
                        continue;
                if (*size <= (size_t)l + 1 + ENT_ADDRESS_SIZE) {
                        *errnoptr = NO_RECOVERY;
                        ent = NULL;
                        break;
                }
                free_pos = *line + l + 1;
                aliases_max = (*size - (size_t)l - 1 - ENT_ADDRESS_SIZE) / sizeof(char *);
                ent->ENTRY(name) = 0;
                ent->ENTRY(aliases) = 0;
                if ((item = strchr(*line, '#'))) {
                        *item++ = '\n';
                        *item = 0;
                }
                item = strtok_r(*line, delims, &saveptr);
                if (item == NULL)
                        continue;
#if ENT_ADDRLIST
                ent->ENTRY(addr_list) = (char **)free_pos;
                free_pos += ENT_ADDRESS_LIST_SIZE;
                ent->ENTRY(addr_list[0]) = free_pos;
                ent->ENTRY(addr_list[1]) = NULL;
                free_pos += ENT_ADDRESS_ENTRY_SIZE;
                if (__str_to_addr(item,
                                  ent->ENTRY(addr_list[0]),
                                  &ent->ENTRY(addrtype),
                                  &ent->ENTRY(length)) != 1)
                        continue;
                item = strtok_r(NULL, delims, &saveptr);
#endif
                if (item == NULL) {
                        ent->ENTRY(name) = "";
                        break;
                }
                ent->ENTRY(name) = item;
#if !ENT_ADDRLIST
                item = strtok_r(NULL, delims, &saveptr);
                if (inet_aton(item, (struct in_addr *)&ent->ENTRY(net)) == 0)
                        continue;
                ent->ENTRY(net) = htonl(ent->ENTRY(net));
#endif
                ent->ENTRY(aliases) = (char **)free_pos;
                item = strtok_r(NULL, delims, &saveptr);
                for (aliases_cnt = 0; item != NULL && aliases_cnt < aliases_max; aliases_cnt++) {
                        ent->ENTRY(aliases)[aliases_cnt] = item;
                        item = strtok_r(NULL, delims, &saveptr);
                }
                ent->ENTRY(aliases)[aliases_cnt] = NULL;
                break;
        }
        if (res)
                *res = ent;
        return ent;
}

void VARDEF(__wrap_set)(int stayopen __attribute__((unused)))
{
        if (VARDEF(__file) != NULL)
                fclose(VARDEF(__file));
        VARDEF(__file) = NULL;
}

void VARDEF(__wrap_end)(void)
{
        VARDEF(__wrap_set)(0);
}

static inline FILE *VARDEF(__open_file_)(void)
{
        if (VARDEF(__file) == NULL)
                VARDEF(__file) = fopen(ENTFILE, "r");
        return VARDEF(__file);
}

int VARDEF_R(__wrap_get)(struct ENTNAME *ret, char *buf, size_t buflen, struct ENTNAME **res,
                         int *h_errnop)
{
        if (VARDEF(__open_file_)() == NULL)
                return 1;
        if (VARDEF(__get)(VARDEF(__file), ret, &buf, &buflen, res, h_errnop) == NULL)
                return 1;
        return 0;
}

struct ENTNAME *VARDEF(__wrap_get)(void)
{
        static char *__buf = NULL;
        static size_t __size = 0;
        static struct ENTNAME __ent;
        struct ENTNAME *res = &__ent;
        if (__buf == NULL) {
                __buf = calloc(1, MAX_BUFFER_SIZE);
                __size = MAX_BUFFER_SIZE;
        }
        if (VARDEF_R(__wrap_get)(&__ent, __buf, __size, &res, &errno) == 1)
                return NULL;
        return res;
}

#undef MAX_LINE_SIZE
#undef MAX_ALIASES
#undef ENT_ADDRESS_LIST_SIZE
#undef ENT_ADDRESS_ENTRY_SIZE
#undef ENT_ADDRESS_SIZE
#undef MAX_BUFFER_SIZE
#undef CONCAT
#undef CONCAT2
#undef PREFIX
#undef PREPOSTFIX
#undef VARDEF
#undef VARDEF_R
#undef ENTRY
