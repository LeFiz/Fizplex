CC = g++
CC_FLAGS = -Wall -Wextra -pedantic

LINKER = g++

SOURCE_DIR = ./src
BIN_DIR = ./bin
OBJ_DIR = $(BIN_DIR)/obj

TARGET = $(BIN_DIR)/main
SOURCES = $(wildcard $(SOURCE_DIR)/*.cc)
OBJECTS = $(SOURCES:$(SOURCE_DIR)/%.cc=$(OBJ_DIR)/%.o)

$(TARGET) : $(OBJECTS)
	@$(LINKER) -o $(TARGET) $(OBJECTS)

$(OBJECTS): $(OBJ_DIR)/%.o : $(SOURCE_DIR)/%.cc
	@$(CC) $(CC_FLAGS) -c $< -o $@

.PHONY: run
run: $(TARGET)
	@$(TARGET)

.PHONY: clean
clean:
	@rm -f $(OBJECTS) $(TARGET)
