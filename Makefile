obj-m := toyfs_mod.o
toyfs_mod-objs := toyfs.o 
all: module mainfs


module:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm mainfs
