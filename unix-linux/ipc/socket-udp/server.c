/*
	unix domain socket: designed for interprocess communication 
	key system calls: socket/bind/connect/listen/accept
	key structure: struct sockaddr_un 
	
	Author: hittlle
	purpose: Demo of UNIX Domain Socket
	Program Name: DomainServer
*/

#include "header.h"

int main(int argc, char **argv){
	
	int listenFd, clientFd;
	struct sockaddr_un servAddr, clientAddr;
	socklen_t socklen;

	char buf[BUFSIZE];

	bzero(buf, BUFSIZE);
	bzero(&servAddr, sizeof(servAddr));

	//set address 
	servAddr.sun_family = AF_UNIX;//AF_LOCALE is also OK
	strcpy(servAddr.sun_path, SOCKET_PATH);

	unlink(SOCKET_PATH);	
	if((listenFd = socket(PF_UNIX, SOCK_DGRAM,0)) < 0)
		error("%s", strerror(errno));
	
	if(bind(listenFd, (struct sockaddr*)&servAddr,sizeof(servAddr)) < 0){
		close(listenFd);
		error("%s", strerror(errno));
	}
	
	while(1){
		if(recvfrom(listenFd,buf,BUFSIZE,0,(struct sockaddr*)&clientAddr,&socklen) > 0)
			write(STDOUT_FILENO,buf,sizeof(buf));
	}
	
	return 0;
}



