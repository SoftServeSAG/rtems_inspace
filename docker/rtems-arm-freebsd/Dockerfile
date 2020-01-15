FROM ubuntu:16.04

ENV DEBIAN_FRONTEND noninteractive
ENV TERM linux
ENV PATH="/opt/rtems/5/bin:${PATH}"
ENV RTEMS_MAKEFILE_PATH=/opt/rtems/5/arm-rtems5/arm

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
    qemu \
    vim \
    ca-certificates \
    libtinfo-dev && \
    ln -T /usr/bin/python2.7 /usr/bin/python

RUN echo "############### Building crosscompiler ###############"
RUN mkdir -p $HOME/dev && \
    cd $HOME/dev && \
    git clone https://github.com/RTEMS/rtems-source-builder.git && \
    cd rtems-source-builder && \
    ./source-builder/sb-check &&\
    cd rtems && \
    ../source-builder/sb-set-builder --prefix=/opt/rtems/5 5/rtems-arm

RUN echo "############### Building kernel ###############"
RUN cd $HOME/dev && \
    git clone https://github.com/RTEMS/rtems.git && \
    cd rtems && \
    ./bootstrap -c && \
#    ./bootstrap -p && \
    ../rtems-source-builder/source-builder/sb-bootstrap

RUN cd $HOME/dev && mkdir rtems-arm && cd rtems-arm && ../rtems/configure \
    --prefix=/opt/rtems/5 \
    --target=arm-rtems5 \
    --enable-rtemsbsp=xilinx_zynq_a9_qemu \
    --enable-tests=samples \
    --enable-posix \
    --enable-cxx \
    --disable-networking && \
    make -j8 && \
    make install

RUN echo "############### Building RTEMS-libbsd ###############"
RUN cd $HOME/dev && \
    git clone https://github.com/RTEMS/rtems-libbsd.git && \
    cd rtems-libbsd && \
    git submodule init && \
    git submodule update rtems_waf
RUN cd $HOME/dev/rtems-libbsd && \
    ./waf configure \
        --prefix="/opt/rtems/5" \
        --rtems-bsps=arm/xilinx_zynq_a9_qemu \
        --buildset=buildset/default.ini && \
    ./waf && \
    ./waf install

RUN echo "############### Building RTEMS Tools ###############"
RUN mkdir -p $HOME/dev && \
    cd $HOME/dev && \
    git clone https://github.com/RTEMS/rtems-tools.git
RUN cd $HOME/dev/rtems-tools && \
    ./waf configure \
        --prefix=/opt/rtems/5 && \
    ./waf && \
    ./waf install

RUN echo "############### Building RTEMS Examples ###############"
RUN mkdir -p $HOME/dev && \
    cd $HOME/dev && \
    git clone https://github.com/RTEMS/rtems-examples.git && \
    cd rtems-examples && \
    git submodule init && \
    git submodule update
RUN cd $HOME/dev/rtems-examples && \
    ./waf configure \
        --prefix=/opt/rtems/5 && \
    ./waf && \
    ./waf install

RUN apt-get install -y \
    iproute2 \
    uml-utilities \
    bridge-utils \
    tmux