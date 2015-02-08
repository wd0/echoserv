CC = clang
CFLAGS = -Wall -Wpedantic -Wextra -ansi
SRC = echo.c
NAME = echo


all: build


build: clean
	${CC} -o ${NAME} ${CFLAGS} ${SRC} 

test: build
	./${NAME} 

clean:
	rm -f ${NAME}
