#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

void print_for_file(char *file)
{
	fprintf(stdout, "\t%s\n", file);
}


void print_for_dir(char *dir)
{
	DIR* dptr = NULL;
	struct dirent *ndir;
	struct stat *pinfo = NULL;
	char fname[256];

	if ((pinfo = (struct stat *)malloc(sizeof(struct stat))) == NULL) {
		fprintf(stderr, "OOM\n");
		return;
	}

	/* print dir name */
	fprintf(stdout, "%s:\n", dir);
	/* print info for each file in dir*/
	if ((dptr = opendir(dir)) == NULL) {
		fprintf(stderr, "Error opendir\n");
		return;
	}

	while((ndir = readdir(dptr)) != NULL) {
		/* ignore files start on . */
		/* strcmp */
		print_for_file(ndir->d_name);
	}

	rewinddir(dptr);

	while((ndir = readdir(dptr)) != NULL) {
		/* ignore dir start on . */
		if (!strcmp(ndir->d_name, "."))
			continue;

		if (!strcmp(ndir->d_name, ".."))
			continue;

		sprintf(fname, "%s/%s", dir, ndir->d_name);
		if (stat(fname, pinfo) < 0) {
			perror("Error stat");
			continue;
		}
		if (S_ISDIR(pinfo->st_mode)) {
			print_for_dir(fname);
			continue;
		}
	}

	free(pinfo);
	closedir(dptr);
}


int main(int argc, char **argv)
{
	int i = 0;
	char *cwd = NULL;
	if (argc == 1) {
		/* print for current work directory */
		int size = pathconf(".", _PC_PATH_MAX);
		cwd = (char *)malloc(size);
		if (cwd == NULL) {
			fprintf(stderr, "OOM\n");
			return -1;
		}
		if (!getcwd(cwd, size)) {
			fprintf(stderr, "Error get cwd\n");
			return -1;
		}
		print_for_dir(cwd);
		free(cwd);
	} else {
		for (i = 1; i < argc; i++) {
			/* print for each argv[i] */
			print_for_dir(argv[i]);
		}
	}

	return 0;
}

