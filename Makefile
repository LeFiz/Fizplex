CC = g++
CC_FLAGS =	-Werror -Wall -Wextra -Wfloat-equal -Winline -Wcast-qual -Wcast-align -Wconversion -Wshadow -Wswitch-default\
						-Wmissing-declarations -Wmissing-include-dirs -Wuninitialized -Wwrite-strings -Wold-style-cast -Woverloaded-virtual\
						-Wsign-promo -pedantic -std=c++17 -MD -MP

LINKER = g++

SOURCE_DIR = ./src
BIN_DIR = ./bin
OBJ_DIR = $(BIN_DIR)/obj
TEST_LIB = ./lib/libtest.a
TEST_INCLUDE = ${GTEST_DIR}/include
TEST_FLAGS = -I$(TEST_INCLUDE) -pthread

TARGET = $(BIN_DIR)/test
SOURCES = $(wildcard $(SOURCE_DIR)/*.cc)
OBJECTS = $(SOURCES:$(SOURCE_DIR)/%.cc=$(OBJ_DIR)/%.o)
DEPENDENCIES = $(SOURCES:$(SOURCE_DIR)/%.cc=$(OBJ_DIR)/%.d)

$(TARGET) : $(OBJECTS)
	@$(LINKER) $(TEST_FLAGS) -o $(TARGET) $(OBJECTS) $(TEST_LIB)

-include $(DEPENDENCIES)

$(OBJECTS): $(OBJ_DIR)/%.o : $(SOURCE_DIR)/%.cc
	@$(CC) $(CC_FLAGS) $(TEST_FLAGS) -c $< -o $@

.PHONY: run
run: $(TARGET)
	@$(TARGET)

.PHONY: clean
clean:
	@rm -f $(OBJECTS) $(TARGET)
