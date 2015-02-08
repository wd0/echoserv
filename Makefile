CC = cc
CFLAGS = -Wall -Wpedantic -Wextra -ansi
SRC = echoserv.c
NAME = echoserv


all: build


build: 
	${CC} -o ${NAME} ${CFLAGS} ${SRC} 

install: all
	install ${NAME} /usr/local/bin

uninstall:
	rm -i /usr/local/bin/${NAME}

clean:
	rm -f ${NAME}
