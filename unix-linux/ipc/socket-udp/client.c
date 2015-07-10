// Client
#include "header.h"

int main(int argc, char **argv){
	
	int sockfd;
	struct sockaddr_un servaddr;
	socklen_t socklen = sizeof(struct sockaddr_un);
	char buf[BUFSIZE] = "This is a test message here......Demo of Domain Socket\n";

	bzero(&servaddr, sizeof(servaddr));
	
	servaddr.sun_family = AF_UNIX;
	strcpy(servaddr.sun_path, SOCKET_PATH);
	
	if((sockfd = socket(PF_UNIX,SOCK_DGRAM,0)) < 0)
		error("%s", strerror(errno));
	
	
	sendto(sockfd,buf,sizeof(buf),0, (struct sockaddr*)&servaddr,socklen);
	
	close(sockfd);
	
	return 0;
}


