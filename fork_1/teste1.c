#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(){
    printf("alo mundo\n");
    sleep(10);

    /*for(int i = 0; i < 1000; i++){
        printf("[%d] alo\n", (i+1));

    }
    */
   /*int i = 0;
   while(1){    
        i++;
        printf("[%d] alo\n", (i+1));
   }*/
   int x;
   printf("digite x:");
   scanf("%d", &x);
    return 0;
}
