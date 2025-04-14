#!/bin/bash

set -e

echo "[INFO] Creating build folder (Debug)..."
mkdir -p Build

cd Build

echo "[INFO] Running CMake configure (Debug mode)..."
cmake -DCMAKE_BUILD_TYPE=Debug ..

if [ $? -ne 0 ]; then
    echo "[ERROR] CMake configuration failed."
    exit $?
fi

echo "[INFO] Building project (Debug)..."
cmake --build . --config Debug

if [ $? -ne 0 ]; then
    echo "[ERROR] Build failed."
    exit $?
fi

echo "[INFO] Debug build completed successfully."
