#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define QTD_THREADS 50

long var_global = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* funcao_thread(void* args) {
	long index = (long) args;
	int counter = 0;

	while(counter < 100000) {
		//printf("[t] Sou a thread %ld (%d)\n", index, counter);
		counter++;
		printf("[t-%ld] antes da SC\n", index);
		pthread_mutex_lock(&mutex);
		//inicio secao critica
		var_global++;
		//printf("[t-%ld] %ld\n", index, var_global);
		//usleep(2000);
		//sleep(1);
		//fim secao critica
		pthread_mutex_unlock(&mutex);
		printf("[t-%ld] depois da SC\n", index);
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
