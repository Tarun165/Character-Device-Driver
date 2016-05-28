
PURPOSE: Make a char device driver using producer and consumer problem.

FILES: 1)Character Device driver code with open, read, write and release method: chardev.c 
	   2)Producer which produce the string and put into the buffer: my_producer.c 
	   3)Consumer, consumer the given length string: my_consumer.c 
	   4)To compile the Code: Makefile

COMPILE THE CODE: sudo make

RUN THE CODE: 1)Insert module and initialize the buffer size
				sudo insmod chardev bsize=<buffer_size>

			  2)Producer and consumer  	
			  ./my_producer.o <string_name> <length_string>
			  ./my_consumer.o <length_string>

			  3)Remove module
			  make clean
			  
SOFTWARE REQUIREMENT: linux kernel-4.4.3

CODE REFERENCE:
	(1) http://lxr.free-electrons.com/ident?i=copy_to_user
	(2)https://www.quora.com/Linux-Kernel-How-does-copy_to_user-work
	(3)http://linux.die.net/lkmpg/x569.html
	(4)http://derekmolloy.ie/writing-a-linux-kernel-module-part-2-a-character-device/
	(5)https://appusajeev.wordpress.com/2011/06/18/writing-a-linux-character-device-driver/
	(6)http://www.crashcourse.ca/introduction-linux-kernel-programming/lesson-17-your-first-character-device-driver
	(7)https://www.freebsd.org/doc/en/books/arch-handbook/driverbasics-char.html
	(8)http://lxr.free-electrons.com/ident?i=copy_to_user
	(9)https://www.fsl.cs.sunysb.edu/kernel-api/re256.html
	(10)http://www.c4learn.com/c-programming/c-double-pointer/
	(11)http://stackoverflow.com/questions/252782/strdup-what-does-it-do-in-c
	(12)http://lxr.free-electrons.com/source/kernel/locking/semaphore.c
	(13)https://www.eskimo.com/~scs/cclass/int/sx8.html
	(14)http://www.ibm.com/developerworks/library/l-kernel-memory-access/
	(15)http://www.cs.bham.ac.uk/~exr/lectures/opsys/12_13/docs/kernelAPI/
	(16)http://stackoverflow.com/questions/2099070/how-can-i-allocate-memory-in-the-linux-kernel-for-a-char-type-string
	(17)http://stackoverflow.com/questions/1853642/copying-a-2d-array-of-char-to-user-space-from-kernel-space
	(18)http://linux.die.net/man/3/strdup
	(19)http://stackoverflow.com/questions/8164000/how-to-dynamically-allocate-memory-space-for-a-string-and-get-that-string-from-u
	(20)http://www.tutorialsdaddy.com/tutorials/writing-character-device-driver.php?course=Linux%20device%20driver


EXTRA CREDIT: N/A