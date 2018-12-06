#include "led.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int led_open(const char *devname)
{
	int led_fd=open(devname, O_RDWR);//调用驱动函数，打开设备文件	
    if(led_fd<0) //如果打开失败
	{
		printf("open device %s failed.\n",devname);
		return -1;
    }
    printf("LED driver is ok\n");
	return led_fd;
}

int led_ioctl(int led_fd,unsigned int cmd, unsigned long led_num)
{
	
	//printf("on/off is ok\t");
	//printf("cmd=%d\n",cmd);
	ioctl(led_fd, cmd, led_num-1); //调用驱动函数，设置对应io口的状态
	return 0;
}

int led_close(int led_fd)
{
	if(led_fd)
		close(led_fd);//调用驱动函数，关闭设备文件
}
