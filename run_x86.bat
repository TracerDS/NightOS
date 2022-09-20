@echo off
cls
echo.
echo %cd%^>%0

qemu-system-i386 -fda bin\os.bin