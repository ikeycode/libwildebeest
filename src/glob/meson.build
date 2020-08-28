pkg = import('pkgconfig')

libglob = static_library('wildebeest-glob',
    sources: ['lwb_glob.c'],
    install: true,
    include_directories: root_includedir,
)

pkg.generate(libglob,
    name: 'libwildebeest-glob',
    subdirs: 'libwildebeest',
    extra_cflags: '--include=lwb_glob.h',
    libraries: '-Wl,--wrap=glob_r',
)
