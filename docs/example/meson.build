project(
  'spool-example',
  'c',
  meson_version: '>=1.4',
  default_options: ['c_std=c23'],
)

spool_dep = dependency(
  'spool',
  fallback: ['spool', 'spool_dep'],
)

executable(
  'spool-demo',
  'main.c',
  dependencies: spool_dep,
)
