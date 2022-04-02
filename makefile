CFLAGS = -c -o
CC = mpic++
SRC = ./src
BIN = ./bin
OBJ = $(BIN)/utils.o \
	  $(BIN)/GameBoard.o \
	  $(BIN)/io.o \
	  $(BIN)/search_algorithms.o \
	  $(BIN)/search_algorithms_utils.o \
	  $(BIN)/main.o 
# Rules are of the format
# target: <prerequisite>
# where prerequisites are optional.
main: $(OBJ)
		$(CC) -o $(BIN)/finalgame.o $(OBJ)

# $< - means the first prerequisite listed
# $@ - prints the target name
$(BIN)/%.o: $(SRC)/%.cpp
		$(CC) $(CFLAGS) $@ $<
clean: 
		rm -f pengu $(BIN)/*.o