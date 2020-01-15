FROM ubuntu:16.04

RUN apt-get update
RUN apt-get install -y --no-install-recommends \
    build-essential \
    zlib1g-dev \
    pkg-config \
    libglib2.0-dev \
    binutils-dev \
    libboost-all-dev \
    autoconf \
    libtool \
    libssl-dev \  
    libpixman-1-dev \
    libpython-dev \ 
    python-pip \ 
    python-capstone \
    virtualenv \
    flex \
    bison \
    wget

RUN cd /root && \
    wget https://download.qemu.org/qemu-4.2.0.tar.xz && \
    tar -xf qemu-4.2.0.tar.xz && \
    cd qemu-4.2.0 && \
    ./configure && \
    make -j$(nproc) && \
    make install