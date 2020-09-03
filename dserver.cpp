/* server.c */
#include <stdio.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <ctype.h>

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
//编译的时候 因为thread不是内置的  要加上gcc main.c -o test -lpthread 
struct s_info {
	struct sockaddr_in cliaddr;
	int connfd;
};
void *do_work(void *arg)
{
	int n,i;
	struct s_info *ts = (struct s_info*)arg;
	char buf[MAXLINE];//在线程自己的用户空间栈开辟的,该线程运行结束的时候,主控线程就不能操作这块内存了
	char str[INET_ADDRSTRLEN];//INET_ADDRSTRLEN 是宏16个字节
 
//在创建线程前设置线程创建属性,设为分离态,效率高
	pthread_detach(pthread_self());
 
	while (1) {
	n = read(ts->connfd, buf, MAXLINE);
	if (n == 0) {
		printf("the other side has been closed.\n");
		break;
		}
	printf("received from %s at PORT %d\n",
	inet_ntop(AF_INET, &(*ts).cliaddr.sin_addr, str, sizeof(str)),ntohs((*ts).cliaddr.sin_port));
 
	for (i = 0; i < n; i++)
	    buf[i] = toupper(buf[i]);
	write(ts->connfd, buf, n);
	}
	close(ts->connfd);
}
int main(void)
{
	struct sockaddr_in servaddr, cliaddr;
	socklen_t cliaddr_len;
	int listenfd, connfd;
	int i = 0;
	pthread_t tid;
	struct s_info ts[3497];
 
	listenfd = socket(AF_INET, SOCK_STREAM, 0);
 
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(SERV_PORT);
	
	bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	listen(listenfd, 20);
 
	printf("Accepting connections ...\n");
	while (1) {
		cliaddr_len = sizeof(cliaddr);
		connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &cliaddr_len);
		ts[i].cliaddr = cliaddr;
		ts[i].connfd = connfd;
/* 达到线程最大数时，pthread_create出错处理, 增加服务器稳定性 */
		pthread_create(&tid, NULL, do_work, (void*)&ts[i]);//把accept得到的客户端信息传给线程，让线程去和客户端进行数据的收发
		i++;
	}
return 0;
}
