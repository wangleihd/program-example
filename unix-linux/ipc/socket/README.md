# 进程间通信 IPC (Inter-Process Communication)

## SOCKET




## 编译运行

```
[root@localhost]# ./s_unix &
[1] 11664
[root@localhost]# ./c_unix &

=====info=====
Message from client (1024)) :message from client
Message from client (1024)) :message from client
Message from client (1024)) :message from client
Message from client (1024)) :message from client
[2] 11665
[1]-  Done                    ./s_unix
[2]+  Done                    ./c_unix
[root@localhost]#


当运行s_unix程序后，该程序将处于监听状态。这时，可以通过netstat命令查看程序运行情况，s_unix的套接字类型为流套接字，并处于监听状态。
[root@localhost]# 
[root@localhost]# ./s_unix &
[1] 12056
[root@localhost]# netstat -an |grep /tmp
unix  2      [ ACC ]     STREAM     LISTENING     64014  /tmp/ssh-CekOJ11069/agent.11069
unix  2      [ ACC ]     STREAM     LISTENING     6216   /tmp/.font-unix/fs7100
unix  2      [ ACC ]     STREAM     LISTENING     62042  /tmp/ssh-XOCgkr9439/agent.9439
unix  2      [ ACC ]     STREAM     LISTENING     62316  /tmp/ssh-mojoaQ9648/agent.9648
unix  2      [ ACC ]     STREAM     LISTENING     65267  /tmp/UNIX.domain
unix  2      [ ACC ]     STREAM     LISTENING     65210  /tmp/ssh-NlKtA12012/agent.12012
```
