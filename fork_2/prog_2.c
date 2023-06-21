/*
Programa que le um numero e cria esta quantidade de processos.
Os processos criados devem existir por 15 segundos.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define MAX_SLEEP_TIME 10

int* pid_array;

int main(){
    int quant;
    int pid_original;

    pid_original = getpid();

    printf("Digite um numero inteiro: ");
    scanf("%d", &quant);
    printf("Criando %d processos...", quant);

    pid_array = (int*)malloc(sizeof(int)*quant);
    
    fflush(stdout);
    for(int i = 0; i < quant; i++){
        if(getpid() == pid_original){
            int fork_return;
            fork_return = fork();
            if(getpid() != pid_original){
                printf("[CLONE] %d\n", getpid());
                int r_sleep;
                time_t t;
                srand(time(&t));
                r_sleep = rand() % getpid() % MAX_SLEEP_TIME;
                r_sleep++;
                printf("[CLONE] %ds\n", r_sleep);
                sleep(r_sleep);
                printf("[CLONE] fim\n");
                exit(r_sleep);
            }else{
                printf("[ORIGINAL] array setup\n");
                pid_array[i] = fork_return;
        }
        }

    }

    if(getpid() == pid_original){
        for (int i = 0; i < quant; i++) {
			printf("[ORIGINAL] %d\n", pid_array[i]);
		}

        printf("[ORIGINAL] wait\n");
        int clone_exit;

        for(int i = 0; i < quant; i++){
            waitpid(pid_array[i], &clone_exit, 0);
            printf("[ORIGINAL] exit: %d\n", WEXITSTATUS(clone_exit));
        }
        sleep(1);
        printf("[ORIGINAL] saindo....\n");
    }

    return 0;
}