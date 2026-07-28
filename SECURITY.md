# Security Policy

## Supported versions

Spool is **experimental `1.0.x`** (Apache-2.0). Only the **latest published
`1.0.x`** line is considered for security fixes. Older tags and pre-release
scratch builds are unsupported. The API may change while Spool remains
experimental.

| Version | Supported         |
|---------|-------------------|
| 1.0.x   | Yes (latest only) |
| < 1.0   | No                |

## Reporting a vulnerability

**Do not** open a public GitHub issue for security-sensitive reports.

Prefer GitHub’s private advisory flow:

1. Open a [private security advisory](https://github.com/relicloops/spool/security/advisories/new) on this repository.
2. Include Spool version as `MAJOR.MINOR.PATCH-BUILD` (e.g. `1.0.0-000` from
   `spool_version_get()` / `meson.build`), OS, compiler, and a minimal route
   table or argv repro when possible.
3. Allow reasonable time for triage before any public disclosure.

If private advisories are unavailable for your account, contact the maintainers
listed on the [relicloops](https://github.com/relicloops) GitHub organization,
and mark the report as security-sensitive.

## Scope notes

Spool is a post-parse verb dispatch companion for Valve (route tables,
ownership transfer, version helpers). Security issues in applications that
*use* Spool, or defects clearly in Valve itself, belong in those repositories
unless the defect is clearly in Spool.
