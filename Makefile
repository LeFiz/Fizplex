CC = g++
GENERAL_CC_FLAGS =	-Werror -Wall -Wextra -Wfloat-equal\
										-Wcast-qual -Wcast-align -Wconversion -Wshadow -Wswitch-default\
										-Wmissing-declarations -Wmissing-include-dirs -Wuninitialized\
										-Wwrite-strings -Wold-style-cast -Woverloaded-virtual\
										-Wsign-promo -pedantic -std=c++17 -MD -MP
TEST_CC_FLAGS = -O0 -ggdb

LINKER = g++

TEST_LIB = ./lib/libtest.a
TEST_INCLUDE = ${GTEST_DIR}/include
TEST_FLAGS = -I$(TEST_INCLUDE) -pthread

CC_FLAGS = $(GENERAL_CC_FLAGS) $(TEST_CC_FLAGS) $(TEST_FLAGS)

SOURCE_DIR = ./src
BIN_DIR = ./bin
OBJ_DIR = $(BIN_DIR)/obj
TEST_TARGET = $(BIN_DIR)/test
TARGET = $(TEST_TARGET)

SOURCES = $(wildcard $(SOURCE_DIR)/*.cc)
OBJECTS = $(SOURCES:$(SOURCE_DIR)/%.cc=$(OBJ_DIR)/%.o)
DEPENDENCIES = $(SOURCES:$(SOURCE_DIR)/%.cc=$(OBJ_DIR)/%.d)

$(TARGET) : $(OBJECTS)
	@$(LINKER) $(TEST_FLAGS) -o $(TARGET) $(OBJECTS) $(TEST_LIB)

-include $(DEPENDENCIES)

$(OBJECTS): $(OBJ_DIR)/%.o : $(SOURCE_DIR)/%.cc
	@$(CC) $(CC_FLAGS) -c $< -o $@

.PHONY: run
run: $(TARGET)
	@$(TARGET)

.PHONY: clean
clean:
	@rm -f $(OBJECTS) $(BUILD_TARGET) $(TEST_TARGET)
