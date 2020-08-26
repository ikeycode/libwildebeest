pkg = import('pkgconfig')

libstring = static_library('wildebeest-string',
    sources: ['lwb_string.c'],
    install: true,
    include_directories: root_includedir,
)

pkg.generate(libstring,
    name: 'libwildebeest-string',
    subdirs: 'libwildebeest',
    extra_cflags: '--include=lwb_string.h',
    libraries: '-Wl,--wrap=strndupa',
)
