#!/bin/bash

gdb -q -ex 'target remote localhost:1234' -ex 'symbol-file kernel.sym'
