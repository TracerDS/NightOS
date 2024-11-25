#!/bin/bash

OS_NAME="NightOS"

qemu-system-i386 -drive file=out/$OS_NAME.iso,format=raw,index=0,media=disk $* -d int -M smm=off -no-reboot -no-shutdown