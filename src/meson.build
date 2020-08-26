pkg = import('pkgconfig')

root_includedir = include_directories('include')

# Handle installation of headers
subdir('include')

# Actually build stuff now
subdir('string')

# Generate a global pkgconfig for enabling all features
pkg.generate(name: 'wildebeest',
    description: 'Globally enable all libwildebeest features',
    subdirs: 'libwildebeest',
    requires: [
        'wildebeest-string',
    ],
)
