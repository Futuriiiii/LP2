#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pthread.h>

#define QUANT 15

void* funcao_thread (void* p)
{
    long index =(long) p;
    printf("sou a thread %ld\n", index);
    sleep(3);
    printf("tchau\n");
}

int main()
{
    pthread_t threads[QUANT];
    for(long i = 0; i < QUANT; i++)
    {
        pthread_create(&threads[i], NULL, funcao_thread, (void*) i);
    }
    for(int i = 0; i < QUANT; i++)
    {
        pthread_join(threads[i], NULL);
    }
    printf("main saindo...");

    return 0;
}