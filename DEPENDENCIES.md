# Dependencies

## C++ core

Build dependencies (install via the system package manager). Pinned versions
used by CI and the Docker image:

- GCC >= 11 or Clang >= 14 with C++11 support
- CMake >= 3.16
- Make >= 4.3
- libpq >= 14.0 (optional, enables the PostgreSQL backend)
- libmysqlclient >= 8.0 (optional, enables the MySQL backend)

Feature macros are detected at configure time (`FNORD_ENABLE_POSTGRES`,
`FNORD_ENABLE_MYSQL`). CI installs `libpq-dev` and `default-libmysqlclient-dev`
on ubuntu-latest, which satisfy the pins above.

Optional feature macros are detected at configure time (`FNORD_ENABLE_POSTGRES`,
`FNORD_ENABLE_MYSQL`).

## Web UI

`fnordmetric-webui` is a plain JavaScript bundle assembled with `make`; it has
no runtime npm dependencies. See `fnordmetric-webui/Makefile`.

## Reproducible installs

The root `Makefile` is the single entry point:

```
make          # build
make test     # build and run unit tests
make install  # install binaries
make clean    # remove build artifacts
```