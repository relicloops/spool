spool_inc = include_directories('.')

# Embedded consumers link the library directly; only a standalone build
# installs the public header into the system prefix.
if not meson.is_subproject()
  install_headers(
    'spool/spool.h',
    subdir: 'spool',
  )
endif
