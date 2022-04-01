CFLAGS = -c -o
CC = mpic++
SRC = ./src
BIN = ./bin
SOURCE = \
		$(SRC)/GameBoard.cpp\
		$(SRC)/io.cpp\
		$(SRC)/main.cpp\
		$(SRC)/parallel_search_algorithms.cpp\
		$(SRC)/search_algorithms.cpp\
		$(SRC)/solution.cpp \
		$(SRC)/terminate.cpp \
		$(SRC)/utils.cpp
HDRS = \
		$(SRC)/GameBoard.hpp\
		$(SRC)/io.hpp\
		$(SRC)/main.hpp\
		$(SRC)/parallel_search_algorithms.hpp\
		$(SRC)/search_algorithms.hpp\
		$(SRC)/solution.hpp \
		$(SRC)/terminate.hpp \
		$(SRC)/utils.hpp
OBJS = \
		$(BIN)/GameBoard.o\
		$(BIN)/io.o\
		$(BIN)/main.o\
		$(BIN)/parallel_search_algorithms.o\
		$(BIN)/search_algorithms.o\
		$(BIN)/solution.o \
		$(BIN)/terminate.o \
		$(BIN)/utils.o


# Rules are of the format
# target: <prerequisite>
# where prerequisites are optional.
main: $(OBJS)
		$(CC) -o $(BIN)/finalgame.o $(OBJS)
# $(BIN)/utils.o $(BIN)/GameBoard.o $(BIN)/io.o $(BIN)/search_algorithms.o $(BIN)/main.o

# $< - means the first prerequisite listed
# $@ - prints the target name
$(BIN)/%.o: $(SRC)/%.cpp
		$(CC) $(CFLAGS) $@ $<
clean: 
		rm -f pengu $(BIN)/*.o