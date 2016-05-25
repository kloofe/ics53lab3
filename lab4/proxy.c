#include "csapp.h"

int main(int argc, char **argv) {
	int listenfd, connfd;
	char hostname[1024], port[1024];
	socklen_t clientlen;
	struct sockaddr_storage clientaddr;

	if(argc != 2) {
		exit(1);
	}
	
	listenfd = Open_listenfd(argv[1]);
	while(1) {
		clientlen = sizeof(clientaddr);
		connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
		Getnameinfo((SA *) &clientaddr, clientlen, hostname, 1024, port, 1024, 0);
		printf("Accepted connection from (%s, %s)\n", hostname, port);
		Close(connfd);
	}
	return 0;
}
