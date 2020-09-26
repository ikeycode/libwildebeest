pkg = import('pkgconfig')

libunistd = static_library('wildebeest-unistd',
    sources: ['lwb_unistd.c'],
    install: true,
    include_directories: root_includedir,
)

pkg.generate(libunistd,
    name: 'libwildebeest-unistd',
    subdirs: 'libwildebeest',
    extra_cflags: '--include=lwb_unistd.h',
    libraries: '-Wl,--wrap=faccessat',
)
