/* client.c */
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
 
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h> /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include<unistd.h>
#include <cstdlib>
#include <string>
#include <iostream>



#define MAXLINE 80
#define SERV_PORT 8000
 
int main(int argc, char *argv[])
{
	struct sockaddr_in servaddr;
	char buf[MAXLINE];
	int sockfd, n;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr.s_addr);//本地ip
	servaddr.sin_port = htons(SERV_PORT);
 
	connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
 
	while (fgets(buf, MAXLINE, stdin) != NULL) {
		write(sockfd, buf, strlen(buf));
		n = read(sockfd, buf, MAXLINE);
		if (n == 0)
		printf("the other side has been closed.\n");
		else
		write(STDOUT_FILENO, buf, n);
		}
		close(sockfd);
	return 0;
}
