#!/bin/bash

mkdir -p isodir

for arch in $(cat archs.txt); do
    mkdir -p isodir/$arch
done

if [[ $1 = "" || $2 = "" ]]; then
    echo "Usage: $0 <arch> <path to grub file>"
    exit 1
fi

# $2 = build/i686.NightOS.bin
raw_path_no_ext=${2%.*} # build/i686.NightOS
raw_path=${raw_path_no_ext//"$1."/} # build/NightOS

iso_name=${raw_path##*/} # NightOS
isodir_path=isodir/$1

mkdir -p $isodir_path/boot/grub
cp $2 $isodir_path/boot/$iso_name.bin
cp config/grub/grub.$1.cfg $isodir_path/boot/grub/grub.cfg
grub-mkrescue -o $raw_path_no_ext.iso $isodir_path
objcopy --only-keep-debug $2 $raw_path_no_ext.sym