pkg = import('pkgconfig')

libpwd = static_library('wildebeest-pwd',
    sources: ['lwb_pwd.c'],
    install: true,
    include_directories: root_includedir,
)

pkg.generate(libpwd,
    name: 'libwildebeest-pwd',
    subdirs: 'libwildebeest',
    extra_cflags: '--include=lwb_pwd.h',
    libraries: '-Wl,--wrap=fgetpwent_r',
)
