/*Faça um programa em C/C++ que implemente uma solução para o problema do buffer compartilhado.
Com um buffer compartilhado de 64 chars, o Programa deverá implementar duas funções:
 
	- produtor() - gera um caractere alfanumérico aleatório e o deposita
	no buffer; exibe o valor e a posição no buffer na tela; dorme 300ms;
	repete;
 
	- consumidor() - captura um dos valores disponíveis no buffer 
	compartilhado e armazena em um arquivo; dorme um tempo aleatório 
	entre 50ms e 200ms; repete;
 
A função produtor deverá ser realizada por uma thread; já a função consumidor deverá ser realizada por três (5) threads.
Cada consumidor armazena as informações em um buffer diferente (de tamanho 64) e todas as threads devem armazenar
todos os valores gerados pelo produtor. 
 
Use semáforos (biblioteca semaphore.h) para sincronizar o acesso ao buffer  compartilhado,
de forma que todos os números gerados pelo produtor sejam consumidos uma vez, evitando qualquer problema ao ler ou escrever no buffer.
Quando o buffer do consumidor estiver completo, o programa deverá exibir o seu contaúdo. Quando o buffer for consumido 2 vezes
(ou seja, quando o produtor tiverem gerado 128 caracteres e estes forem consumidos por TODOS os consumidores) finalize o programa.*/

#include <iostream>
#include <thread>
#include <semaphore.h>
#include <random>
#include <chrono>
#include <fstream>

using namespace std;

const int BUFFER_SIZE = 64;
const int NUM_CONSUMERS = 5;
const int MAX_SLEEP_PRODUCER = 300;
const int MIN_SLEEP_CONSUMER = 50;
const int MAX_SLEEP_CONSUMER = 200;

char buffer[BUFFER_SIZE];
sem_t emptySlots;
sem_t usedSlots;
sem_t bufferMutex;
ofstream outputFile;

void produtor() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(48, 122);  // Caracteres alfanuméricos na tabela ASCII

    for (int i = 0; i < 128; ++i) {
        char character = dist(gen);

        sem_wait(&emptySlots);
        sem_wait(&bufferMutex);

        int position = -1;
        for (int j = 0; j < BUFFER_SIZE; ++j) {
            if (buffer[j] == '\0') {
                buffer[j] = character;
                position = j;
                break;
            }
        }

        sem_post(&bufferMutex);
        sem_post(&usedSlots);

        cout << "Produtor: " << character << " (Posição: " << position << ")" << endl;

        this_thread::sleep_for(chrono::milliseconds(MAX_SLEEP_PRODUCER));
    }
}

void consumidor(int id) {
    char consumerBuffer[BUFFER_SIZE];
    int consumedCount = 0;

    while (consumedCount < 128) {
        sem_wait(&usedSlots);
        sem_wait(&bufferMutex);

        int position = -1;
        for (int i = 0; i < BUFFER_SIZE; ++i) {
            if (buffer[i] != '\0') {
                consumerBuffer[i] = buffer[i];
                buffer[i] = '\0';
                position = i;
                ++consumedCount;
            }
        }

        sem_post(&bufferMutex);
        sem_post(&emptySlots);

        cout << "Consumidor " << id << ": " << consumerBuffer[position] << " (Posição: " << position << ")" << endl;

        outputFile << consumerBuffer[position];

        int sleepTime = rand() % (MAX_SLEEP_CONSUMER - MIN_SLEEP_CONSUMER + 1) + MIN_SLEEP_CONSUMER;
        this_thread::sleep_for(chrono::milliseconds(sleepTime));
    }

    outputFile << endl;
}

int main() {
    sem_init(&emptySlots, 0, BUFFER_SIZE);
    sem_init(&usedSlots, 0, 0);
    sem_init(&bufferMutex, 0, 1);
    outputFile.open("output.txt");

    thread produtorThread(produtor);
    thread consumidorThreads[NUM_CONSUMERS];

    for (int i = 0; i < NUM_CONSUMERS; ++i) {
        consumidorThreads[i] = thread(consumidor, i);
    }

    produtorThread.join();
    for (int i = 0; i < NUM_CONSUMERS; ++i) {
        consumidorThreads[i].join();
    }

    outputFile.close();

    sem_destroy(&emptySlots);
    sem_destroy(&usedSlots);
    sem_destroy(&bufferMutex);

    return 0;
}

