#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>

/* barreira - forma geral
 * process Worker[i=1 to n] {
 *	while(true) {
 *		job();
 *		ponto de embarreiramento;
 *	}
 * }
 */

#define QTD_THREADS 5
#define SLEEP_MAX 6
#define MAX_REPEAT 5

pthread_barrier_t my_barrier;

void* job_function(void* p) {	
	long index = (long)p;
	int counter = 0;

	//int _sleep  = ((int)index) + 1;

	while (counter < MAX_REPEAT) {
		int _sleep = rand() % SLEEP_MAX;
		_sleep++;
		
		printf("[%ld] job(%d)\n", index, _sleep); 
		//simula job
		sleep(_sleep);
		//end simula job
		printf("[%ld] finished job() - barrier\n", index);
		pthread_barrier_wait(&my_barrier);
		printf("[%ld] left barrier\n", index);
		counter++;
	}
}

int main(void) {
	time_t t;
	pthread_t threads[QTD_THREADS];
	pthread_barrier_init(&my_barrier, NULL, QTD_THREADS);

	srand(time(&t));

	for(long i = 0; i < QTD_THREADS; i++) {
		pthread_create(&threads[i], NULL, 
				job_function, (void*)i);
	}

	for(int i = 0; i < QTD_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	pthread_barrier_destroy(&my_barrier);

	printf("[main] finished\n");

	return 0;
}
