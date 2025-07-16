#!/bin/bash
set -e

BUILD_DIR="build"

if [ ! -d "$BUILD_DIR" ]; then
    echo "Creating build directory..."
    mkdir "$BUILD_DIR"
else
    echo "Using existing build directory..."
fi

echo "Running CMake configuration..."
cmake -S . -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release

echo "Building project..."
cmake --build "$BUILD_DIR" -- -j$(nproc)

