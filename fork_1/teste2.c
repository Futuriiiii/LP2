#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(){
    printf("ANTES\n");
    int r = fork();

    if(r == 0){
      printf("CLONADO![%d]\n", getpid());
      printf("PID do orginal: %d\n", getppid());
      while(1);
    }else{
      printf("ORIGINAL\n");
      printf("PID do clonado: %d\n", r);
      printf("PID do original: %d\n", getpid());
    }

    return 0;
}
