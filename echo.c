#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

enum { PORTNO = 7 };
enum { CHUNKSIZE = 2048 };
enum { NLISTEN = 16 };

int
echo(int sock) {
    char echobuf[CHUNKSIZE];
    long nread, nsent;
    long totalsent;

    while ((nread = recv(sock, echobuf, sizeof echobuf, 0)) != 0) {
	if (nread == -1) {
	    perror("recv");
	    return 1;
	}
	nsent = send(sock, echobuf, nread, 0);
	for (totalsent = nsent; totalsent < nread; totalsent += nsent) {
	    if (nsent == -1) {
		perror("send");
		return 1;
	    nsent = send(sock, echobuf + totalsent, nread - totalsent, 0);
	    }
	}
    }

    return 0;
}

int
setup(int argc, char **argv) {
    int msock;
    struct sockaddr_in serv;
    socklen_t socksize = sizeof(struct sockaddr);
    short port; 

    if (argc > 1) {
	port = atoi(argv[1]);
    } else {
	port = PORTNO;
    }

    if ((msock = socket(AF_INET, SOCK_STREAM, 0)) == -1) { 
	perror("socket");
	return -1;
    }
    memset(&serv, 0, socksize);
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = htonl(INADDR_ANY);
    serv.sin_port = htons(port);
    if (bind(msock, (struct sockaddr *)&serv, socksize) == -1) {
	perror("bind");
	return -1;
    }
    if (listen(msock, NLISTEN) == -1) {
	perror("listen");
	return -1;
    }
    
    return msock;
}

int
main(int argc, char **argv) {
    int msock, sock;
    struct sockaddr_in remote;
    socklen_t socksize = sizeof(struct sockaddr);

    if ((msock = setup(argc, argv)) == -1) {
	return EXIT_FAILURE;
    }

    for (;;) {
	sock = accept(msock, (struct sockaddr *)&remote, &socksize);
	if (sock == -1) {
	    perror("accept");
	    continue;
	}
	echo(sock);
	close(sock);
    }

    close(msock);
    return EXIT_SUCCESS;
}
