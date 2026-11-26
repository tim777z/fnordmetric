# Changelog

All notable changes to FnordMetric are documented here. The project follows
semantic versioning once a tagged release is cut; until then entries are
grouped by date.

## Unreleased

### Added
- `.clang-format` and `.clang-tidy` configuration for the C++ core.
- `docker-compose.test.yml` for ephemeral PostgreSQL/MySQL test databases.
- `CONTRIBUTING.md` and `CHANGELOG.md`.
- `.github/dependabot.yml` for npm, bundler, and GitHub Actions updates.
- Coverage build option (`COVERAGE=ON`) and `make coverage` target.
- CI lint, coverage, and dependency audit steps.

### Changed
- README documents the isolated test flow and dependency pinning.
- `DEPENDENCIES.md` lists pinned system dependency versions.
- `EXPECT_EXCEPTION` keeps exception messages alive while formatting diagnostics,
  removing a dangling-pointer failure on modern GCC and restoring CI test signal.
- Added `test-unittest` regression coverage for exception-match diagnostics.
---