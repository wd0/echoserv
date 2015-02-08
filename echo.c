#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

enum { NPORT = 2800 };
enum { DEFAULTBUF = 1024 };
enum { NLISTEN = 8 };

void *
emalloc(size_t n) {
    void *m = malloc(n);
    if (m == NULL) { 
	perror("malloc");
	exit(1);
    }
    return m;
}

int
echo(int sock) {
    char *echobuf = NULL;
    int bufsize = DEFAULTBUF;
    long nread, nsent;

    echobuf = emalloc(bufsize);
    nread = recv(sock, echobuf, bufsize, 0);
    if (nread == -1) {
	perror("recv");
	close(sock);
	return -1;
    } else if (nread == 0) {
	return 0;
    } else if (nread > 0) {
	nsent = send(sock, echobuf, nread, 0);
	while (nsent != nread) { /* Fix me */
	    nsent = send(sock, echobuf + nsent, nread - nsent, 0);
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
	port = NPORT;
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
