
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdarg.h>
#include <errno.h>
#include <signal.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/un.h> //for unix domain socket
#include <pthread.h>


#define BUFSIZE 1024
#define SERVPORT 5000

//for unix domain socket
#define SOCKET_PATH  "/tmp/serverpath"
//error
#ifdef __cplusplus
extern "C" {
#endif

void error(const char *fmt,...){
	char buf[BUFSIZE];
	va_list ptr;
	memset(buf,0,BUFSIZE);
	
	va_start(ptr,fmt);
	vsnprintf(buf,BUFSIZE,fmt,ptr);
	va_end(ptr);
	
	fprintf(stderr,"%s\n",buf);
	exit(EXIT_FAILURE);
}
#ifdef __cplusplus
}
#endif

