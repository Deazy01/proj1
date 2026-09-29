CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -O0 -Iinclude
LDFLAGS =
LDLIBS = -lgtest -lgtest_main -pthread

SRC_DIR = src
TEST_SRC_DIR = tests/src

BIN_DIR = bin
OBJ_DIR = obj
TEST_BIN_DIR = tests/bin
TEST_OBJ_DIR = tests/obj
TESTCOVER_DIR = tests/htmlcov

COVERAGE_INFO_FILE = coverage.info

DEQUE_SRC = $(SRC_DIR)/MaxSizeDeque.cpp $(SRC_DIR)/VariableSizeDeque.cpp
DEQUE_OBJ = $(OBJ_DIR)/MaxSizeDeque.o $(OBJ_DIR)/VariableSizeDeque.o

TEST_SRC = $(TEST_SRC_DIR)/DequeTest.cpp
TEST_OBJ = $(TEST_OBJ_DIR)/DequeTest.o

TEST_PROGRAM = $(TEST_BIN_DIR)/testdeque
ANALYSIS_PROGRAM = $(BIN_DIR)/analysis

.PHONY: all clean test coverage analysis

all: test

$(BIN_DIR) $(OBJ_DIR) $(TEST_BIN_DIR) $(TEST_OBJ_DIR) $(TESTCOVER_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_OBJ_DIR)/%.o: $(TEST_SRC_DIR)/%.cpp | $(TEST_OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_PROGRAM): $(DEQUE_OBJ) $(TEST_OBJ) | $(TEST_BIN_DIR)
	$(CXX) $(DEQUE_OBJ) $(TEST_OBJ) $(LDFLAGS) $(LDLIBS) -o $@

$(ANALYSIS_PROGRAM): $(DEQUE_OBJ) $(SRC_DIR)/analysis.cpp | $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(DEQUE_OBJ) $(SRC_DIR)/analysis.cpp -o $@

test: $(TEST_PROGRAM)
	./$(TEST_PROGRAM)

coverage: $(TEST_PROGRAM)
	rm -rf $(TESTCOVER_DIR)
	mkdir -p $(TESTCOVER_DIR)
	lcov --capture --directory `pwd` --output-file $(COVERAGE_INFO_FILE) --ignore-errors inconsistent,source 2> lcov.err && grep -v "mismatched end line" lcov.err >&2 && rm -f lcov.err
	lcov --remove $(COVERAGE_INFO_FILE) '/usr/*' '*/tests/src/*' '*/*.h' --output-file $(COVERAGE_INFO_FILE)
	genhtml $(COVERAGE_INFO_FILE) --output-directory $(TESTCOVER_DIR)

analysis: $(ANALYSIS_PROGRAM)
	valgrind --leak-check=full --errors-for-leak-kinds=definite,indirect --error-exitcode=1 ./$(ANALYSIS_PROGRAM) --max 256 --iter 1024
	valgrind --leak-check=full --errors-for-leak-kinds=definite,indirect --error-exitcode=1 ./$(ANALYSIS_PROGRAM) --max 256 --iter 2048
	valgrind --leak-check=full --errors-for-leak-kinds=definite,indirect --error-exitcode=1 ./$(ANALYSIS_PROGRAM) --max 256 --iter 4096
	valgrind --leak-check=full --errors-for-leak-kinds=definite,indirect --error-exitcode=1 ./$(ANALYSIS_PROGRAM) --max 256 --iter 8192
	./$(ANALYSIS_PROGRAM) --max 256 --iter 1024
	./$(ANALYSIS_PROGRAM) --max 256 --iter 2048
	./$(ANALYSIS_PROGRAM) --max 256 --iter 4096
	./$(ANALYSIS_PROGRAM) --max 256 --iter 8192

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR) $(TEST_BIN_DIR) $(TEST_OBJ_DIR) $(TESTCOVER_DIR)
	rm -f $(COVERAGE_INFO_FILE) lcov.err results.csv
