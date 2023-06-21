#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define QTD_THREADS 5

long var_global = 0;

void* funcao_thread(void* args) {
	long index = (long) args;
	int counter = 0;

	while(counter < 100000) {
		//printf("[t] Sou a thread %ld (%d)\n", index, counter);
		counter++;

		//inicio secao critica
		var_global++;
		//fim secao critica
		//usleep(50);
	}
}

int main() {
	pthread_t threads[QTD_THREADS];

	printf("[m] programa iniciado\n");

	for (long i = 0; i < QTD_THREADS; i++) {
		pthread_create(&threads[i], NULL, funcao_thread, (void*)i);
	}

	for (int i = 0; i < QTD_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	printf("[m] Valor var_global = %ld\n", var_global);

	return 0;
}
