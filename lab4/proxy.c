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
	strncpy(substr, substr, (int) (strchr(substr, "/") - substr));
	printf("substr: %s hostname: %s \n", substr, uri);
	strcpy(hostname, substr);
	strcpy(port, "80"); // default for now
}

void foo(int fd) {
	struct stat sbuf;
	int serverfd;
	char buf[MAXLINE], method[MAXLINE], uri[MAXLINE], version[MAXLINE];
	char hostname[MAXLINE], port[MAXLINE];
	char request[MAXLINE] = {};
	char temp[MAXLINE];
	int n;	
	rio_t rio;

	Rio_readinitb(&rio, fd);
 	Rio_readlineb(&rio, buf, MAXLINE); //get the first line http request
	sscanf(buf, "%s %s %s", method, uri, version);
 	Rio_readlineb(&rio, buf, MAXLINE);//Host: web:port line if 1.1 
	sscanf(buf, "Host: %s", hostname);
	printf("debug header request: %s, %s, %s, %s", method, uri, version, hostname);
	
	//loop through rest of request
 	while(1){
	//	printf("line: %s", buf);
		if ((n = Rio_readlineb(&rio, buf, MAXLINE)) <= 0){
			printf("Can't read request from client to end server\n");
			fflush(stdout);
			return;
		}
		if (strcmp(buf, "\r\n") == 0)
			break;
		strcat(request, buf); 	
	}
	//printf("request: %s \n strlen: %zd\n", request,strlen(request));
	printf("debug Method: %s\n", method);
	if (strncmp(method, "GET", 3) != 0){
		printf("Non-get request\n");
		return;
	}
	//need to parse here
	//parse_uri(uri, hostname, port);
	strcpy(temp, "GET /~harris/test.html HTTP/1.1\r\nHost: www.ics.uci.edu\r\n\r\n");
	printf("temp: %zd\n",strlen(temp));
	serverfd = Open_clientfd("www.ics.uci.edu", "80"); //connect to webserver
 	
	//send request to end server
 	Rio_writen(serverfd, temp, strlen(temp));
	Rio_writen(serverfd, request, strlen(request));
	
	//opens empty read buffer associated to serverfd
	Rio_readinitb(&rio, serverfd);
	
	//grab first line to check if ok header response
	Rio_readlineb(&rio, buf, MAXLINE); 
	if (strstr(buf, "OK") == NULL){
		Close(serverfd);
		return;
	}

	//prepare for reading rest of header.. if has content-length keep
	while(1){
		Rio_readlineb(&rio, buf, MAXLINE);
		printf("%s", buf);
		if(strcmp(buf, "\r\n") == 0)
			break;
	}

	//read the html body and write to client
	while(1){
		Rio_writen(fd, buf, strlen(buf));
		Rio_readlineb(&rio, buf, MAXLINE);
		printf("%s",buf);
		if(strcmp(buf, "\r\n") == 0 || strstr(buf, "</html>")) //fix
                        break;
	} 
	Close(serverfd);
/*
	Rio_writen(serverfd, method, strlen(method));
	Rio_writen(serverfd, " ", strlen(" "));
	Rio_writen(serverfd, uri, strlen(uri));
	Rio_writen(serverfd, " HTTP/1.0\r\n", strlen(" HTTP/1.0\r\n"));
	
	Rio_readinitb(&rio, serverfd);
	Rio_readlineb(&rio, buf, MAXLINE);
*/
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
