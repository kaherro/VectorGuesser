FROM ubuntu:24.04 AS builder

RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential cmake git libsqlite3-dev sqlite3 ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY CMakeLists.txt ./
COPY src ./src
RUN mkdir build && cd build && cmake .. && make -j"$(nproc)"

COPY data/cities.sql ./data/cities.sql
RUN sqlite3 /app/data/cities.db < /app/data/cities.sql

FROM ubuntu:24.04 AS runtime

RUN apt-get update && apt-get install -y --no-install-recommends \
    libsqlite3-0 \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY --from=builder /app/build/vector_guesser ./build/vector_guesser
COPY --from=builder /app/data/cities.db ./data/cities.db
COPY web ./web

WORKDIR /app/build
EXPOSE 18080
CMD ["./vector_guesser"]