#!/bin/bash

OS_NAME="NightOS"

objcopy --only-keep-debug out/$OS_NAME.bin out/kernel.sym
#objcopy --strip-debug $OS_NAME.bin

./make-isodir.sh

./run-qemu.sh -s -S -D log-qemu.txt -d int