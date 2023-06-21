/*Escreva um programa em C que simule um sistema de detecção de vazamento em uma represa.
O programa deve usar threads POSIX e espera ocupada para detectar mudanças em uma variável que representa
o nível de água da represa (valor inicial: 1000 litros).
O nível de água é modificado por um conjunto de 5 threads. Cada thread receberá um valor aleatório do quanto incrementará
no nível do reservatório (entre 10 e 100 litros) e dormirá um tempo aleatório (entre 0 e 300ms) entre cada incremento.
Quando o nível de água atingir 5000 litros, essas threads devem ser bloqueadas até que a thread principal garanta que
o nível do reservatório foi reestabelecido para 1000 litros (e depois dormir 200ms).
Deve ser utilizada espera ocupada em todos os casos e funções atômicas para modificação do nível do reservatório.*/

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdatomic.h>
#include <time.h>

#define QUANT_THREADS 5
#define INCREMENTO_MAX_AGUA 100
#define SLEEP_MAX 300
#define LIMITE_RESERVATORIO 5000

atomic_int nivelAgua = ATOMIC_VAR_INIT(1000); // inicia uma variável atomica para guardar o nivel da agua
int blocked = 0; // flag para espera ocupada

void *encheReservatorio(void *thread_id){
    int id = *((int *) thread_id); // cria uma variável q armazena o ID da thread
    srand(time(NULL) + id); // usa o ID como seed para um número aleatório

    while (1) {
        int incremento = rand() % (INCREMENTO_MAX_AGUA + 1 - 10) + 10; // cria a variavel de encremento e atribui um valor aleatório de quanto vai encher o reservatorio
        int dorme = rand() % (SLEEP_MAX + 1); // cria a variavel dorme e atribui um valor aleatório de quanto vai dormir depois de adcionar a agua no reservatório
        usleep(dorme * 1000); // dorme o tempo aleatorio (em microsegundos)

        atomic_fetch_add(&nivelAgua, incremento); // faz o incremento atomico da variável que guarda o nivel da agua

        if (atomic_load(&nivelAgua) >= LIMITE_RESERVATORIO) { // verifica se o nivel da agua atingiu ou passou do limite
            while (blocked) {
                // espera ocupada até que o reservatório seja esvaziado
            }
            break;
        }
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[QUANT_THREADS];
    int thread_ids[QUANT_THREADS];

    for (int i = 0; i < QUANT_THREADS; i++) { // for para criar as 5 threads e fazer cada uma encher o reservatorio
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, encheReservatorio, (void *) &thread_ids[i]);
    }

    while (1) {
        if (atomic_load(&nivelAgua) >= LIMITE_RESERVATORIO) { // if para verificar se o nivel de agua passou ou igualou ao limite
            blocked = 1; // se sim, bloqueia as threads

            atomic_store(&nivelAgua, 1000); // esvazia o reservatório ate 1000

            usleep(200000); // dorme os 200 ms depois de esvaziar o reservatório

            blocked = 0; // desbloqueia as threads
            
        }
    }

    for (int i = 0; i < QUANT_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}

