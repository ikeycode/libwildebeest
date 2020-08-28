pkg = import('pkgconfig')

libftw = static_library('wildebeest-ftw',
    sources: ['lwb_ftw.c'],
    install: true,
    include_directories: root_includedir,
)

pkg.generate(libftw,
    name: 'libwildebeest-ftw',
    subdirs: 'libwildebeest',
    extra_cflags: '--include=lwb_ftw.h',
    libraries: '-Wl,--wrap=ftw -Wl,--wrap=nftw',
)
