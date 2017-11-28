CC = g++
LINKER = g++

GENERAL_CC_FLAGS =	-Werror -Wall -Wextra -Wfloat-equal\
										-Wcast-qual -Wcast-align -Wconversion -Wshadow -Wswitch-default\
										-Wmissing-declarations -Wmissing-include-dirs -Wuninitialized\
										-Wwrite-strings -Wold-style-cast -Woverloaded-virtual\
										-Wsign-promo -pedantic -std=c++17 -MD -MP -O2
TEST_CC_FLAGS = -I$(TEST_INCLUDE) -pthread -ggdb
TEST_LINKER_FLAGS = -I$(TEST_INCLUDE) -pthread

TEST_LIB = ./lib/libtest.a
TEST_INCLUDE = ${GTEST_DIR}/include

SOURCE_DIR = ./src
BIN_DIR = ./bin
OBJ_DIR = $(BIN_DIR)/obj

ALL_SOURCES = $(wildcard $(SOURCE_DIR)/*.cc)
TEST_ONLY_SOURCES = $(wildcard $(SOURCE_DIR)/*test.cc)
TEST_SOURCES = $(filter-out $(RELEASE_ONLY_SOURCES), $(ALL_SOURCES))
RELEASE_ONLY_SOURCES = $(wildcard $(SOURCE_DIR)/*release.cc)
RELEASE_SOURCES = $(filter-out $(TEST_ONLY_SOURCES), $(ALL_SOURCES))

TEST_ONLY_OBJECTS = $(TEST_ONLY_SOURCES:$(SOURCE_DIR)/%.cc=$(OBJ_DIR)/%.o)
TEST_OBJECTS = $(TEST_SOURCES:$(SOURCE_DIR)/%.cc=$(OBJ_DIR)/%.o)
RELEASE_OBJECTS = $(RELEASE_SOURCES:$(SOURCE_DIR)/%.cc=$(OBJ_DIR)/%.o)
DEPENDENCIES = $(ALL_SOURCES:$(SOURCE_DIR)/%.cc=$(OBJ_DIR)/%.d)

TEST_TARGET = $(BIN_DIR)/test
RELEASE_TARGET = $(BIN_DIR)/fizplex

$(RELEASE_TARGET) : $(RELEASE_OBJECTS)
	@$(LINKER) -o $(RELEASE_TARGET) $(RELEASE_OBJECTS)

$(RELEASE_OBJECTS): $(OBJ_DIR)/%.o : $(SOURCE_DIR)/%.cc
	@$(CC) $(GENERAL_CC_FLAGS) -c $< -o $@

.PHONY: run
run: $(RELEASE_TARGET)
	@$(RELEASE_TARGET)

#
# Test builds
#

$(TEST_TARGET) : $(TEST_OBJECTS)
	@$(LINKER) $(TEST_LINKER_FLAGS) -o $(TEST_TARGET) $(TEST_OBJECTS) $(TEST_LIB)

-include $(DEPENDENCIES)

$(TEST_ONLY_OBJECTS): $(OBJ_DIR)/%.o : $(SOURCE_DIR)/%.cc
	@$(CC) $(TEST_CC_FLAGS) $(GENERAL_CC_FLAGS) -c $< -o $@

test: $(TEST_TARGET)
	@$(TEST_TARGET)

.PHONY: clean
clean:
	@rm -f $(TEST_TARGET) $(RELEASE_TARGET) $(TEST_OBJECTS) $(RELEASE_OBJECTS) $(DEPENDENCIES)
