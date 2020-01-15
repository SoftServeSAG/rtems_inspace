#!/usr/bin/env bash

if [ "$1" == "" ]; then
    echo "First arg should be path to kernel"
    exit 1
fi

SCRIPT_PATH=$(readlink -f "$0")
SCRIPT_DIR_PATH=$(dirname "$SCRIPT_PATH")
KERNEL_PATH="$1"

qemu-system-arm \
  -serial null \
  -serial mon:stdio \
  -nographic \
  -M xilinx-zynq-a9 \
  -m 256M \
  -net tap,ifname=qtap,script=no,downscript=no \
  -net nic,model=cadence_gem,macaddr=0e:b0:ba:5e:ba:12 \
  -kernel "$KERNEL_PATH"