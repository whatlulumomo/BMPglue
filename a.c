#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
int main(void){
	int fd = open("mario_16_bit.bmp",O_RDONLY);
	int w = 0;
	lseek(fd, 0x12, SEEK_SET); read(fd, &w, 4); 
	printf("%d\n",w);
	return 0;
}