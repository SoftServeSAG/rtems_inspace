#!/usr/bin/env bash

SCRIPT_PATH=$(readlink -f "$0")
SCRIPT_DIR_PATH=$(dirname "$SCRIPT_PATH")
WS_DIR_PATH="$SCRIPT_DIR_PATH/../.."

xhost +local:root

docker run -it \
    --runtime=nvidia \
    --privileged \
    --net=host \
    --env="DISPLAY"  \
    --env="QT_X11_NO_MITSHM=1"  \
    --volume="/tmp/.X11-unix:/tmp/.X11-unix:rw" \
    --volume="$XAUTH:$XAUTH" \
    --volume="$WS_DIR_PATH:/root/dev/ws" \
   rtems-i386-freebsd

xhost -local:root
