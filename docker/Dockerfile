FROM ubuntu:focal as build

LABEL description = "Build container for TEMPLEX"

RUN apt-get update && apt-get install --no-install-recommends --yes \
    gcc-9 g++-9 \
    cmake make ninja-build \
    llvm-10 llvm-10-dev clang-10 libclang-10-dev \
    clang-format-10 libboost-dev

RUN apt-get clean && apt-get auto-remove --yes

RUN ln -s /usr/bin/gcc-9 /usr/bin/gcc && \
    ln -s /usr/bin/g++-9 /usr/bin/g++

COPY docker/build-script.sh /usr/local/bin