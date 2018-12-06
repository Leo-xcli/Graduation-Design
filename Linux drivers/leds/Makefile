KERN_DIR = /home/uftp1/FL2440/linux-2.6.32.2_cong

all:
	make -C $(KERN_DIR) M=`pwd` modules 

clean:
	make -C $(KERN_DIR) M=`pwd` modules clean
	rm -rf modules.order

obj-m	+= mini2440_leds.o
