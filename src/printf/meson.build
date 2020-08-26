pkg = import('pkgconfig')

libprintf = static_library('wildebeest-printf',
    sources: ['lwb_printf.c'],
    install: true,
    include_directories: root_includedir,
)

pkg.generate(libprintf,
    name: 'libwildebeest-printf',
    subdirs: 'libwildebeest',
    libraries: '-Wl,--wrap=parse_printf_format',
)
