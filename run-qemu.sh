#!/bin/bash

qemu-system-i386 -drive file=NightOS.iso,format=raw,index=0,media=disk $*