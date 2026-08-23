# Valve is an installed RelicLoops library, found through pkg-config like any
# other. It was previously pulled in with subproject(), which forces a source
# build of Valve every time Spool configures -- unconditionally, even on a box
# where Valve is already installed and has been for months. Valve has no
# dependencies of its own, so nothing was gained by vendoring it, and the
# checked-out copy under subprojects/ drifted against its own wrap revision.
valve_dep = dependency('valve')

spool_c_args = [
  '-DSPOOL_VERSION="' + spool_version + '"',
]
if host_machine.system() == 'linux'
  spool_c_args += '-D_GNU_SOURCE'
endif

spool_sources = files(
  'unit/dispatch.c',
  'unit/route_find_.c',
  'unit/run_routes_.c',
  'unit/version_get.c',
  'unit/version_string.c',
)

spool_standalone = not meson.is_subproject()

spool_lib = library(
  'spool',
  spool_sources,
  include_directories: spool_inc,
  dependencies: valve_dep,
  c_args: spool_c_args,
  install: spool_standalone,
  version: spool_version.split('-')[0],
  soversion: '1',
)

spool_dep = declare_dependency(
  include_directories: spool_inc,
  link_with: spool_lib,
  dependencies: valve_dep,
)

meson.override_dependency('spool', spool_dep)

# A pkg-config file describes an installed library. When Spool is embedded
# as a subproject the library is never installed, so the .pc would dangle.
if spool_standalone
  pkg = import('pkgconfig')
  pkg.generate(
    name: 'spool',
    description: 'RelicLoops Valve companion — post-parse verb dispatch',
    version: spool_version,
    libraries: spool_lib,
    requires: 'valve',
    subdirs: '',
  )
endif
