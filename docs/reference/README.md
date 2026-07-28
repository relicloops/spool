# Spool reference

Spool is a Valve companion. Valve parses and validates `argc` / `argv`. Spool
owns the hand-off after a successful parse: snapshot, destroy, dispatch.

## Public API

Declared in `include/spool/spool.h`:

| Symbol | Role |
|--------|------|
| `spool_route_t` | Route entry: `.verb`, optional `.subverb`, `.run` handler |
| `spool_handler_fn` | `int (*)(const char *verb, const char *subverb, void *userdata)` |
| `spool_dispatch` | Take ownership of `valve_t **`, destroy it, run a matching route |
| `spool_version_get` | Library version string (`MAJOR.MINOR.PATCH-BUILD`) |
| `spool_version_string` | Banner-style version string |

The header forward-declares `valve_t` only. Implementation sources include
`valve/valve.h` and link Valve.

## Ownership

`spool_dispatch(valve_t **v, const spool_route_t *routes, void *userdata)`:

1. Rejects `v == NULL`, `*v == NULL`, or `routes == NULL` with `-1` (no destroy).
2. Reads `vl_verb_get` / `vl_subverb_get` and copies into stack buffers.
3. Calls `vl_destroy(*v)` and sets `*v = NULL`.
4. Finds a matching route and returns the handler’s return code, or `-1` if none.

After a successful ownership take (step 3), Valve heap is gone even when no
route matches. Callers that skip dispatch (parse errors, reserved help/version)
must still `vl_destroy` when `v` is non-NULL.

## Route matching

Routes are a NULL-terminated table (last entry has `.verb == NULL`).

- Verb must match exactly (`strcmp`).
- If route `.subverb` is non-NULL, it must match the active subverb exactly.
- If route `.subverb` is `NULL`, any subverb (including none) matches.
- Entries with `.run == NULL` are skipped.
- First matching entry wins.

## Snapshot limit

Verb and subverb are copied into 256-byte buffers before `vl_destroy`. Longer
strings are truncated by `snprintf`. Keep verb and subverb names well under
that limit in schemas.

## Versioning

Form: `MAJOR.MINOR.PATCH-BUILD` (tag `vMAJOR.MINOR.PATCH-BUILD`). While
experimental `1.x`, pin exact tags; MINOR may break the public contract.
