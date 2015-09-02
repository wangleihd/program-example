#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>     		 /* nonblocking */
#include <sys/types.h>       /* basic system data types */
#include <sys/socket.h>      /* basic socket definitions */
#include <netinet/in.h>      /* sockaddr_in{} and other Internet defns */
#include <arpa/inet.h>       /* inet(3) functions */
#include <sys/epoll.h>		 /* epoll function */
#include <sys/inotify.h>     /* inotify function */
#include <sys/resource.h>    /* setrlimit */

#define MAXEPOLLSIZE 10
#define MAXLINE 10240
#define LINE_LEN 1024

#define filename "./welcome.md"

char line[LINE_LEN] = {0};

void readfile(void){
	FILE * fp;
	if((fp = fopen(filename, "r")) == NULL){
		printf("Open %s is error.\n", filename);
		exit(1);
	}
	while(!feof(fp)){
		fgets(line, LINE_LEN, fp);
	}
	fclose(fp);
}

int handle(int connfd) {
	int nread;
	char buf[MAXLINE];
	nread = read(connfd, buf, MAXLINE);//读取客户端socket流

	if (nread == 0) {
		printf("client close the connection\n");
		return -1;
	} 
	if (nread < 0) {
		perror("read error");
		return -1;
	}    
	write(connfd, line, strlen(line) - 1);
	write(connfd, buf, nread);//响应客户端  
	return 0;
}

int main(int argc, char **argv)
{
	int  servPort = 8000;

	int listenfd, connfd, kdpfd, nfds, n, nread, inotifyfd, watchfd;
	int curfds,acceptCount = 0;
	struct sockaddr_in servaddr, cliaddr;
	socklen_t socklen = sizeof(struct sockaddr_in);
	struct epoll_event ev;
	struct epoll_event events[MAXEPOLLSIZE];
	char buf[MAXLINE];
	char buffer[LINE_LEN];

	int length, i = 0;

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
	servaddr.sin_port = htons (servPort);

	readfile();

	listenfd = socket(AF_INET, SOCK_STREAM, 0); 

	int opt = 1;
	setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	if (bind(listenfd, (struct sockaddr *) &servaddr, sizeof(struct sockaddr)) == -1){
		perror("bind error");
		exit(1);
	} 

	listen(listenfd, 10);

	/* 创建 epoll 句柄，把监听 socket 加入到 epoll 集合里 */
	kdpfd = epoll_create(MAXEPOLLSIZE);
	ev.events = EPOLLIN | EPOLLET;
	ev.data.fd = listenfd;
	epoll_ctl(kdpfd, EPOLL_CTL_ADD, listenfd, &ev);
	printf("epollserver startup,port %d\n", servPort);

	if((inotifyfd = inotify_init()) < 0){
		perror("inotify_init error.");
		exit(1);
	}

	watchfd = inotify_add_watch(inotifyfd, argv[1], IN_MODIFY);
	ev.events = EPOLLIN;
	ev.data.fd = inotifyfd;
	epoll_ctl(kdpfd, EPOLL_CTL_ADD, inotifyfd, &ev);

	curfds = 1;
	while (1) {
		/* 等待有事件发生 */
		nfds = epoll_wait(kdpfd, events, curfds, -1);
		/* 处理所有事件 */
		printf("curfds = %d event-fd = %d\n", curfds, events[0].data.fd);

		for (n = 0; n < nfds; ++n)
		{
			if(events[0].data.fd == inotifyfd){
				length = read(inotifyfd, buffer, LINE_LEN);
				printf("length = %d\n", length);
				i = 0;
				while(length > i){
					struct inotify_event *event = (struct inotify_event *)&buffer[i];
						if(event->mask & IN_MODIFY){

							readfile();
						}
					i += (sizeof(struct inotify_event)) + event->len;
				}
			}else if (events[n].data.fd == listenfd){
				connfd = accept(listenfd, (struct sockaddr *)&cliaddr,&socklen);

				sprintf(buf, "accept form %s:%d\n", inet_ntoa(cliaddr.sin_addr), cliaddr.sin_port);
				printf("%d:%s", ++acceptCount, buf);

				ev.events = EPOLLIN | EPOLLET;
				ev.data.fd = connfd;
				epoll_ctl(kdpfd, EPOLL_CTL_ADD, connfd, &ev);
				curfds++;
				continue;
			} else if (handle(events[n].data.fd) < 0) {
				epoll_ctl(kdpfd, EPOLL_CTL_DEL, events[n].data.fd,&ev);
				close(events[n].data.fd);
				curfds--;
			}
		}

	}
	inotify_rm_watch(inotifyfd, watchfd);
	close(inotifyfd);
	close(listenfd);
	return 0;
}

