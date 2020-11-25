#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define PAGE_SIZE 4096
#define FIFO "file.fifo"

int main(int argc, char **argv)
{
	int fd = -1;
	int rd = -1, wr = -1, fh = -1;
	void *buf = NULL;

	if ((buf = malloc(PAGE_SIZE)) == NULL) {
		fprintf(stderr, "OOM\n");
		return -1;
	}

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
		return -1;
	}

	if ((fh = open(argv[1], O_RDONLY, 0666)) < 0) {
		perror("open file");
		return errno;
	}

	umask(0);
	if (mkfifo(FIFO, 0666) < 0) {
		perror("mkfifo");
		return errno;
	}

	if ((fd = open(FIFO, O_WRONLY)) < 0) {
		perror("open fifo");
		return errno;
	}

	while (1) {
		if ((rd = read(fh, buf, PAGE_SIZE)) < 0) {
			perror("parent read");
			return errno;
		}

		if (rd == 0)
			break;

		if ((wr = write(fd, buf, rd)) < 0) {
			perror("parent write");
			return errno;
		}
	}
	close (fd);
	close (fh);

	return 0;
}

