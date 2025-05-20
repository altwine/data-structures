@echo off

:: Build & check for errors
g++ ".\lab3.cxx" -o ".\lab3.exe"
if %errorlevel% neq 0 ( exit /b )

:: Run & check for errors
".\lab3.exe"
if %errorlevel% neq 0 ( exit /b )

:: Remove tmp executable
del ".\lab3.exe"