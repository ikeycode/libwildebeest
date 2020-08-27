pkg = import('pkgconfig')

libqsort = static_library('wildebeest-qsort',
    sources: ['lwb_qsort.c'],
    install: true,
    include_directories: root_includedir,
)

pkg.generate(libqsort,
    name: 'libwildebeest-qsort',
    subdirs: 'libwildebeest',
    extra_cflags: '--include=lwb_qsort.h',
    libraries: '-Wl,--wrap=qsort_r',
)
