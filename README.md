### libwildebeest

Serpent OS seeks to be **correct** wherever possible, in the most pragmatic sense.
Some upstream projects that we use, are incompatible with Clang/Musl/Both for a
variety of reasons, not always limited to non-compliance. In some cases, the
toolchain simply doesn't provide a viable alternative to the functionality
being used.

In these cases, we'll be using `libwildebeest`. It will provide a very
small overlay library with the missing definitions and functions to get
the software to build and run successfully.

Additionally we'll be able to track so-called 'GNUisms' in Serpent OS by
checking any package that build-depends upon `libwildebeest` and work with
various upstreams to eradicate the need for this software.

As such there is no promise of ABI stability, and static linking will be
preferred.

Our goal is simple, centralise extensive patchwork that seeks to recreate
libgcc_s/glibc compatibility in each program, instead of patching those
into every single software package.

## TODO

List of functions/APIs we definitely need to bolt on.
This list will be growing over time as we encounter more areas
of incompatibility.

 - `parse_printf_format` / `printf.h`
 - `strdupa` & `strndupa` / `string.h`
 - `strtoull_l` & `strtoll_l` / `string.h`

Potential musl function replacements for non-POSIX expectation:

 - `nftw` (missing CONTINUE, etc.)
 - `getopt` (GNU libc permutates `argv` unless `:` is the first arg, POSIX stops at first non option)
 - `glob` family (missing brace support)
