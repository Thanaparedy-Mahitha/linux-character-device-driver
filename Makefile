
# Change the target by swapping the obj-m line depending on the file you use.
# For hello.c:
obj-m +=char1.o

KDIR := /lib/modules/$(shell uname -r)/build

PWD  := $(shell pwd)

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

