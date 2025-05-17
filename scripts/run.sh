#!/bin/bash

if [ -z "$1" ]; then
    echo "Usage: $0 <target>"
    exit 1
fi

qemu-system-i386 -vga virtio -cdrom $*