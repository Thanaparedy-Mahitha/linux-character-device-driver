
# Change the target by swapping the obj-m line depending on the file you use.
# For hello.c:
obj-m +=char1.o

# For hello_seq.c (if you pick the seq_file version):
# obj-m += hello_seq.o

KDIR := /home/mahitha/Kernel_soure/WSL2-Linux-Kernel-linux-msft-wsl-6.6.87.2 
PWD  := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

