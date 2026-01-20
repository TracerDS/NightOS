#!/bin/bash

target=$1

if [ -z "$target" ]; then
    echo "Usage: $0 <target>"
    exit 1
fi

SCRIPT_DIR=$(dirname "$0")

if [ -f "$SCRIPT_DIR/../archs/$target/build/$target.iso" ]; then
    target="$SCRIPT_DIR/../archs/$target/build/$target.iso"
fi

args=${@:2}

if [[ ! -d "$SCRIPT_DIR/../logs" ]]; then
	mkdir "$SCRIPT_DIR/../logs"
fi

echo "" > "$SCRIPT_DIR/../logs/qemu.log"

qemu-system-i386 \
    $args \
    -m 4G \
    -vga virtio \
    -cdrom $target \
    -D "$SCRIPT_DIR/../logs/qemu.log" \
    -d int,cpu_reset \
    -monitor stdio \
    -serial file:"$SCRIPT_DIR/../logs/com1_serial.log" \
    -serial file:"$SCRIPT_DIR/../logs/com2_serial.log" \
