@echo off
cls
echo.
echo %cd%^>%0

qemu-system-i386 bootloader\boot_x86.bin