CC=g++
FLAGS=-Wfatal-errors -std=c++20
BIN=./calc
SRC=src/main.cc

all: calc

clean:
	rm -f ${BIN}

calc: clean
	${CC} ${FLAGS} ${SRC} -o ${BIN}

run: calc
	${BIN}

test: calc
	./test.sh
