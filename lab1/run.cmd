@echo off

:: Build & check for errors
g++ ".\lab1.c" -o ".\lab1.exe"
if %errorlevel% neq 0 ( exit /b )

:: Run & check for errors
".\lab1.exe"
if %errorlevel% neq 0 ( exit /b )

:: Remove tmp executable
del ".\lab1.exe"