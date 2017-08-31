CC = g++
CC_FLAGS =	-Werror -Wall -Wextra -Wfloat-equal -Winline -Wcast-qual -Wcast-align -Wconversion -Wshadow -Wswitch-default\
						-Wmissing-declarations -Wmissing-include-dirs -Wuninitialized -Wwrite-strings -Wold-style-cast -Woverloaded-virtual\
						-Wsign-promo -pedantic -std=c++14 -MD -MP

LINKER = g++

SOURCE_DIR = ./src
BIN_DIR = ./bin
OBJ_DIR = $(BIN_DIR)/obj

TARGET = $(BIN_DIR)/main
SOURCES = $(wildcard $(SOURCE_DIR)/*.cc)
OBJECTS = $(SOURCES:$(SOURCE_DIR)/%.cc=$(OBJ_DIR)/%.o)
DEPENDENCIES = $(SOURCES:$(SOURCE_DIR)/%.cc=$(OBJ_DIR)/%.d)

$(TARGET) : $(OBJECTS)
	@$(LINKER) -o $(TARGET) $(OBJECTS)

-include $(DEPENDENCIES)

$(OBJECTS): $(OBJ_DIR)/%.o : $(SOURCE_DIR)/%.cc
	@$(CC) $(CC_FLAGS) -c $< -o $@

.PHONY: run
run: $(TARGET)
	@$(TARGET)

.PHONY: clean
clean:
	@rm -f $(OBJECTS) $(TARGET)
