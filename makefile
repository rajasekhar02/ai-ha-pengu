CFLAGS = -c -o
CC = mpic++
SRC = ./src
BIN = ./bin
SOURCE = \
		$(SRC)/GameBoard.cpp\
		$(SRC)/io.cpp\
		$(SRC)/parallel_search_algorithms.cpp\
		$(SRC)/search_algorithms.cpp\
		$(SRC)/solution.cpp \
		$(SRC)/terminate.cpp \
		$(SRC)/utils.cpp \
		$(SRC)/work_remains.cpp \
		$(SRC)/queue.cpp \
		$(SRC)/node_stack.cpp \
		$(SRC)/service_requests.cpp \
		$(SRC)/main.cpp
HDRS = \
		$(SRC)/GameBoard.hpp\
		$(SRC)/io.hpp\
		$(SRC)/parallel_search_algorithms.hpp\
		$(SRC)/search_algorithms.hpp\
		$(SRC)/solution.hpp \
		$(SRC)/terminate.hpp \
		$(SRC)/utils.hpp \
		$(SRC)/work_remains.hpp \
		$(SRC)/queue.hpp \
		$(SRC)/node_stack.hpp \
		$(SRC)/service_requests.hpp \
		$(SRC)/main.hpp
OBJS = \
		$(BIN)/GameBoard.o\
		$(BIN)/io.o\
		$(BIN)/parallel_search_algorithms.o\
		$(BIN)/search_algorithms.o\
		$(BIN)/solution.o \
		$(BIN)/terminate.o \
		$(BIN)/utils.o \
		$(BIN)/work_remains.o \
		$(BIN)/queue.o \
		$(BIN)/node_stack.o \
		$(BIN)/service_requests.o \
		$(BIN)/main.o


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