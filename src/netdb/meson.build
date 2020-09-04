pkg = import('pkgconfig')

libnetdb = static_library('wildebeest-netdb',
    sources: ['lwb_netdb.c'],
    install: true,
    include_directories: root_includedir,
)

pkg.generate(libnetdb,
    name: 'libwildebeest-netdb',
    subdirs: 'libwildebeest',
    libraries: '-Wl,--wrap=sethostent -Wl,--wrap=endhostent -Wl,--wrap=gethostent -Wl,--wrap=gethostent_r',
)
