#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
void update_DutyCycle(int CH_num,int LED_ON,int LED_OFF);
void pca9685_Hz(int  Hz);
void pca9685_init(void);