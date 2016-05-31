#include "csapp.h"

void parse_uri(char* uri, char* hostname, char* path) {
	//parse uri (which will then be path)
	char substr[MAXLINE] = {};
	char *temp, *uritemp;
	uritemp = uri+7;//assume uri has http://
	temp = strchr(uritemp, '/');
	if (temp != NULL){
		strncpy(substr, temp, strlen(temp));
		strncpy(path, substr, strlen(uri));
		printf("substr: %s \npath: %s \n", substr, path);
	}
}

int write_log(char *hostname, char* uri, char *cl){
	FILE *log;
	struct addrinfo *p, *listp, info;
	int flag, spanlen;
	char buf[20];
	char uriparsed[MAXLINE]={};
	char *temp;
	//get time
	time_t now = time(0);	
	char cday[4], nday[4], month[4], time[8], year[4]; 
	sscanf(ctime(&now), "%s %s %s %s %s", cday, month, nday, time, year);
	//get ip address
	memset(&info, 0, sizeof(struct addrinfo));
	info.ai_family = AF_INET;
	info.ai_socktype = SOCK_STREAM;
	Getaddrinfo(hostname, NULL, &info, &listp);
	flag = NI_NUMERICHOST;
	p = listp;
	Getnameinfo(p->ai_addr, p->ai_addrlen, buf, MAXLINE, NULL, 0, flag);
	Freeaddrinfo(listp);
	//reparse uri
	temp = uri+7;
	spanlen = strcspn(temp, "/");
	strncpy(uriparsed, uri, spanlen+7);
	//open file and write to it
	log = fopen("proxy.log", "a");
	if (log == NULL) return -1;
	fprintf(log, "%s %s %s %s %s GMT: %s %s/ %s\n",cday, nday, month, year, time, buf, uriparsed, cl);
	fclose(log);	
	return 0;
}

void foo(int fd) {
	int serverfd;
	char method[20], uri[MAXLINE], path[MAXLINE], version[20];
	char hostname[MAXLINE] = {};
	char request[MAXLINE] = {};
	char temp[MAXLINE]= {};
	char buf[MAXLINE]={};
	int n;
	char contentlength[20];	
	rio_t rio;

	Rio_readinitb(&rio, fd);
 	Rio_readlineb(&rio, buf, MAXLINE); //get the first line http request
	sscanf(buf, "%s %s %s", method, uri, version);
 	Rio_readlineb(&rio, buf, MAXLINE);//Host: web:port line if 1.1 
	sscanf(buf, "Host: %s", hostname);
	printf("--debug header request: %s, %s, %s, %s\n", method, uri, version, hostname);
	
	//loop through rest of request
 	while(1){
		printf("buf: %s", buf);
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
	printf("--debug Method: %s\n", method);
	if (strncmp(method, "GET", 3) != 0){
		printf("Non-get request\n");
		return;
	}

	int tempnum;
	//parsing here
	parse_uri(uri, hostname, path); 
	printf("uri after parse: %s\n", uri);
	strcpy(temp, "GET ");
	strcat(temp, path);
	strcat(temp, " ");
	strcat(temp, version);
	strcat(temp, "\r\nHost: ");
	strcat(temp, hostname);
//	strcat(temp, "\r\n\r\n");
	strcat(temp, "\r\nConnection: close\r\n\r\n");
//	strcpy(temp, "GET /~harris/test.html HTTP/1.1\r\nHost: www.ics.uci.edu\r\n\r\n");
	printf("--get request: %s\n",temp);
	serverfd = Open_clientfd(hostname, "80"); //connect to webserver
 	
	//send request to end server
 	Rio_writen(serverfd, temp, strlen(temp));
	//Rio_writen(serverfd, request, strlen(request)); //not sure if to add all header request....
	
	//opens empty read buffer associated to serverfd
	Rio_readinitb(&rio, serverfd);
	
	//grab first line to check if ok header response
	Rio_readlineb(&rio, buf, MAXLINE); 
	if (strstr(buf, "OK") == NULL){
		printf("Header response not 200, : %s\n", buf);
		Close(serverfd);
		return;
	}

	//prepare for reading rest of header.. if has content-length keep
	while(1){
		Rio_readlineb(&rio, buf, MAXLINE);
		printf("head: %s", buf);
		if(strcmp(buf, "\r\n") == 0)
			break;
		if(strstr(buf, "Content-Length: ") != NULL){//get contentlength string
			sscanf(buf, "Content-Length: %s", contentlength);
		}
	}

	if ((tempnum = write_log(hostname, uri, contentlength)) < 0)
	{
		printf("Couldn't write to proxy.log");
	}
	//read the html body and write to client
	while(1){
		Rio_writen(fd, buf, strlen(buf));
		if (Rio_readlineb(&rio, buf, MAXLINE) <= 0) break;
		printf("body: %s",buf);
	} 
	Close(serverfd);
	Close(fd);
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
	}
	return 0;
}
