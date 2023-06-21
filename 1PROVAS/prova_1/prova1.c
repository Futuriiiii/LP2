#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <sys/types.h>
#include <sys/signal.h>

long soma_nota_th = 0;
long soma_nota_pr = 0;
int rnd;

void* notas_thread(void* p)
{
    long val = rand() % 100;
    while(1) //loop infinito que vai ser quebrado quando o valor da nota gerada aleatoriamente for válido
    {
        if(val >= 0 && val <= rnd)
        {
            soma_nota_th+= val;
            break;
        }else
        {
            val = rand() % 100;
        }
    }
	pthread_exit((void*)val);
}

void notas_processos(int i)
{
	long val = rand() % 100;
    while(1) //loop infinito que vai ser quebrado quando o valor da nota gerada aleatoriamente for válido
    {
        if(val >= 0 && val <= rnd)
        {
            soma_nota_pr+= val;
            break;
        }else
        {
            val = rand() % 100;
        }
    }
}

int main()
{
    int q_th, q_pr, pid_original, ret_fork;
    int* pid_clones;
    double tempo_th, tempo_pr;

    tempo_th = tempo_pr = 0;

    scanf("%d", &q_th);
    scanf("%d", &q_pr);
    scanf("%d", &rnd);

    //parte de threads
    pthread_t ths[q_th];
    void* ret[q_th];

    clock_t tempo_th_inicio = clock(); //inicio da contagem do tempo de criação e execução para as threads

	for(long i = 0; i < q_th; i++)
    {
        pthread_create(&ths[i], 0,notas_thread,(void*)i); //criação da thread para executar a função de criação das notas
    }
    for(int i = 0; i < q_th; i++)
    {
	    pthread_join(ths[i], &ret[i]); // execução da thread
	}

    clock_t tempo_th_fim = clock(); //fim da contagem do tempo das threads
    tempo_th += (double)(tempo_th_fim - tempo_th_inicio) / CLOCKS_PER_SEC; //obtenção do resultado em segundos

    //parte de processos
    pid_original = getpid();

    pid_clones = (int*)malloc(sizeof(int)*q_pr);

    clock_t tempo_pr_inicio = clock(); //inicio da contagem do tempo de criação e execução para os processos

    for(int i = 0; i < q_pr; i++)
    {
	    if(getpid() == pid_original) //verifica se estou no processo original
        {
			ret_fork = fork();  //criação de processo clone do original
			notas_processos(i); //executar a função de criação das notas
			if(getpid() == pid_original) //verifica se ainda estou no processo original
            { 
				pid_clones[i] = ret_fork; //armazena o retorno do fork do processo clone em um array para finalizar ele depois
			}
		}
	}

    if(getpid() == pid_original) //verifica se estou no processo original
    {
        for(int i = 0; i < q_pr; i++)
    {
        kill(pid_clones[i], SIGHUP);  //finaliza os processos utilizando o pid armazenado no array.
    }
	}else
    {
		kill(getpid(), SIGHUP);
	}

    clock_t tempo_pr_fim = clock(); //fim da contagem do tempo dos processos
    tempo_pr += (double)(tempo_pr_fim - tempo_pr_inicio) / CLOCKS_PER_SEC; //obtenção do resultado em segundos

    //exibindo os resultados

    printf("Threads geradas: %d (%lfs)\n", q_th, tempo_th);
    printf("Processos geradas: %d (%lfs)\n", q_pr, tempo_pr);
    printf("Média dos valores gerados pelas threads: %.2f\n", (float)(soma_nota_th)/q_th);
    printf("Média dos valores gerados pelos processos: %.2f\n", (float)(soma_nota_pr)/q_pr);

}