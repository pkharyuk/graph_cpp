FROM ubuntu:22.04

RUN apt update \
    && apt install -y \
        make \
        gcc \
        bash \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /demo
COPY ./ ./
RUN chmod +x ./scripts.sh

CMD ["./scripts.sh", "compile"]