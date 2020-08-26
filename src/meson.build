pkg = import('pkgconfig')

root_includedir = include_directories('include')

# Handle installation of headers
subdir('include')

# Actually build stuff now
subdir('string')
subdir('printf')

# Generate a global pkgconfig for enabling all features
pkg.generate(name: 'libwildebeest',
    description: 'Globally enable all libwildebeest features',
    subdirs: 'libwildebeest',
    requires: [
        'libwildebeest-string',
        'libwildebeest-printf',
    ],
)
