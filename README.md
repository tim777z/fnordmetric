# FnordMetric

FnordMetric is an open-source metrics and dashboarding toolkit built around a
small SQL-like query language. It ships a C++ core (`fnordmetric-core`) with a
charting/rendering library and a web UI (`fnordmetric-webui`).

## Prerequisites

- `git`
- A C++11-capable compiler (GCC or Clang)
- `cmake` >= 2.6
- `make`
- Optional: `libpq` (PostgreSQL backend) and `libmysqlclient` (MySQL backend)

On Debian/Ubuntu:

```bash
sudo apt-get install -y build-essential cmake libpq-dev default-libmysqlclient-dev
```

## Clone

```bash
git clone https://github.com/tim777z/fnordmetric.git
cd fnordmetric
```

## Build

```bash
make
```

This builds the web UI bundle and the C++ core in release mode.

## Install

```bash
sudo make install
```

## Test

```bash
make test
```

Runs the C++ unit test suites (`SQLTest`, `UITest`, and the smaller component
suites). The command exits non-zero on any failure.

## Run the server

```bash
mkdir -p /tmp/fnordmetric-data
./fnordmetric-core/build/cmake/target/fnordmetric-server --datadir /tmp/fnordmetric-data --verbose
```

## Run via Docker Compose

```bash
cp .env.example .env
# edit .env, then:
docker compose up --build
```

## Configuration

Runtime secrets and connection settings should be supplied via environment
variables. See `.env.example` for the full list; never commit a real `.env`.

## Dependencies

See `DEPENDENCIES.md` for pinned system dependency versions and notes on the
web UI build.

## License

See `LICENSE`.