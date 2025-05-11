#!/bin/bash

objcopy --only-keep-debug build/i686.NightOS.bin build/i686.NightOS.sym
objcopy --strip-debug build/i686.NightOS.bin