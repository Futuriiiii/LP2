/* PROGRAMA QUE CRIA UM FILHO, FAZ ELE EXECUTAR OUTRO PROGRAMA E DPS ENCERRA O FILHO */

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

#define EXEC_PATH "/home/rafa/LP2/aula_3/loop"

int main()
{
    printf("[ORIGINAL]Exemplo exec()\n");

    int frk = fork();

    if(frk < 0)
    {
        printf("fork() error\n");
        exit(0);
    }

    if(frk == 0)
    {
        char *args[] = {"loop", "loop", NULL};
        printf("[CLONADO] Vou executar o programa\n");
        int exec_return = execv(EXEC_PATH, args);
        printf("[CLONADO] Depois do exec(): %d\n", exec_return);
    } else
    {
        int ret_val;
        waitpid(frk, &ret_val, 0);
        printf("\n[ORIGINAL] Clonado finalizado\n");
    }

    return 0;
}