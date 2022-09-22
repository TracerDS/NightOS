@echo off

call build_x86.bat
if %errorlevel% NEQ 0 (
    echo [91m[91mCompilation not successful![0m
    exit /b
)
call run_x86.bat