FROM ubuntu:16.04

ENV DEBIAN_FRONTEND noninteractive
ENV TERM linux
ENV PATH="/opt/rtems/4.11/bin:${PATH}"
ENV RTEMS_MAKEFILE_PATH=/opt/rtems/4.11/arm-rtems4.11/raspberrypi

RUN echo "############### Installing dependencies ###############"
RUN apt-get update && apt-get install -y --no-install-recommends \
    binutils \
    make \
    patch \
    gcc \
    g++ \
    gdb \
    pax \
    python2.7-dev \
    zlib1g-dev \
    git \
    bison \
    flex \
    texinfo \
    bzip2 \
    xz-utils \
    unzip \
    libtinfo-dev && \
    ln -T /usr/bin/python2.7 /usr/bin/python

RUN echo "############### Building crosscompiler ###############"
RUN mkdir -p $HOME/dev/rtems && \
    cd $HOME/dev/rtems && \
    git clone --branch 4.11.3 --depth 1 git://git.rtems.org/rtems-source-builder.git rsb && \
    cd rsb && \
    ./source-builder/sb-check &&\
    cd rtems && \
    ../source-builder/sb-set-builder --prefix=/opt/rtems/4.11 4.11/rtems-arm

RUN echo "############### Builing kernel ###############"
RUN cd $HOME/dev/rtems && \
    git clone --branch 4.11.3 --depth 1 git://git.rtems.org/rtems.git kernel && \
    cd kernel && \
    ./bootstrap -c && ./bootstrap -p && ../rsb/source-builder/sb-bootstrap

RUN echo "############### Builing raspberrypi ###############"
RUN cd $HOME/dev && mkdir raspberrypi && cd raspberrypi && \
    ../rtems/kernel/configure \
        --prefix=/opt/rtems/4.11 \
        --target=arm-rtems4.11 \
        --enable-rtemsbsp=raspberrypi \
        --enable-tests=samples \
        --enable-posix \
        --enable-cxx && \
        # --enable-networking \
    make -j8 && make install

# RUN echo "############### Cleanup ###############"
# RUN cd / && \
#     rm $HOME/dev -r && \
#     rm /opt/rtems/4.11/lib/gcc/arm-rtems4.11/4.9.3/thumb -r && \
#     rm /opt/rtems/4.11/lib/gcc/arm-rtems4.11/4.9.3/eb -r && \
#     rm /opt/rtems/4.11/arm-rtems4.11/lib/thumb -r && \
#     rm /opt/rtems/4.11/arm-rtems4.11/lib/eb -r && \
#     rm -rf /var/lib/apt/lists/*

RUN echo "############### Installing QEMU ###############"
RUN apt-get update && apt-get install -y \
    qemu