#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define QTD_THREADS 50

long var_global = 0;
int lock = 0;

void* funcao_thread(void* args) { 
        long index = (long) args;
        int counter = 0;

        while(counter < 100000) {
                //printf("[t] Sou a thread %ld (%d)\n", index, counter);
                counter++;
                //printf("[t-%ld] antes da SC\n", index);
		while(__atomic_test_and_set(&lock,0)); // (P.E.)
		//secao critica
		var_global++;
		//printf("[t-%ld] %ld\n", index, var_global);
		//secao critica
		lock = 0; //P.S.
		printf("[t-%ld] depois da SC\n", index);
	}
}

int main(void) {
	pthread_t threads[QTD_THREADS];

	for (long i = 0; i < QTD_THREADS; i++) {
		pthread_create(&threads[i], NULL, funcao_thread, (void*) i);
	}

	for (int i = 0; i < QTD_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	printf("[m] var_global = %ld\n", var_global);

	return 0;
}
