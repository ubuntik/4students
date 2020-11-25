#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>

#define N 5

typedef struct output {
	unsigned long int id;
} output;

typedef struct input {
	int index;
	output *out;
} input;

void *thread_function(void *data)
{
	input *in = (input *)data;
	output out;
	in->out->id = (unsigned long int)pthread_self();
	printf("thread %d: %li\n", in->index, (unsigned long int)pthread_self());
	pthread_exit((void *)in->out);
}

int main()
{
	int i = 0;
	input in[N];
	pthread_t ths[N];
	output *res;

	for (i = 0; i < N; i++) {
		in[i].index = i;
		in[i].out = (output *) malloc(sizeof(output));
		if (pthread_create(&ths[i], NULL, thread_function, (void *)(&in[i])) != 0) {
			perror("thread create");
			return errno;
		}
	}

	for (i = 0; i < N; i++) {
		if (pthread_join(ths[i], (void **)&res) != 0) {
			perror("thread join");
			continue;
		}
		printf("master: %d - %li\n", i, res->id);
		free(res);
	}

	return 0;
}

