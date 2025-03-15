#!/bin/bash
set -e

# Build
gcc "./lab1.c" -o "./lab1"

# Run
"./lab1"

# Remove tmp executable
rm "./lab1"