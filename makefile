CFLAGS = -c -o
CC = g++
SRC = ./src
BIN = ./bin
# Rules are of the format
# target: <prerequisite>
# where prerequisites are optional.
a-star: $(BIN)/utils.o $(BIN)/GameBoard.o $(BIN)/io.o $(BIN)/a-star.o
		$(CC) -o $(BIN)/finalgame.o $(BIN)/utils.o $(BIN)/GameBoard.o $(BIN)/io.o $(BIN)/a-star.o

# $< - means the first prerequisite listed
# $@ - prints the target name
$(BIN)/%.o: $(SRC)/%.cpp
		$(CC) $(CFLAGS) $@ $<
clean: 
		rm -f pengu $(BIN)/*.o