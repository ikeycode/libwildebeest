pkg = import('pkgconfig')

libstdlib = static_library('wildebeest-stdlib',
    sources: ['lwb_stdlib.c'],
    install: true,
    include_directories: root_includedir,
)

pkg.generate(libstdlib,
    name: 'libwildebeest-stdlib',
    subdirs: 'libwildebeest',
    extra_cflags: '--include=lwb_stdlib.h',
    libraries: '-Wl,--wrap=strtoll_l -Wl,--wrap=strtoull_l',
)
