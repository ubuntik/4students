#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define FIFO "file.fifo"

int main(int argc, char **argv)
{
	int fd = -1;
	int size = 0;

	umask(0);
	if (mkfifo(FIFO, 0666) < 0) {
		perror("mkfifo");
		return errno;
	}

	if ((fd = open(FIFO, O_RDWR | O_NONBLOCK)) < 0) {
		perror("open fifo");
		unlink (FIFO);
		return errno;
	}

	while (write(fd, "a", 1) == 1)
		size++;

	if (errno != EAGAIN) {
		perror("write");
		close (fd);
		unlink (FIFO);
		return errno;
	}
	printf("FIFO size is %d\n", size);
	close (fd);
	unlink (FIFO);
	return 0;
}

