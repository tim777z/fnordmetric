# Contributing to FnordMetric

## Build and test

```bash
make          # build the web UI bundle and the C++ core
make test     # run the C++ unit test suites (must pass before submitting)
```

## Workflow

- Keep each feature or fix in its own commit (or small PR) that includes the
  tests pinning the new behavior.
- Avoid bulk commits that mix formatting, refactors, and features.
- Run `make test` locally before opening a PR; CI runs the same command.
- Security issues: see `SECURITY.md`.

## Style

- C++ code follows `.clang-format` (Google-based, 2-space indent). Run
  `clang-format --dry-run --Werror` on touched files.
- Static analysis runs via `.clang-tidy` (bugprone/readability checks).
- Do not commit real credentials; copy `.env.example` to `.env` locally.

## Dependencies

See `DEPENDENCIES.md` for pinned system dependency versions.
---