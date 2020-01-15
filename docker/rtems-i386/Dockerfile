FROM ubuntu:16.04

ENV DEBIAN_FRONTEND noninteractive
ENV TERM linux
ENV PATH="/opt/rtems/5/bin:${PATH}"
ENV RTEMS_MAKEFILE_PATH=/opt/rtems/5/i386-rtems5/i386

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
    ../source-builder/sb-set-builder --prefix=/opt/rtems/5 5/rtems-i386

RUN echo "############### Builing kernel ###############"
RUN cd $HOME/dev && \
    git clone https://github.com/RTEMS/rtems.git && \
    cd rtems && \
    ./bootstrap -c && \
#    ./bootstrap -p && \
    ../rtems-source-builder/source-builder/sb-bootstrap

RUN cd $HOME/dev && mkdir rtems-i386 && cd rtems-i386 && ../rtems/configure \
    --prefix=/opt/rtems/5 \
    --target=i386-rtems5 \
    --enable-rtemsbsp=pc386 \
    --enable-tests=samples \
    --enable-posix \
    --enable-cxx && \
    # --enable-networking \
    make -j8 && \
    make install


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