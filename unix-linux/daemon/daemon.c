#include <unistd.h>
#include <signal.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <stdlib.h>
#include <syslog.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>

int main(int argc,char **argv)
{
	time_t now;
	int childpid,fd,fdtablesize;
	int error,in,out;

	int i;
    char *buf = "This is a Daemon\n";
	/* 忽略终端 I/O信号,STOP信号 */
	signal(SIGTTOU,SIG_IGN);
	signal(SIGTTIN,SIG_IGN);
	signal(SIGTSTP,SIG_IGN);
	signal(SIGHUP ,SIG_IGN);
	/* 父进程退出,程序进入后台运行 */
	if(fork()!=0) 
		exit(1);
	if(setsid()<0)
		exit(1);/* 创建一个新的会议组 */
	/* 子进程退出,孙进程没有控制终端了 */
	if(fork()!=0)
		exit(1);
	if(chdir("/tmp")==-1)
		exit(1);
	/* 关闭打开的文件描述符,包括标准输入、标准输出和标准错误输出 */
	for (fd = 0, fdtablesize = getdtablesize(); fd < fdtablesize; fd++)
		close(fd);
	umask(0);/*重设文件创建掩模 */
	signal(SIGCHLD,SIG_IGN);/* 忽略SIGCHLD信号 */
	/*打开log系统*/
	syslog(LOG_USER|LOG_INFO,"守护进程测试!\n");
	while(1){
		// time(&now);
		// syslog(LOG_USER|LOG_INFO,"当前时间:\t%s\t\t\n",ctime(&now));
		// sleep(6);

		if ((fd = open("/tmp/daemon.log", O_CREAT|O_WRONLY|O_APPEND, 0600)) < 0)
            {
                syslog(LOG_USER|LOG_INFO,"Open file error\n");
                exit(1);
            }
            write(fd, buf, strlen(buf) + 1);
            close(fd);
            sleep(10);
	}
	return 0;
}
