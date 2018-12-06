#TOP_DIR := $(shell pwd)
APP      = video

#all: video 
#CFLAGS 表示用于 C 编译器的选项，CXXFLAGS 表示用于C++编译器的选项。 
#这两个变量实际上涵盖了编译和汇编两个步骤
# -O0 无优化 -g 可产生代调试信息的代码 -c 编译汇编到目标代码 -fPIC 作用于编译阶段，告诉编译器产生与位置无关代码

#指定编译器
CC      =  arm-linux-gcc  
CFLAGS  =   -g -O0 
CFALGF  =   -march=armv4t
LIBS    =  -lpthread
#DEP_LIBS  = -L    这是头文件路径,路径跟在-L后面
OBJS    =  tcp_send.o video_main.o vid.o led.o pca9685.o


# $<:第一个依赖文件  $^:所有依赖文件 $@:目标文件
# ".c.0":所有的.o文件依赖于对应的.c文件
all: $(OBJS)
	$(CC) $(CFLAGS) $(CFALGF) -o $(APP) $(OBJS) $(LIBS)

clean:
	rm -f *.o a.out
	rm -f $(APP) core *~

