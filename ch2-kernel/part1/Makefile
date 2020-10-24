obj-m+=simple.o
all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	mv *.o ./build/
	mv *.mod.c ./build/
	mv *.ko ./build/
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
	rm -f ./build/*
