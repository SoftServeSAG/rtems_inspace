#!/usr/bin/env bash

xhost +local:root

docker run -it \
    --runtime=nvidia \
    --net=host \
    --env="DISPLAY"  \
    --env="QT_X11_NO_MITSHM=1"  \
    --volume="/tmp/.X11-unix:/tmp/.X11-unix:rw" \
    --volume="$XAUTH:$XAUTH" \
   rtems-rpi

xhost -local:root
