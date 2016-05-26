#include "csapp.h"

void foo(int fd) {
	struct stat sbuf;
	char buf[1024], method[1024], uri[1024], version[1024];
	char filename[1024], cgiargs[1024];
	rio_t rio;

	Rio_readinitb(&rio, fd);
	Rio_readlineb(&rio, buf, 1024);
	sscanf(buf, "%s %s %s", method, uri, version);
	printf("%s %s %s\n", method, uri, version);
}

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
		foo(connfd);
		Close(connfd);
	}
	return 0;
}
