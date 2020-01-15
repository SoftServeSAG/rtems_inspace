#!/usr/bin/env bash

if [ "$1" == "" ]; then
    echo "First arg should be path to kernel"
    exit 1
fi

SCRIPT_PATH=$(readlink -f "$0")
SCRIPT_DIR_PATH=$(dirname "$SCRIPT_PATH")
KERNEL_PATH="$1"

qemu-system-arm \
  -no-reboot \
  -serial null \
  -serial mon:stdio \
  -net none \
  -nographic \
  -M xilinx-zynq-a9 \
  -m 256M \
  -kernel "$KERNEL_PATH"