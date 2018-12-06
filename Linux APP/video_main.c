#include "video_main.h"
#include "led.h"
#include "pca9685.h"

#define PORT 1234  //TCP服务端端口

int uart_init(int arg, int baud)
{
    int fd;
    char port[20];
    struct termios Opt;
    int uartbiit[6]= {B115200,B9600,B19200,B4800,B2400,B1200};
    sprintf(port,"/dev/ttySAC%d",arg);   
    printf("Use port: %s \n", port);
    fd = open(port, O_RDWR);     //打开串口
    if (fd<0)
    {
        return -1;                 //没有打开返回
    }
    tcgetattr(fd,&Opt);      //初始化
    tcflush(fd,TCIFLUSH);
    cfsetispeed(&Opt,uartbiit[baud]);    //设置波特率
    cfsetospeed(&Opt,uartbiit[baud]);
    Opt.c_cflag |= CS8;        //设置数据位
    Opt.c_cflag &= ~PARENB;   // //c_cflag 控制选择
    Opt.c_cflag &= ~CSTOPB;
	
	Opt.c_oflag &= ~(OPOST);	//输出选择
    Opt.c_lflag &= ~(ICANON|ISIG|ECHO|IEXTEN);//行选择
	
    Opt.c_iflag &= ~(INPCK|BRKINT|ICRNL|ISTRIP|IXON); //输入选择
	
    Opt.c_cc[VMIN] = 64;            //最大长度
    Opt.c_cc[VTIME] = 1;            //超时时间
    if (tcsetattr(fd,TCSANOW,&Opt) != 0)       //装载初始化参数
    {
        perror("SetupSerial!\n");
        close(fd);
        return -1;
    }
    return(fd);
}
int Tcp_Init(void)
{
	int sockfd;
    //数据缓冲区
    struct hostent *he;
    //服务器IPv4地址信息
    struct sockaddr_in server;
 
    //通过gethostbyname()得到服务端的主机信息
	if((he = gethostbyname(SERVERIP)) == NULL)
    {
        printf("gethostbyname() error\n");
        exit(1);
    }
 
    //使用socket()创建套接字
    if((sockfd= socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("socket() error\n");
        exit(1);
    }
 
    //初始化server地址信息
    bzero(&server, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);
    server.sin_addr = *((struct in_addr *)he->h_addr);
     
    //使用connect()函数来配置套接字，建立一个与TCP服务器的连接
    if(connect(sockfd, (struct sockaddr *)&server, sizeof(server)) == -1)
    {
        printf("connect() error\n");
        exit(1);
    }
	return sockfd;
}
int data_prase(char *data_buf)
{
	if((data_buf[0]==0x42)&&(data_buf[1]==0x4d))
	{	
		//printf("successed 1\n");
		int len=strlen(data_buf);
		if(data_buf[2]==len)
		{	
			//printf("successed 2\n");
			if(data_buf[len-1]==0x55)
				return 1;
			else
				return 0;//返回0表示错误
		}
		else
		{
			return 0;//返回0表示错误
		}
	}
	else
	{
		return 0;//返回0表示错误
	}
}
int main()
{
		signal(SIGPIPE,SIG_IGN); /*用来防止客户端断开连接，程序中断*/
		pthread_t tid;
		pthread_t sendid;
		pthread_mutex_init(&g_lock,NULL);
		pthread_cond_init(&g_cond,NULL);
		
		char temp_buf[100];//TCP环境数据发送buf
		char buf[64];//串口数据接收buf
		char Tcp_buf[64];//串口数据接收buf
		unsigned char Environmental_switch=0;
		fd_set fdsr;
		int maxsock;

		int sock_fd=Tcp_Init();  //连接TCP服务器 192.168.1.4 端口1234
		int serial_fd = uart_init(2,1);//打开串口3，波特率9600
		int led_fd = led_open("/dev/leds");
		int pwm_fd =open("/dev/pwm",O_RDWR);
		pca9685_init();//舵机驱动控制器初始化
		
		if(pwm_fd<0) //如果打开失败
		{
			printf("open device /dev/pwm failed.\n");
			return -1;
		}
		struct timeval tv;
		video_init();//摄像头初始化，并且打开摄像头
		if(sock_fd>serial_fd)
			maxsock=sock_fd;
		else
			maxsock=serial_fd;
		int ret=pthread_create(&tid,NULL,pthread_video,NULL);
		while(1)
		{
				FD_ZERO(&fdsr);
				FD_SET(sock_fd,&fdsr);
				FD_SET(serial_fd,&fdsr);
				tv.tv_sec=30;
				tv.tv_usec=0;
				ret=select(maxsock+1,&fdsr,NULL,NULL,NULL);//select一直阻塞
				if(ret<0)
				{
						perror("select");
						break;
				}
				else if(ret==0)
				{
						printf("timeout\n");
						continue;
				}
				if(FD_ISSET(sock_fd,&fdsr))
				{		
						int result;
						memset(Tcp_buf, 0 ,sizeof(Tcp_buf));
						read(sock_fd,Tcp_buf,64);
						result=data_prase(Tcp_buf);
						if(result)//数据接收成功
						{	
							printf("successed data\n");
							switch(Tcp_buf[3])//判断帧类型
							{
								case 1:	//小车相关控制命令
									if(Tcp_buf[4]==0x01)
									{	
										ioctl(pwm_fd,8,8);//后两位参数设置左右轮占空比
										led_ioctl(led_fd,0,1);
										led_ioctl(led_fd,1,2);
										led_ioctl(led_fd,0,3);
										led_ioctl(led_fd,1,4);
									}else if(Tcp_buf[4]==0x02)
									{	
										ioctl(pwm_fd,8,8);//后两位参数设置左右轮占空比
										led_ioctl(led_fd,1,1);
										led_ioctl(led_fd,0,2);
										led_ioctl(led_fd,1,3);
										led_ioctl(led_fd,0,4);
									}else if(Tcp_buf[4]==0x03)
									{	
										ioctl(pwm_fd,3,8);//后两位参数设置左右轮占空比
										printf("left\n");
									}else if(Tcp_buf[4]==0x04)
									{	
										ioctl(pwm_fd,8,3);//后两位参数设置左右轮占空比
										printf("right\n");
									}else if(Tcp_buf[4]==0x05)
									{	
										led_ioctl(led_fd,0,1);
										led_ioctl(led_fd,0,2);
										led_ioctl(led_fd,0,3);
										led_ioctl(led_fd,0,4);
									}
									break;
								case 2://摄像头相关控制命令
									if(Tcp_buf[4]==0x01)//帧类型0x02 命令值0x01
									{	
										
										ret=pthread_create(&sendid,NULL,pthread_snd,((void *)sock_fd));//开始摄像头数据传输
									}
									else if(Tcp_buf[4]==0x02)//帧类型0x02 命令值0x00
									{
										ret=pthread_cancel(&sendid);	//关闭摄像头传输
									}else if(Tcp_buf[4]>=5)//帧类型0x02 命令值0x00
									{
										update_DutyCycle(0,100,240+Tcp_buf[4]*5);//控制序号为0的舵机
									}
									break;
								case 3://环境监控命令
									if(Tcp_buf[4]==0x01)//帧类型0x03 命令值0x01
									{
										Environmental_switch=1;//开始环境数据传输
									}
									else if(Tcp_buf[4]==0x02)//帧类型0x02 命令值0x02
									{
										Environmental_switch=0;	//关闭环境数据传输
									}
									break;
							}
						}
						else
						{
							printf("data rev error\n");
						}
						
				}
				if(FD_ISSET(serial_fd,&fdsr))
				{	
					memset(buf, 0 ,sizeof(buf));
					read(serial_fd,buf,64);
					int PM2_5,PM10,TEMP,RH;
					PM2_5=buf[6]*255+buf[7];
					PM10 =buf[8]*255+buf[9];
					TEMP =buf[30]*255+buf[31];
					RH   =buf[32]*255+buf[33];
					//printf("PM2_5:%4d PM10:%4d TEMP:%4d RH:%4d\n",PM2_5,PM10,TEMP,RH);
					if(Environmental_switch)
					{
						sprintf(temp_buf,"PM2_5:%4d PM10:%4d TEMP:%4d RH:%4d\n",PM2_5,PM10,TEMP,RH);
						write(sock_fd, temp_buf,strlen(temp_buf));		
					}
									
						
				}
		}
		led_close(led_fd);
		return 0;
}



