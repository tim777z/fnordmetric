# Dependencies

## C++ core

Build dependencies (install via the system package manager):

- GCC or Clang with C++11 support
- CMake >= 2.6
- Make
- libpq >= 9.0 (optional, enables the PostgreSQL backend)
- libmysqlclient (optional, enables the MySQL backend)

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