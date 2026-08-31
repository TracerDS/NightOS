#!/bin/bash

target=$1

if [ -z "$target" ]; then
    echo "Usage: $0 <target>"
    exit 1
fi

SCRIPT_DIR=$(dirname "$0")
BUILD_DIR="$SCRIPT_DIR/../archs/$target/build"
RAW_NAME="${target%.*}"
DISK_PATH="$RAW_NAME"_disk.img

if [ -f "$BUILD_DIR/$target.iso" ]; then
    target="$BUILD_DIR/$target.iso"
    DISK_PATH="$BUILD_DIR/$RAW_NAME"_disk.img
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
    -drive file=$target,format=raw,media=disk,index=0 \
    -drive file=$DISK_PATH,format=raw,media=disk,index=1 \
    -D "$SCRIPT_DIR/../logs/qemu.log" \
    -d int,cpu_reset \
    -monitor stdio \
    -serial file:"$SCRIPT_DIR/../logs/com1_serial.log" \
    -serial file:"$SCRIPT_DIR/../logs/com2_serial.log" \
