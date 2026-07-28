# Spool example

Minimal demo: Valve parses a `serve` verb, then Spool destroys the parser and
dispatches to a route handler.

## Build

From this directory (or the repo root with a path adjust):

```bash
# From repository root — prefer the documented wrap workflow in the README.
# This example expects Valve and Spool available via dependency/fallback.
cd docs/example
ln -sfn ../../../valve subprojects/valve   # optional local co-dev
ln -sfn ../.. subprojects/spool            # build against this checkout
meson setup build
meson compile -C build
./build/spool-demo serve
```

When consuming published tags, use `subprojects/valve.wrap` and
`subprojects/spool.wrap` as shown in the root README instead of symlinks.
