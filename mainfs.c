#include "toyfs.h"
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <linux/types.h>
#include <linux/stat.h>

int main(int argc, char *argv[])
{
	printf("Running Main File: mainfs.c\n");
	int file_d;
	int error;
	struct toyfs_super_block sb;

	if(argc != 2){
		printf("Running: Toy FS <device>\n");
		return -1;
	}

	//opens file with rd/wr permissions	
	file_d = open(argv[1], O_RDWR);
	//system call error checking
	if(file_d == -1) {
		printf("Error opening device.\n");
		return -1;
	}

	sb.block_size = 4096;
	sb.num_free_blocks = 0;	

	error = write(file_d, (char *)&sb, sizeof(sb));

	if(error != 4096)
		printf("bytes written [%d] are not equal to the default block size\n",(int)error);
	else
		printf("Super block written successfully");

	close(file_d);
	return 0;
}
