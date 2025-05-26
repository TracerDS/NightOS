#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: $0 <target>"
    exit 1
fi

script_path=$(dirname "$0")

qemu-system-i386 -vga virtio -cdrom $* \
    -D "$script_path/../logs/qemu.log" \
    -d int,cpu_reset \
    -monitor pty \