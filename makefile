CFLAGS = -c -o
CC = mpic++
SRC = ./src
BIN = ./bin
# Rules are of the format
# target: <prerequisite>
# where prerequisites are optional.
main: $(BIN)/utils.o $(BIN)/GameBoard.o $(BIN)/io.o $(BIN)/main.o $(BIN)/search_algorithms.o
		$(CC) -o $(BIN)/finalgame.o $(BIN)/utils.o $(BIN)/GameBoard.o $(BIN)/io.o $(BIN)/search_algorithms.o $(BIN)/main.o

# $< - means the first prerequisite listed
# $@ - prints the target name
$(BIN)/%.o: $(SRC)/%.cpp
		$(CC) $(CFLAGS) $@ $<
clean: 
		rm -f pengu $(BIN)/*.o