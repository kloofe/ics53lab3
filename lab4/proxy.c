#include "csapp.h"

void parse_uri(char* uri, char* hostname, char* path) {
	//parse uri and get path)
	char substr[MAXLINE] = {};
	char *temp, *uritemp;
	uritemp = uri+7;//assume uri has http://
	temp = strchr(uritemp, '/');
	if (temp != NULL){
		strncpy(substr, temp, strlen(temp));
		strncpy(path, substr, strlen(uri));
	}
}

int write_log(char *hostname, char* uri, char *cl, char* ipc){
	FILE *log;
	//get time
	time_t now = time(0);	
	char cday[4], nday[4], month[4], time[8], year[4]; 
	sscanf(ctime(&now), "%s %s %s %s %s", cday, month, nday, time, year);
	//open file and write to it
	log = fopen("proxy.log", "a");
	if (log == NULL) return -1;
	fprintf(log, "%s %s %s %s %s GMT: %s %s/ %s\n",cday, nday, month, year, time, ipc, uri, cl);
	fclose(log);	
	return 0;
}

void foo(int fd, char* ip) {
	int serverfd;
	char method[20], uri[MAXLINE], path[MAXLINE], version[20];
	char hostname[MAXLINE] = {};
	char temp[MAXLINE]= {};
	char buf[MAXLINE]={};
	int n, tempnum;
	char contentlength[20];	
	rio_t rio;

	Rio_readinitb(&rio, fd);
 	Rio_readlineb(&rio, buf, MAXLINE); //get the first line http request
	sscanf(buf, "%s %s %s", method, uri, version);
 	if (strcmp(version, "HTTP/1.1") == 0){
		Rio_readlineb(&rio, buf, MAXLINE);//Host: web:port line if 1.1 
		sscanf(buf, "Host: %s", hostname);
	}
	//parse for hostname and path
        parse_uri(uri, hostname, path);

	//printf("--debug header request: %s, %s, %s, %s\n", method, uri, version, hostname);
	printf("Client is sending %s request to %s...\n", method, hostname);	
	//loop through rest of request
 	while(1){
		if ((n = Rio_readlineb(&rio, buf, MAXLINE)) <= 0){
			printf("Can't read request from client to end server\n");
			fflush(stdout);
			return;
		}
		if (strcmp(buf, "\r\n") == 0)
			break;
	}
	if (strncmp(method, "GET", 3) != 0){
		printf("%s is unsupported: %s\n", method, uri);
		return;
	}

	//printf("uri after parse: %s\n", uri);
	//preparing request to send to end server, only supports http/1.1
	strcpy(temp, "GET ");
	strcat(temp, path);
	strcat(temp, " ");
	strcat(temp, version);
	strcat(temp, "\r\nHost: ");
	strcat(temp, hostname);
	strcat(temp, "\r\nConnection: close\r\n\r\n");
	
	//connect to webserver with hostname and port 80
	serverfd = Open_clientfd(hostname, "80");
 	
	//send get and host(http1.1) request to end server
 	Rio_writen(serverfd, temp, strlen(temp));
	
	//opens empty read buffer associated to serverfd
	Rio_readinitb(&rio, serverfd);
	
	//grab first line to check if ok header response
	Rio_readlineb(&rio, buf, MAXLINE); 
	if (strstr(buf, "OK") == NULL){
		printf("Header response not 200: %s\n", buf);
		Close(serverfd);
		return;
	}

	//prepare for reading rest of header.. if has content-length keep
	while(1){
		Rio_readlineb(&rio, buf, MAXLINE);
		if(strcmp(buf, "\r\n") == 0)
			break;
		if(strstr(buf, "Content-Length: ") != NULL){//get contentlength string
			sscanf(buf, "Content-Length: %s", contentlength);
		}
	}
	if ((tempnum = write_log(hostname, uri, contentlength, &(*ip))) < 0)
	{
		printf("Couldn't write to proxy.log");
	}
	//read the html body and write to client
	while(1){
		Rio_writen(fd, buf, strlen(buf));
		if (Rio_readlineb(&rio, buf, MAXLINE) <= 0) break;
		//printf("body: %s",buf);
	}
	printf("Client retreived webpage %s with %s bytes\n", uri, contentlength); 
	Close(serverfd);
}

int main(int argc, char **argv) {
	int listenfd, connfd;
	char hostname[MAXLINE], port[MAXLINE];
	socklen_t clientlen;
	struct sockaddr_storage clientaddr;
	char ipchar[20];	

	if(argc != 2) {
		exit(1);
	}
	listenfd = Open_listenfd(argv[1]);
	while(1) {
		clientlen = sizeof(clientaddr);
		connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);
		Getnameinfo((SA *) &clientaddr, clientlen, hostname, 1024, port, 1024, 0);
		printf("Accepted connection from (%s, %s)\n", hostname, port);
		Inet_ntop(clientaddr.ss_family, &(((struct sockaddr_in *)&clientaddr)->sin_addr),ipchar,20);
		foo(connfd, ipchar);
		Close(connfd);
	}
	return 0;
}
