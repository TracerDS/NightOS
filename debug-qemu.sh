#!/bin/bash

objcopy --only-keep-debug NightOS.bin kernel.sym
#objcopy --strip-debug NightOS.bin
cp NightOS.bin isodir/boot/NightOS.bin
grub-mkrescue -o NightOS.iso isodir

./run-qemu.sh -s -S