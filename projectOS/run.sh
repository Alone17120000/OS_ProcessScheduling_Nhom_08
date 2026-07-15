#!/bin/bash

# Run script for Scheduling Simulator
# Usage: bash run.sh

echo "================================================"
echo "Running Scheduling Simulator Tests"
echo "================================================"

if [ ! -f bin/scheduler ]; then
    if [ ! -f bin/scheduler.exe ]; then
        echo "❌ Executable not found. Please run build.sh first"
        exit 1
    fi
    EXECUTABLE="bin/scheduler.exe"
else
    EXECUTABLE="bin/scheduler"
fi

# Test FCFS
echo ""
echo "=== Testing FCFS Algorithm ==="
$EXECUTABLE test/input.txt test/output_fcfs.txt 1
echo ""
echo "Output:"
cat test/output_fcfs.txt

# Test RR with default quantum=2
echo ""
echo "=== Testing RR Algorithm (quantum=2) ==="
$EXECUTABLE test/input.txt test/output_rr_q2.txt 2
echo ""
echo "Output:"
cat test/output_rr_q2.txt

# Test RR with quantum=3
echo ""
echo "=== Testing RR Algorithm (quantum=3) ==="
$EXECUTABLE test/input.txt test/output_rr_q3.txt 2 3
echo ""
echo "Output:"
cat test/output_rr_q3.txt

echo ""
echo "================================================"
echo "✅ All tests completed!"
echo "================================================"