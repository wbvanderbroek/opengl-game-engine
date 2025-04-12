#!/bin/bash

set -e

echo "[INFO] Creating build folder (Release)..."
mkdir -p Build

cd Build

echo "[INFO] Running CMake configure (Release mode)..."
cmake -DCMAKE_BUILD_TYPE=Release ..

if [ $? -ne 0 ]; then
    echo "[ERROR] CMake configuration failed."
    exit $?
fi

echo "[INFO] Building project (Release)..."
cmake --build . --config Release

if [ $? -ne 0 ]; then
    echo "[ERROR] Build failed."
    exit $?
fi

echo "[INFO] Release build completed successfully."
