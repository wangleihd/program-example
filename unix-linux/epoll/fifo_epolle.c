// $ mkfifo p1 p2
// $ ./a.out p1 p2 &
// $ cat > p1
// hello

#include <stdio.h>
#include <sys/epoll.h>
#include <sys/stat.h>
#include <fcntl.h>

struct epoll_event ev;

int main(void)
{
    int pfd1 = open("p1", O_RDONLY|O_NONBLOCK);   
    int pfd2 = open("p2", O_RDONLY|O_NONBLOCK);

    int epfd = epoll_create(1);                   

    ev.data.fd = pfd1;
    ev.events = EPOLLIN|EPOLLET;
    epoll_ctl(epfd, EPOLL_CTL_ADD, pfd1, &ev);    

    ev.data.fd = pfd2;
    ev.events = EPOLLIN|EPOLLET;
    epoll_ctl(epfd, EPOLL_CTL_ADD, pfd2, &ev);    

    while(1) {
        epoll_wait(epfd, &ev, 1, -1);         
        int fd = ev.data.fd;                  
        char buf[32];
        int len = read(fd,buf,16);            
        buf[len]='\0';
        printf("fd:%d ~ %s\n", fd, buf);       
    }  //继续while循环以监听p1,p2

    return 0;
}
