# Spool

*Parse with Valve. Dispatch with Spool.*

[![CI](https://github.com/relicloops/spool/actions/workflows/ci.yml/badge.svg)](https://github.com/relicloops/spool/actions/workflows/ci.yml)
[![License](https://img.shields.io/badge/license-Apache--2.0-blue.svg)](LICENSE)
[![Status](https://img.shields.io/badge/status-experimental-orange.svg)](#spool)
[![Version](https://img.shields.io/badge/version-1.0.0--000-lightgrey.svg)](meson.build)
[![Linux](https://img.shields.io/badge/Linux-supported-FCC624?logo=linux&logoColor=black)](.github/workflows/ci.yml)
[![macOS](https://img.shields.io/badge/macOS-supported-000000?logo=apple&logoColor=white)](.github/workflows/ci.yml)

## Introduction

Spool is the optional [Valve](https://github.com/relicloops/valve) companion for C programs. After a successful `vl_parse`, it takes ownership of the `valve_t`, snapshots the active verb and subverb, destroys the parser, and dispatches to a NULL-terminated route table of handlers.

Valve remains the argv parser. Apps that prefer a hand-rolled `strcmp` ladder never need to link Spool. The public header only forward-declares `valve_t`; it does not include `valve/valve.h`.

## Status and limitations

Spool `1.x` is experimental. Minor releases may change the public source or ABI contract, so consumers should pin an exact version tag. Spool will announce when the experimental status is lifted; from that release onward, breaking public API or ABI changes require a major-version bump, backward-compatible additions require a minor-version bump, and fixes require a patch-version bump. The final `BUILD` component identifies the published build.

`spool_dispatch` snapshots verb and subverb into fixed 256-byte buffers before destroying Valve. Longer verb or subverb strings are truncated. Route matching treats a `NULL` route `.subverb` as “any subverb (including none)”. On unknown routes or bad arguments, Valve is still destroyed when ownership was taken, and the call returns `-1`.

For ownership and matching rules, see the [reference](docs/reference/README.md). For a complete demo, see the [example](docs/example/README.md).

## Quick start

Create a small project with three files:

```text
spool-demo/
├── main.c
├── meson.build
└── subprojects/
    ├── valve.wrap
    └── spool.wrap
```

### `main.c`

Parse with Valve, then dispatch with Spool:

```c
#include "spool/spool.h"
#include "valve/valve.h"

#include <stdio.h>

static int serve_run(const char *verb, const char *subverb, void *userdata) {
  (void)verb;
  (void)subverb;
  (void)userdata;
  puts("serving");
  return 0;
}

static const spool_route_t routes[] = {
    {.verb = "serve", .subverb = nullptr, .run = serve_run},
    {.verb = nullptr},
};

int main(int argc, char **argv) {
  static const vl_verb_t serve_cmd = {
      .name = "serve",
      .description = "Run the server.",
  };
  static const vl_verb_t *const verbs[] = {&serve_cmd, nullptr};
  const vl_executable_t settings = {
      .program_name = "demo",
      .program_version = "0.1.0",
      .verbs = verbs,
  };

  valve_t *v = vl_create(&settings);
  if (!v)
    return 1;

  if (vl_parse(v, argc, argv) != 0) {
    vl_errors_print(v, stderr);
    vl_destroy(v);
    return 2;
  }

  int rc = 0;
  if (!vl_reserved_fired(v))
    rc = spool_dispatch(&v, routes, nullptr);
  if (v)
    vl_destroy(v);
  return rc < 0 ? 3 : rc;
}
```

### `subprojects/valve.wrap`

```ini
[wrap-git]
directory = valve
url = https://github.com/relicloops/valve.git
revision = v1.0.0-000
```

### `subprojects/spool.wrap`

Pin a published Spool version tag rather than tracking `main`:

```ini
[wrap-git]
directory = spool
url = https://github.com/relicloops/spool.git
revision = v1.0.0-000
```

### `meson.build`

```meson
project(
  'spool-demo',
  'c',
  default_options: ['c_std=c23'],
)

spool_proj = subproject('spool')
spool_dep = spool_proj.get_variable('spool_dep')

executable('demo', 'main.c', dependencies: spool_dep)
```

Spool pulls Valve transitively through `spool_dep`. Keep an explicit `valve_dep` only when your sources include `valve/valve.h` directly and you prefer a separate dependency entry.

### Build and run

```bash
meson setup build
meson compile -C build

./build/demo --help
./build/demo serve
```

## Install

Install Spool from a source checkout to a prefix (Valve must be available as a subproject wrap or already installed):

```bash
meson setup build --buildtype=release --prefix="$HOME/.local"
meson compile -C build
meson install -C build
```

This installs `libspool`, the public header under `include/spool`, and `spool.pc`. Meson can then consume the installed library:

```meson
spool_dep = dependency('spool')
```

## Development

```bash
git clone https://github.com/relicloops/spool.git
cd spool
```

### Testing with Ceedling

Ceedling requires Ruby, Meson 1.4 or newer, and a compiler that can build C23 (GCC 14+ or Clang 18+). Install Ruby, Meson, Ninja, and the compiler with your system package manager, then install the pinned test runner and coverage reporter:

```bash
gem install ceedling -v 1.1.1
python3 -m pip install gcovr
```

Meson is the front door for both the library build and the test system:

```bash
meson setup build -Dtests=enabled
meson compile -C build
meson test -C build --print-errorlogs
```

That `meson test` step runs Ceedling `gcov:all` and produces `.cache/ceedling/artifacts/gcov/gcovr/spool-coverage.html`.

After configure, use the generated wrapper for direct Ceedling work:

```bash
build/spool-ceedling test:all
build/spool-ceedling gcov:all
```

Or open a devenv so `spool-ceedling` is on `PATH`:

```bash
meson devenv -C build
spool-ceedling test:all
```

See [Contributing](CONTRIBUTING.md) for prerequisites and CI details.

Local Valve co-dev (gitignored):

```bash
ln -sfn ../valve subprojects/valve
```

## License

Spool is licensed under Apache-2.0. See the [license](LICENSE).
