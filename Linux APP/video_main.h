#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <getopt.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <malloc.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <linux/videodev2.h>
#include <asm/types.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>


#include <termio.h>
#include <syslog.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <bits/signum.h>
#include <netdb.h>
#include <sys/resource.h>
/*
#define SERVERPORT 6666
#define SERVERIP "192.168.1.10"
*/
#define SERVERIP "192.168.43.144"  //TCP服务器IP
#define CLEAR(x) memset (&(x), 0, sizeof (x))
pthread_mutex_t  g_lock;/*定义一个互斥锁*/
pthread_cond_t   g_cond;
typedef struct data
{
		unsigned int datasize;
		char buf[];
}buf_t;
buf_t *databuf; //TCP传输结构体
void *pthread_snd(void *socketsd);
void *pthread_video(void *arg);
