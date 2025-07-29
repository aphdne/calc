CC=g++
FLAGS=-Wfatal-errors
BIN=./bin/calc

SRC=src/main.cpp

all: calc

calc:
	${CC} ${FLAGS} ${SRC} -o ${BIN}

run: calc
	${BIN}
