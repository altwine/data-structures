#!/bin/bash
set -e

g++ "./lab1.c" -o "./lab1.exe"
"./lab1.exe"

rm "./lab1.exe"