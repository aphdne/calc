CC=g++
FLAGS=-Wfatal-errors -std=c++20
BIN=./calc

SRC=src/main.cpp

all: calc

clean:
	@if [ -f ${BIN} ]; then\
		rm ${BIN};\
	fi

calc: clean
	${CC} ${FLAGS} ${SRC} -o ${BIN}

run: calc
	${BIN}

test: calc
	./test.sh
