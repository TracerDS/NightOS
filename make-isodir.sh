#!/bin/bash

OS_NAME="NightOS"

if ! grub-file --is-x86-multiboot out/$OS_NAME.bin; then
    echo "OS is not multiboot!"
    exit
fi

mkdir -p isodir/boot/grub
mkdir -p out
cp out/$OS_NAME.bin isodir/boot/$OS_NAME.bin

if [ ! -f isodir/boot/grub/grub.cfg ]; then
    echo "menuentry \"$OS_NAME\" {" > isodir/boot/grub/grub.cfg
    echo -e "\tmultiboot /boot/$OS_NAME.bin" >> isodir/boot/grub/grub.cfg
    echo '}' >> isodir/boot/grub/grub.cfg
fi
grub-mkrescue -o out/$OS_NAME.iso isodir