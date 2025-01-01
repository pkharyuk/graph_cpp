SHELL = bash
CC = g++
LIBS = 

SRC_DIR = src
BUILD_DIR = build

GRAPH_DEMO_DIR = demo
GAME_DEMO_DIR = game

EXCLUDE_DIRS = $(SRC_DIR)/$(GRAPH_DEMO_DIR) $(SRC_DIR)/$(GAME_DEMO_DIR)

INCLUDE_DIRS = $(shell find $(SRC_DIR) -type d $(foreach subdir, $(EXCLUDE_DIRS) $(SRC_DIR), -not -path "$(subdir)"))
INCLUDE_FLAGS = $(patsubst %, -I%, $(INCLUDE_DIRS))

TARGET_GRAPH_SRC_FILE = demo_graph.cpp
TARGET_GAME_SRC_FILE = demo_hex_game.cpp
#SRC_FILES = $(wildcard $(SRC_DIR)/%.cpp)
COMMON_SRC_FILES = $(shell find $(SRC_DIR) -name "*.cpp" $(foreach subdir, $(EXCLUDE_DIRS), -not -path "$(subdir)/*"))
COMMON_OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(COMMON_SRC_FILES))

GRAPH_DEMO_SRC_FILES = $(shell find $(SRC_DIR)/$(GRAPH_DEMO_DIR) -name "*.cpp")
GRAPH_DEMO_OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(GRAPH_DEMO_SRC_FILES))

GAME_DEMO_SRC_FILES = $(shell find $(SRC_DIR)/$(GAME_DEMO_DIR) -name "*.cpp")
GAME_DEMO_OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(GAME_DEMO_SRC_FILES))

CXX_FLAGS = -Wall --pedantic
UT_FLAGS = -g

TARGET_GRAPH = demo_graph
TARGET_GAME = demo_hex_game

compile: $(TARGET_GRAPH) $(TARGET_GAME)

run_demo_graph: $(TARGET_GRAPH)
	@echo "[1] Running program on random sample with n_nodes=50, density=0.4, seed=5510:"
	./$(TARGET_GRAPH) -s 5510 -n 50 -d 0.4 -a 1 -b 10
	@echo "[2] Running program on the sample_input.txt with seed=550:"
	./$(TARGET_GRAPH) -s 550 -f sample_input.txt

run_hex_game: $(TARGET_GAME)
	@echo "Running the game with size=11, seed=550, maxitnum=1000,"
	@echo "color=R, bot_strategy=random (switching MC lookahead/shortest pah completion):"
	./$(TARGET_GAME) -s 550 -m 1000 -p R -n 11 -b 2

sample_compare: $(TARGET_GRAPH)
	@./$(TARGET_GRAPH) -s 550 -f sample_input.txt | diff ./sample_output.txt -


$(TARGET_GRAPH): $(COMMON_OBJ_FILES) $(GRAPH_DEMO_OBJ_FILES)
	$(CC) $(CXX_FLAGS) $(INCLUDE_FLAGS) -I$(SRC_DIR)/$(GRAPH_DEMO_DIR) -o $@ $^ $(LIBS)

$(TARGET_GAME): $(COMMON_OBJ_FILES) $(GAME_DEMO_OBJ_FILES)
	$(CC) $(CXX_FLAGS) $(INCLUDE_FLAGS) -I$(SRC_DIR)/$(GAME_DEMO_DIR)  -o $@ $^ $(LIBS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CXX_FLAGS) $(INCLUDE_FLAGS) $(LIBS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@	

clean:
	rm -rf build/* $(TARGET_GRAPH) $(TARGET_GAME)

.PHONY: compile clean
