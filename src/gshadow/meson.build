pkg = import('pkgconfig')

libgshadow = static_library('wildebeest-gshadow',
    sources: ['lwb_gshadow.c'],
    install: true,
    include_directories: root_includedir,
)

pkg.generate(libgshadow,
    name: 'libwildebeest-gshadow',
    subdirs: 'libwildebeest',
    libraries: '-Wl,--wrap=setsgent -Wl,--wrap=endsgent -Wl,--wrap=getsgent -Wl,--wrap=getsgnam -Wl,--wrap=sgetsgent -Wl,--wrap=fgetsgent -Wl,--wrap=putsgent -Wl,--wrap=getsgent_r -Wl,--wrap=getsgnam_r -Wl,--wrap=sgetsgent_r -Wl,--wrap=fgetsgent_r',
)
