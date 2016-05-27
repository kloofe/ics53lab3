#include "csapp.h"

void read_request(rio_t *rp) {
	char buf[MAXLINE];
	
	Rio_readlineb(rp, buf, MAXLINE);
	printf("Reading request:\n\n");
	while(strcmp(buf, "This is a test.\n")) {
		Rio_readlineb(rp, buf, MAXLINE);
		printf("line: %s", buf);
	}
	return;
}

void parse_uri(char* uri, char* hostname, char* port) {
	char substr[MAXLINE];
	strncpy(substr, uri + 7, strlen(uri));
	printf("substr: %s hostname: %s \n", substr, uri);
	strcpy(hostname, substr);
	strcpy(port, "80"); // default for now
}

void foo(int fd) {
	struct stat sbuf;
	int serverfd;
	char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
	char hostname[MAXLINE], port[MAXLINE];
	char temp[MAXLINE];
	rio_t rio;

	Rio_readinitb(&rio, fd);
	Rio_readlineb(&rio, buf, MAXLINE);
	sscanf(buf, "%s %s %s", method, uri, version);
	parse_uri(uri, hostname, port);
	serverfd = Open_clientfd(uri, "80");
	Rio_writen(serverfd, method, strlen(method));
	Rio_writen(serverfd, " ", strlen(" "));
	Rio_writen(serverfd, uri, strlen(uri));
	Rio_writen(serverfd, " HTTP/1.0\r\n", strlen(" HTTP/1.0\r\n"));
	
	Rio_readinitb(&rio, serverfd);
	Rio_readlineb(&rio, buf, MAXLINE);
	sscanf(buf, "%s", temp);
	printf("WHAT IS THIS: %s", temp);
	

	//read_request(&rio);
}

int main(int argc, char **argv) {
	int listenfd, connfd;
	char hostname[MAXLINE], port[MAXLINE];
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
