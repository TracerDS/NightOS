@echo off
cls
echo.
echo %cd%^>%0

qemu-system-x86_64 bin\os.bin