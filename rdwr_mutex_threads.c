#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>

#define N 1000

pthread_mutex_t r;
pthread_mutex_t w;

int a = 0;

void *writer_function(void *data)
{
	struct timespec t;
	t.tv_sec = 0;

	for (int i = 0; i < N; i++) {
		t.tv_nsec = (random() % 10) * 1000000;
		pthread_mutex_lock(&w);
		nanosleep(&t, NULL);
		a++;
		pthread_mutex_unlock(&r);
	}
	pthread_exit(NULL);
}

void *reader_function(void *data)
{
	for (int i = 0; i < N; i++) {
		pthread_mutex_lock(&r);
		printf("%d\n", a);
		pthread_mutex_unlock(&w);
	}
	pthread_exit(NULL);
}

int main()
{
	pthread_t th_r;
	pthread_t th_w;

	pthread_mutex_init(&r, NULL);
	pthread_mutex_init(&w, NULL);

	pthread_mutex_lock(&r);

	if (pthread_create(&th_w, NULL, writer_function, NULL) != 0) {
		perror("w thread create");
		return errno;
	}
	if (pthread_create(&th_r, NULL, reader_function, NULL) != 0) {
		perror("r thread create");
		return errno;
	}

	if (pthread_join(th_w, NULL) != 0) {
		perror("w thread join");
	}
	if (pthread_join(th_r, NULL) != 0) {
		perror("r thread join");
	}

	printf("master done\n");

	pthread_mutex_destroy(&r);
	pthread_mutex_destroy(&w);

	return 0;
}

