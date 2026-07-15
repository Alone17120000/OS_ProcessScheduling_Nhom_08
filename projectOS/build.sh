#!/bin/bash

# Build script for Scheduling Simulator
# Usage: bash build.sh

echo "================================================"
echo "Building Scheduling Simulator..."
echo "================================================"

# Tạo thư mục nếu chưa tồn tại
mkdir -p obj
mkdir -p bin

# Compile main.c
echo "[1/5] Compiling main.c..."
gcc -Wall -Wextra -g -I./include -c src/main.c -o obj/main.o
if [ $? -ne 0 ]; then
    echo "❌ Failed to compile main.c"
    exit 1
fi

# Compile queue.c
echo "[2/5] Compiling queue.c..."
gcc -Wall -Wextra -g -I./include -c src/queue.c -o obj/queue.o
if [ $? -ne 0 ]; then
    echo "❌ Failed to compile queue.c"
    exit 1
fi

# Compile process.c
echo "[3/5] Compiling process.c..."
gcc -Wall -Wextra -g -I./include -c src/process.c -o obj/process.o
if [ $? -ne 0 ]; then
    echo "❌ Failed to compile process.c"
    exit 1
fi

# Compile scheduler.c
echo "[4/5] Compiling scheduler.c..."
gcc -Wall -Wextra -g -I./include -c src/scheduler.c -o obj/scheduler.o
if [ $? -ne 0 ]; then
    echo "❌ Failed to compile scheduler.c"
    exit 1
fi

# Link
echo "[5/5] Linking..."
gcc -g -o bin/scheduler obj/main.o obj/queue.o obj/process.o obj/scheduler.o
if [ $? -ne 0 ]; then
    echo "❌ Failed to link"
    exit 1
fi

echo "================================================"
echo "✅ Build completed successfully!"
echo "Executable: bin/scheduler (Linux/Mac)"
echo "           bin/scheduler.exe (Windows)"
echo "================================================"