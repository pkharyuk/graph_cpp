SHELL = bash
CC = g++
LIBS = 

SRC_DIR = src
BUILD_DIR = build

INCLUDE_DIRS = $(shell find $(SRC_DIR) -type d)
INCLUDE_FLAGS = $(patsubst %, -I%, $(INCLUDE_DIRS)) -I$(SRC_DIR)
#BUILD_SUBDIRS = $(patsubst $(SRC_DIR)/%, $(BUILD_DIR)/%, $(INCLUDE_DIRS))

TARGET_SRC_FILE = ./demo.cpp
#SRC_FILES = $(wildcard $(SRC_DIR)/%.cpp)
SRC_FILES = $(shell find $(SRC_DIR) -name "*.cpp")
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRC_FILES))

CXX_FLAGS = -Wall --pedantic
UT_FLAGS = -g

TARGET = demo

compile: $(TARGET)

run: $(TARGET)
	@echo "[1] Running program on random sample with n_nodes=50, density=0.4, seed=5510:"
	./$(TARGET) -s 5510 -n 50 -d 0.4 -a 1 -b 10
	@echo "[2] Running program on the sample_input.txt with seed=550:"
	./$(TARGET) -s 550 -f sample_input.txt

sample_compare: $(TARGET)
	@./$(TARGET) -s 550 -f sample_input.txt | diff ./sample_output.txt -


$(TARGET): $(OBJ_FILES) $(TARGET_SRC_FILE)
	$(CC) $(CXX_FLAGS) $(INCLUDE_FLAGS) -o $@ $^ $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CXX_FLAGS) $(INCLUDE_FLAGS) $(LIBS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@	

clean:
	rm -rf build/* $(TARGET)

.PHONY: compile run clean
