obj-m+=birthday_list.o
all:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) modules
	mv *.o ./build/
	mv *.mod.c ./build/
	mv *.ko ./build/
	mv .* ./build/
clean:
	make -C /lib/modules/$(shell uname -r)/build/ M=$(PWD) clean
	rm -f ./build/*