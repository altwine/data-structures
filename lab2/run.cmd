@echo off

:: Build & check for errors
g++ -O3 -march=native -flto -std=c++11 -m64 -IC:\OpenBLAS\include ".\lab2.cxx" -LC:\OpenBLAS\lib -lopenblas -o ".\lab2.exe"
if %errorlevel% neq 0 ( exit /b )

:: Run & check for errors
".\lab2.exe"
if %errorlevel% neq 0 ( exit /b )

:: Remove tmp executable
del ".\lab2.exe"