#include <stdio.h>
#include <unistd.h>
#include <errno.h>

int main(int argc, char **argv, char **envp)
{
	if (argc < 2) {
		fprintf(stderr, "Usage: %s <program name> <args>\n", argv[0]);
		return -1;
	}

	char *new_argv [argc];
	for (int i = 1; i < argc; i++)
		new_argv[i - 1] = argv[i];
	new_argv[argc - 1] = NULL;

	int sz = 0;
	while (envp[++sz] != NULL);

	char *new_envp [sz + 1];
	for (int i = 0; i < sz; i++)
		new_envp[i] = envp[i];
	new_envp[sz] = "A=B";
	new_envp[sz + 1] = NULL;

	execve(argv[1], new_argv, new_envp);
	perror("exec");
	return -1;
}

