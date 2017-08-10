CC = g++

LINKER = g++

SOURCE_DIR = ./src

main : main.o svector.o
	@$(LINKER) -o main main.o svector.o

main.o: $(SOURCE_DIR)/main.cc
	@$(CC) -c $(SOURCE_DIR)/main.cc
svector.o : $(SOURCE_DIR)/svector.cc
	@$(CC) -c $(SOURCE_DIR)/svector.cc


.PHONY: clean
clean:
	@rm -f *.o main
