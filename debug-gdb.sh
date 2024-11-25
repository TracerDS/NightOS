#!/bin/bash

gdb -tui -q -ex 'target remote localhost:1234' -ex 'symbol-file out/kernel.sym'