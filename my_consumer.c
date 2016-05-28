#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
int main(int argc, char* argv[]){
	int counter=0;
	int number_of_bytes_to_read = atoi(argv[1]);
	char* buff = (char*)malloc(number_of_bytes_to_read*sizeof(char));
	int file_ptr = open("/dev/chardev_misc", O_RDONLY);
	if(file_ptr == -1){
		printf("Error: Could not open the file!\n");
		exit(0);
	}
	for(;;){
		printf("calling read...Counter %d \n",counter++);
		int number_of_bytes_read = read(file_ptr, &buff, number_of_bytes_to_read);
		if(number_of_bytes_read == -1){}
		printf("String read = %s\n",&buff);
		sleep(1);
	}
	close(file_ptr);
	return 0;
}
