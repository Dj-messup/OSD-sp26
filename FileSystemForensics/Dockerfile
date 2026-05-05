FROM ubuntu:24.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    dosfstools \
    mtools \
    binutils \
    coreutils \
    util-linux \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /work
