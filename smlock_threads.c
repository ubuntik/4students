#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <time.h>

#ifdef MUTEX
pthread_mutex_t m;
#else
pthread_rwlock_t m;
#endif

int a = 0;

void *function(void *data)
{
	for (int i = 0; i < 10000; i++) {
#ifdef MUTEX
		pthread_mutex_lock(&m);
#else
		pthread_rwlock_wrlock(&m);
#endif
		a++;
#ifdef MUTEX
		pthread_mutex_unlock(&m);
#else
		pthread_rwlock_unlock(&m);
#endif
	}
	pthread_exit(NULL);
}

int main()
{
	pthread_t ths[N];

#ifdef MUTEX
	pthread_mutex_init(&m, NULL);
#else
	pthread_rwlock_init(&m, NULL);
#endif


	struct timespec start, end;
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &start);
	for (int i = 0; i< N; i++) {
		if (pthread_create(&ths[i], NULL, function, NULL) != 0) {
			perror("w thread create");
			return errno;
		}
	}

	for (int i = 0; i< N; i++) {
		if (pthread_join(ths[i], NULL) != 0) {
			perror("r thread join");
		}
	}
	clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &end);
	long seconds = end.tv_sec - start.tv_sec;
	long nanoseconds = end.tv_nsec - start.tv_nsec;
	double elapsed = seconds + nanoseconds*1e-9;
	printf("Time: %.3f seconds.\n", elapsed);

#ifdef MUTEX
	pthread_mutex_destroy(&m);
#else
	pthread_rwlock_destroy(&m);
#endif

	return 0;
}

