FROM rikorose/gcc-cmake:gcc-11

LABEL Description="DIGNEA environment"

ENV HOME /root

SHELL ["/bin/bash", "-c"]

COPY . /dignea

RUN apt-get update && apt-get install -y software-properties-common

RUN add-apt-repository ppa:ubuntu-toolchain-r/test

RUN rm -rf /dignea/.github /dignea/.git /dignea/.vscode

WORKDIR /dignea