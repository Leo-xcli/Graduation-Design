#ifndef LED_H
#define LED_H

int led_open(const char *devname);
int led_ioctl(int led_fd,unsigned int cmd, unsigned long led_num);
int led_close(int led_fd);
#endif  //LED_H
