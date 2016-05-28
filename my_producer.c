/*Program to produce strings and write to the character device*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main(int argc, char* argv[]){
	int counter=0;
	char* input_string = argv[1];
	int input_string_length = atoi(argv[2]);
	int file_ptr = open("/dev/chardev_misc", O_WRONLY);
	if(file_ptr == -1){
		printf("Error: Device open failure!\n");
		exit(0);
	}
	for(;;){
		printf("String is %s and length %d Counter %d\n",input_string,input_string_length,counter++);
		int number_of_bytes_written = write(file_ptr, input_string, input_string_length);
		if(number_of_bytes_written <= 0){}
		else{
			printf("Number of bytes written = %d\n", number_of_bytes_written);
			printf("Write successful!\n");
		}
		sleep(1);
	}
	close(file_ptr);
	return 0;
}
