#include <fcntl.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <stdio.h>

#define PAGE_SIZE 4096

int main(int argc, char **argv)
{
	int fd[2] = {-1, -1};
	int rd = -1, wr = -1, fh = -1, chpid = -1;
	void *buf = NULL;

	if ((buf = malloc(PAGE_SIZE)) == NULL) {
		fprintf(stderr, "OOM\n");
		return -1;
	}

	if (argc != 2) {
		fprintf(stderr, "Usage: %s <file_name>\n", argv[0]);
		return -1;
	}

	if (pipe(fd) < 0) {
		perror("pipe");
		return errno;
	}

	chpid = fork();

	if (chpid > 0) {
		/* parent */
		close (fd[0]);

		if ((fh = open(argv[1], O_RDONLY, 0666)) < 0) {
			perror("open");
			return errno;
		}

		while (1) {
			if ((rd = read(fh, buf, PAGE_SIZE)) < 0) {
				perror("parent read");
				return errno;
			}

			if (rd == 0)
				break;

			if ((wr = write(fd[1], buf, rd)) < 0) {
				perror("parent write");
				return errno;
			}
		}
		close (fd[1]);
		waitpid(chpid, NULL, 0);
	} else if (chpid == 0) {
		/* child */
		close (fd[1]);

		while (1) {
			if ((rd = read(fd[0], buf, PAGE_SIZE)) < 0) {
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
		close (fd[0]);
	} else {
		perror("fork");
		return errno;
	}

	return 0;
}

