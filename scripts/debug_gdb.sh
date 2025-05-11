#!/bin/bash

#/opt/cross/bin/i686-elf-
gdb -tui -q \
    -ex 'target remote localhost:1234' \
    -ex 'set disassembly-flavor intel' \
    -ex 'set architecture i386' \
    -ex 'symbol-file build/i686.NightOS.sym'