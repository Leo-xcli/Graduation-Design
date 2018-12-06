  #include <linux/miscdevice.h>
#include <linux/delay.h>
#include <asm/irq.h>
#include <mach/regs-gpio.h>
#include <mach/hardware.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/types.h>
#include <linux/delay.h>
#include <linux/moduleparam.h>
#include <linux/slab.h>
#include <linux/errno.h>
#include <linux/ioctl.h>0
#include <linux/cdev.h>
#include <linux/string.h>
#include <linux/list.h>
#include <linux/pci.h>
#include <linux/gpio.h>
#include <asm/uaccess.h>
#include <asm/atomic.h>
#include <asm/unistd.h>
#define DEVICE_NAME "leds" //设备名(/dev/leds)
//LED 对应的 GPIO 端口列表
static unsigned long led_table [] = {
S3C2410_GPB(5),
S3C2410_GPB(6),
S3C2410_GPB(8),
S3C2410_GPB(10),
};
//LED 对应端口将要输出的状态列表
static unsigned int led_cfg_table [] = {
S3C2410_GPIO_OUTPUT,
S3C2410_GPIO_OUTPUT,
S3C2410_GPIO_OUTPUT,
S3C2410_GPIO_OUTPUT,
};
/*ioctl 函数的实现
* 在应用/用户层将通过 ioctl 函数向内核传递参数，以控制 LED 的输出状态
*/
static int sbc2440_leds_ioctl(struct inode *inode,struct file *file,unsigned int cmd,unsigned long arg)
{
      switch(cmd) {
                   case 0:
                   case 1:
                       if (arg > 4) 
			            {
                         return -EINVAL;
                         }
                     //根据应用/用户层传递来的参数(取反)，通过 s3c2410_gpio_setpin 函数设置 LED 对应的端口寄存器，
                        s3c2410_gpio_setpin(led_table[arg], !cmd);
                        return 0;
                     default:
                     return -EINVAL;
					}
}
/*
* 设备函数操作集，在此只有 ioctl 函数，通常还有 read, write, open, close 等，因为本 LED 驱动在下面已经
* 注册为 misc 设备，因此也可以不用 open/close
*/
static struct file_operations dev_fops = {
               .owner = THIS_MODULE,
               .ioctl = sbc2440_leds_ioctl,
};
/*
* 把 LED 驱动注册为 MISC 设备
*/
static struct miscdevice misc = {
           .minor = MISC_DYNAMIC_MINOR, //动态设备号
           .name = DEVICE_NAME,
           .fops = &dev_fops,
};
/*
* 设备初始化
*/
static int __init dev_init(void)
{
    int ret;
    int i;
    for (i = 0; i < 4; i++) 
	{
       //设置 LED 对应的端口寄存器为输出(OUTPUT)
       s3c2410_gpio_cfgpin(led_table[i], led_cfg_table[i]);
       //设置 LED 对应的端口寄存器为低电平输出，在模块加载结束后，四个 LED 应该是全部都是发光状态
        s3c2410_gpio_setpin(led_table[i], 0);
     }
   ret = misc_register(&misc); //注册设备printk (DEVICE_NAME"\tinitialized\n"); //打印初始化信息
   return ret;
}
static void __exit dev_exit(void)
{
    misc_deregister(&misc);
}
module_init(dev_init); //模块初始化，仅当使用 insmod/podprobe 命令加载时有用，如果设备不是通过模块方式加载，此处将不会被调用
module_exit(dev_exit);//卸载模块，当该设备通过模块方式加载后，可以通过 rmmod 命令卸载，将调用此函数
MODULE_LICENSE("GPL"); //版权信息