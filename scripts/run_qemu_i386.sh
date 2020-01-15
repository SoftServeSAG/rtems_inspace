#!/usr/bin/env bash

if [ "$1" == "" ]; then
    echo "First arg should be path to kernel"
    exit 1
fi

SCRIPT_PATH=$(readlink -f "$0")
SCRIPT_DIR_PATH=$(dirname "$SCRIPT_PATH")
KERNEL_PATH="$1"

qemu-system-x86_64 \
      -enable-kvm \
      -vga cirrus \
      -net nic,vlan=1,macaddr=00:04:9F:00:27:50,model=i82557b \
      -net tap,ifname=tap0,vlan=1,script=no,downscript=no  \
      -append "--console=/dev/com1" \
      -serial stdio \
      -kernel "$KERNEL_PATH"