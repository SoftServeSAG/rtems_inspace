#!/usr/bin/env bash

ip tuntap add tap0 mode tap user $(whoami)
ip link set tap0 up
ip addr add 10.0.0.1/24 dev tap0