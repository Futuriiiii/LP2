/* PROGRAMA QUE CRIA DOIS PROCESSOS
    - O processo original deve exibir uma mensagem que o indentifique, seu PID e o PID do processo clonado
    - O processo clonado deve exibir uma mensagem que o indentifique, seu PID e o PID do processo ascendente e
    deve solicitar um numero inteiro para posterior exibição
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){

    int fork_return;

    fork_return = fork();

    if(fork_return == -1){
        printf("fork() error\n");
        exit(-1);
    } else if(fork_return == 0){
        int var;
        printf("[Processo Clonado] ");
        printf("meu pid: %d\n", getpid());
        printf("[Processo Clonado] ");
        printf("pid ascendente: %d\n", getppid());

        printf("[Processo Clonado] Digite um inteiro: ");
        scanf("%d", &var);
        printf("[Processo Clonado] Valor inserido: %d\n", var);

        exit(var);
    } else{
        int wait_return;
        printf("[Processo Original] ");
        printf("meu pid: %d\n", getpid());
        printf("[Processo Original] ");
        printf("pid descendente: %d\n", fork_return);

        printf("[Processo Original] Aguardando clonado\n");
        wait(&wait_return);
        printf("[Processo Original] Clonado saiu %d\n",WEXITSTATUS(wait_return));
        printf("[Processo Original] Clonado finalizado\n");
    }

    return 0;
}