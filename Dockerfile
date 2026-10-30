FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update \
 && apt-get install -y --no-install-recommends \
        build-essential \
        cmake \
        libpq-dev \
        default-libmysqlclient-dev \
 && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . .

# Build the project and webui assets.
RUN make

# Runtime data directory.
RUN mkdir -p /data/fnordmetric

EXPOSE 8080 8125

CMD ["/app/fnordmetric-core/build/cmake/target/fnordmetric-server", \
     "--datadir", "/data/fnordmetric", "--verbose"]