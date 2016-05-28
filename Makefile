obj-m = chardev.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
	gcc -o my_consumer.o my_consumer.c
	gcc -o my_producer.o my_producer.c

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
	rm ./my_consumer.o ./my_producer.o
