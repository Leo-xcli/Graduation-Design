/*************************************************************************
	> File Name: test.c
	> Author: lxc
	> Mail: 394101651@qq.com
	> Created Time: Fri 02 Mar 2018 04:27:05 PM CST
 ************************************************************************/
#include "pca9685.h"
static int pca_fd;
//设置PCA9685的输出频率。
void pca9685_Hz(int  Hz)
{
    unsigned char buf[2];
    unsigned char use_Hz;
    use_Hz = 25000000/4096/Hz-1;
    buf[0]=0x00;
    buf[1]=0x11;
    write(pca_fd,buf,2);
    
    buf[0]=0x01;
    buf[1]=0x04;
    write(pca_fd,buf,2);
    
    buf[0]=0xFE;
    buf[1]=use_Hz;
    write(pca_fd,buf,2);
    
    buf[0]=0x00;
    buf[1]=0x01;
    write(pca_fd,buf,2);
}
/*
	*CH_num:控制的舵机序号
	*LED_ON:LEN ON 的开始计数号
	*LED_OFF :LED ON的结束计数号
	* 占空比为(LED_OFF-LED_ON)/4096*100%
	*/
void update_DutyCycle(int CH_num,int LED_ON,int LED_OFF)
{
    unsigned char buf[2]; 
    buf[0]=CH_num*4+6;
    buf[1]=LED_ON%256;
    write(pca_fd,buf,2);
    
    buf[0]=CH_num*4+7;
    buf[1]=LED_ON/256;
    write(pca_fd,buf,2);
    
    buf[0]=CH_num*4+8;
    buf[1]=LED_OFF%256;
    write(pca_fd,buf,2);
    
    buf[0]=CH_num*4+9;
    buf[1]=LED_OFF/256;
    write(pca_fd,buf,2);
    
}
void pca9685_init(void)
{
	pca_fd = open("/dev/mpu6050", O_RDWR);
    pca9685_Hz(50);
	update_DutyCycle(0,100,450);//初始化将舵机置于中间的位置。
	/*
    int LED_ON;
    int LED_OFF;
    int CH_num;
    while(1)
    {
        printf("set the CH_num and LED_ON LED_OFF\n");
        scanf("%d%d%d",&CH_num,&LED_ON,&LED_OFF);
        update_DutyCycle(CH_num,LED_ON,LED_OFF);
    }*/

}

