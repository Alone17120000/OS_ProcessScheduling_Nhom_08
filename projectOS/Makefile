CC = gcc
CFLAGS = -Wall -Wextra -g
INCLUDE_DIR = include
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# Tên executable (Windows)
EXECUTABLE = $(BIN_DIR)/scheduler.exe

# Files
SOURCES = $(SRC_DIR)/main.c $(SRC_DIR)/queue.c $(SRC_DIR)/process.c $(SRC_DIR)/scheduler.c
OBJECTS = $(OBJ_DIR)/main.o $(OBJ_DIR)/queue.o $(OBJ_DIR)/process.o $(OBJ_DIR)/scheduler.o

# Phony targets
.PHONY: all clean run test help

# Default target
all: $(EXECUTABLE)

# Create directories if not exist
$(OBJ_DIR):
	@mkdir -p $(OBJ_DIR)

$(BIN_DIR):
	@mkdir -p $(BIN_DIR)

# Compile source files
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $(SRC_DIR)/main.c -o $@

$(OBJ_DIR)/queue.o: $(SRC_DIR)/queue.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $(SRC_DIR)/queue.c -o $@

$(OBJ_DIR)/process.o: $(SRC_DIR)/process.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $(SRC_DIR)/process.c -o $@

$(OBJ_DIR)/scheduler.o: $(SRC_DIR)/scheduler.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $(SRC_DIR)/scheduler.c -o $@

# Link
$(EXECUTABLE): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^
	@echo ""
	@echo "========================================"
	@echo "Build completed successfully!"
	@echo "Executable: $(EXECUTABLE)"
	@echo "========================================"

# Test
test: all
	@echo ""
	@echo "========== Testing FCFS Algorithm =========="
	./$(EXECUTABLE) test/input.txt output_fcfs.txt 1
	@echo "Output:"
	@cat output_fcfs.txt
	@echo ""
	@echo "========== Testing RR Algorithm (quantum=2) =========="
	./$(EXECUTABLE) test/input.txt output_rr_q2.txt 2
	@echo "Output:"
	@cat output_rr_q2.txt
	@echo ""
	@echo "========== Testing RR Algorithm (quantum=3) =========="
	./$(EXECUTABLE) test/input.txt output_rr_q3.txt 2 3
	@echo "Output:"
	@cat output_rr_q3.txt
	@echo ""
	@echo "========================================"
	@echo "All tests completed!"
	@echo "========================================"

# Clean
clean:
	@echo "Cleaning up..."
	@rm -rf $(OBJ_DIR) $(BIN_DIR) output_fcfs.txt output_rr_q2.txt output_rr_q3.txt
	@echo "Cleaned up object files and executables"

# Help
help:
	@echo ""
	@echo "Makefile for Scheduling Simulator"
	@echo "==================================="
	@echo "make        - Compile the project"
	@echo "make test   - Compile and run all tests"
	@echo "make clean  - Remove compiled files"
	@echo "make help   - Show this help message"
	@echo ""
