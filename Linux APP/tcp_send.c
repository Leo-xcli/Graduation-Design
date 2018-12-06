#include "video_main.h"
/*
int tcp_server_init()
{
		struct sockaddr_in server_addr;
		socklen_t  addrlen;
		int ret;	
		
		server_addr.sin_family=AF_INET;
		server_addr.sin_port=htons(SERVERPORT);
		inet_pton(AF_INET,SERVERIP,&server_addr.sin_addr);
		addrlen=sizeof(struct sockaddr_in);
		int sd=socket(AF_INET,SOCK_STREAM,0);
		if(sd==-1)
		{
			perror("socket\n");
			exit(1);
		}
		ret=bind(sd,(struct sockaddr *)&server_addr,addrlen);
		if(ret==-1)
		{
			perror("bind");
			exit(1);
		}
		ret=listen(sd,20);
		if(ret==-1)
		{
			perror("listen\n");
			exit(1);
		}
	return sd;
}
*/
void *pthread_snd(void *socketsd)
{
		pthread_detach(pthread_self());
		int sd=((int )socketsd);
		int newsd,ret;
		/*newsd=accept(sd,NULL,NULL);  //about TCP
		if(newsd==-1)
		{
				perror("accept");
				return NULL;
		}*/
		int len;
		//udp 部分
		int socket_descriptor; //套接口描述字  
    int iter=0;  
    char buf[80];  
	int port=6789; 
    struct sockaddr_in address;//处理网络通信的地址  
	 bzero(&address,sizeof(address));  
    address.sin_family=AF_INET;  
    address.sin_addr.s_addr=inet_addr(SERVERIP);//这里不一样  
    address.sin_port=htons(port);  
	 //创建一个 UDP socket  
  
    socket_descriptor=socket(AF_INET,SOCK_DGRAM,0);//IPV4  SOCK_DGRAM 数据报套接字（UDP协议）  
//    for(iter=0;iter<=20;iter++)  
//    {  
//         /* 
//         * sprintf(s, "%8d%8d", 123, 4567); //产生：" 123 4567"  
//         * 将格式化后到 字符串存放到s当中 
//         */  
//        sprintf(buf,"data packet with ID %d\n",iter);  
///         
//        /*int PASCAL FAR sendto( SOCKET s, const char FAR* buf, int len, int flags,const struct sockaddr FAR* to, int tolen);　　 
//         * s：一个标识套接口的描述字。　 
//         * buf：包含待发送数据的缓冲区。　　 
//         * len：buf缓冲区中数据的长度。　 
//         * flags：调用方式标志位。　　 
//         * to：（可选）指针，指向目的套接口的地址。　 
//         * tolen：to所指地址的长度。   
////　　      */  
//        sendto(socket_descriptor,buf,sizeof(buf),0,(struct sockaddr *)&address,sizeof(address));  
//    }    
		while(1)
		{    	
	    //锁住由mutex指定的mutex对象。如果mutex已经被锁住，
	     //     调用这个函数的线程阻塞直到mutex可用为止。
				pthread_mutex_lock(&g_lock);//先上锁
				pthread_cond_wait(&g_cond,&g_lock);//pthread_cond_wait() 必须与pthread_mutex 配套使用。等待被唤醒
                //len=write(newsd,databuf->buf,databuf->datasize);  
                //printf("send %d bytes data\n",len);
				sendto(socket_descriptor,databuf->buf,databuf->datasize,0,(struct sockaddr *)&address,sizeof(address));
				if(ret==-1)
				{
						printf("client is out\n");
				}
			
				pthread_mutex_unlock(&g_lock);//释放有参数g_lock指定的g_lock对象的锁。
		}
		return NULL;
}

