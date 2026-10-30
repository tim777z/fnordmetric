# Architecture

FnordMetric is split into:

- `fnordmetric-core/`: C++ core. `src/sql` contains the SQL parser, runtime,
  execution engine, and storage backends (CSV, MySQL, PostgreSQL). `src/ui`
  contains chart primitives. `src/metricdb` implements the metric database.
- `fnordmetric-webui/`: plain JavaScript UI bundle assembled by `make`.
- `fnordmetric-doc/`: documentation and examples.
- `fnordmetric-agents/`: legacy collection agents.

Build entry point is the root `Makefile`, which drives CMake in
`fnordmetric-core/build/cmake` and concatenates the web UI bundle.