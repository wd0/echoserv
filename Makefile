CC = cc
CFLAGS = -Wall -Wpedantic -Wextra -ansi
SRC = echo.c
NAME = echoserv


all: build


build: 
	${CC} -o ${NAME} ${CFLAGS} ${SRC} 

clean:
	rm -f ${NAME}
