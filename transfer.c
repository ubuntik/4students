#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <pthread.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>

#define L 10
#define T 1

#define h 1
#define t 0.01

#define PRINT 5

#define ALL_LEN L/h + 2
#define LEN L/h
#define TIME T/t

#define COEFF  t * (0.5 / h)

int main()
{
	double *u = (double *)calloc(ALL_LEN, sizeof(double));
	double *u_check = (double *)calloc(ALL_LEN, sizeof(double));
	u[0] = 1;


	/* add parallel ver*/
	// clock start
	// create threads

	// ----------|------------|-----------|---------

	// clock stop
	//
	// time(num of threads)
	//
	// compare the result
	for (int i = 0; i < TIME; i++) {
		for (int m = 1; m < LEN; m++)
			u_check[m] = 0.5 * (u[m + 1] + u[m - 1]) - COEFF * (u[m + 1] - u[m - 1]);
		u_check[0] = 1;
		u_check[ALL_LEN - 1] = u_check[ALL_LEN - 2];

		if (i % PRINT == 0) {
			for (int m = 0; m <= LEN; m++)
				printf("%g ", u_check[m]);
			printf("\n");
		}

		memcpy(u, u_check, ALL_LEN * sizeof(double));
	}

	free(u);
	free(u_check);

	return 0;
}

