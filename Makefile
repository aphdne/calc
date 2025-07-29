CC=g++
FLAGS=-Wfatal-errors
BIN=./calc

SRC=src/main.cpp

all: calc

clean:
	rm ${BIN}

calc:
	${CC} ${FLAGS} ${SRC} -o ${BIN}

run: calc
	${BIN}
