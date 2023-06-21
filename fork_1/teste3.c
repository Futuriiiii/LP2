#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(){
    
    printf("ANTES\n");
    int x;
    scanf("%d", &x);
    int pid_0 = getpid();

    for(int i = 0; i < x; i++){
        if(getpid() == pid_0){
         fork();
        }
   }
}
