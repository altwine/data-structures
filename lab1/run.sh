#!/bin/bash
set -e

# Build
g++ "./lab1.c" -o "./lab1"

# Run
"./lab1"

# Remove tmp executable
rm "./lab1"