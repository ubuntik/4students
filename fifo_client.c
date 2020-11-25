#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define PAGE_SIZE 4096
#define FIFO "file.fifo"

int main(int argc, char **argv)
{
	int fd = -1;
	int rd = -1, wr = -1;
	void *buf = NULL;

	if ((buf = malloc(PAGE_SIZE)) == NULL) {
		fprintf(stderr, "OOM\n");
		return -1;
	}

	if ((fd = open(FIFO, O_RDONLY)) < 0) {
		perror("open fifo");
		return errno;
	}

	while (1) {
		if ((rd = read(fd, buf, PAGE_SIZE)) < 0) {
			perror("child read");
			return errno;
		}

		if (rd == 0)
			break;

		if ((wr = write(1, buf, rd)) < 0) {
			perror("child write");
			return errno;
		}
	}
	close (fd);
	unlink(FIFO);
	return 0;
}

