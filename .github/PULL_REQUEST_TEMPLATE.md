## Summary

<!-- What does this change, and why? Keep Spool experimental-status honest if the API moves. -->

## Test plan

- [ ] `meson setup build` (or reconfigure) and `meson compile -C build`
- [ ] `meson test -C build --print-errorlogs` (unit tests + gcovr HTML)
- [ ] Example smoke (if behaviour changes): `docs/example` help / serve path

## Notes for reviewers

<!-- API impact, route-table examples, or areas that need a careful look. -->

See [CONTRIBUTING.md](../CONTRIBUTING.md) for local setup.
