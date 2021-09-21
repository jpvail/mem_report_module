
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <unistd.h>
#include <sys/ioctl.h>


#include "mem_report.h"


int run_cmd(int fd, int pid)
{
	int err = 0; 

	if (ioctl(fd, MEM_REPORT_RUN, &pid) < 0) {
		fprintf(stderr, "*** failed to execute ioctl command\n");
		err |= 1;
	}

	return err;
}

int main(int argc, char *argv[])
{
        int fd;
	int err = 0;
	
        fd = open("/dev/mem_report", O_RDWR);
        if (fd < 0) {
                fprintf(stderr, "*** failed to open /dev/mem_report\n");
                return 1;
        }

	err |= run_cmd(fd, atoi(argv[1]) );

	close(fd);
        return err;
}
