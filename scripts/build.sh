#!/usr/bin/env bash

SCRIPT_PATH=$(readlink -f "$0")
SCRIPT_DIR_PATH=$(dirname "$SCRIPT_PATH")

$SCRIPT_DIR_PATH/waf configure \
    --rtems=/opt/rtems/4.11 \
    --rtems-tools=/opt/rtems/4.11 \
    --rtems-archs=i386

$SCRIPT_DIR_PATH/waf