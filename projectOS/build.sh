#!/bin/bash

# Build script for CPU & Resource Scheduling Simulator
# Usage: ./build.sh OR bash build.sh

echo "================================================"
echo "Building Scheduling Simulator (Hybrid Version)..."
echo "================================================"

# Tạo thư mục nếu chưa tồn tại
mkdir -p obj
mkdir -p bin

# Danh sách các file module lõi cần biên dịch
FILES=("main" "utils" "sjf" "srtn" "fcfs" "rr")
TOTAL_FILES=${#FILES[@]}
CURRENT_STEP=1

# Biên dịch từng file .c thành .o
for file in "${FILES[@]}"; do
    echo "[$CURRENT_STEP/$(($TOTAL_FILES + 1))] Compiling $file.c..."
    gcc -Wall -Wextra -g -I./include -c src/$file.c -o obj/$file.o
    
    if [ $? -ne 0 ]; then
        echo "❌ Failed to compile $file.c"
        exit 1
    fi
    ((CURRENT_STEP++))
done

# Liên kết (Link) tất cả các file .o lại thành file chạy
echo "[$CURRENT_STEP/$(($TOTAL_FILES + 1))] Linking objects..."
gcc -g -o bin/scheduler obj/main.o obj/utils.o obj/sjf.o obj/srtn.o obj/fcfs.o obj/rr.o

if [ $? -ne 0 ]; then
    echo "❌ Failed to link objects"
    exit 1
fi

echo "================================================"
echo "✅ Build completed successfully!"
echo "🚀 Executable: bin/scheduler (Linux/Mac)"
echo "             bin/scheduler.exe (Windows)"
echo "================================================"