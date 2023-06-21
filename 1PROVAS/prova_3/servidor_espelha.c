// Nome: Carlos Rafael Torres Miranda Novack
// Mat.: 20210066961
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>

#define MAX_CLIENTS 6
#define BUFFER_SIZE 1024
#define MSG_SIZE 1024
#define SERVER_PORT 9999

sem_t semaphore;

int shutdown_server = 0;

typedef struct {
	FILE* file;
	pthread_mutex_t mutex;
} t_log_monitor;

t_log_monitor server_log;

void write_log(t_log_monitor* log, char* msg) {
    pthread_mutex_lock(&log->mutex); // mutex_lock para garantir que apenas 1 thread altere o arquivo por vez
	
	fprintf(server_log.file, "[log] %s\n",msg); // escreve no arquivo de log

    pthread_mutex_unlock(&log->mutex); // libera o arquivo para outra thread
}

void mirror_string(char* str) {
    int start = 0; // marca o inicio da string
	int end = strlen(str) - 1; // marca o final da string

    while (start < end) { // enquanto o inicio for menor q o final, repete 
        char aux = str[start]; // auxiliar para guardar o "primeiro" caractere e ajudar no espelhamento da string
        str[start] = str[end]; // define o "primeiro" caractere como o "ultimo"
        str[end] = aux; // define o "ultimo" caractere como o "primeiro"

        start++; // incrementa o inicio
        end--; // decrementa o final
    }
}

void* handle_client(void* p) {
	int client_socket = (long)p; // guarda o socket do cliente que recebeu como parametro 
	
	while(1) {
		char recv_buffer[BUFFER_SIZE] = {0}; // limpa o buffer para garantir que não vai ler algo indevido

		read(client_socket, recv_buffer, BUFFER_SIZE); // le a mensagem do cliente
		if(recv_buffer[strlen(recv_buffer) - 1] == '\n') { // remove o '\n' da string recebida (será executado esse if quando a string eh recebida pelo cliente_espelha.c)
			recv_buffer[strlen(recv_buffer) - 1] = '\0';
		} else {
			recv_buffer[strlen(recv_buffer) - 2] = '\0'; // remove o '\n' e o '\r' da string recebida (será executado esse else quando a string eh recebida diretamente do terminal)
		}

        if(strstr(recv_buffer, "EXIT") != NULL) { // verifica se o cliente escreveu "EXIT"
			write_log(&server_log, recv_buffer); // escreve o EXIT no log
            break; // o loop é encerrado e sua conexão finalizada
        } else if(strstr(recv_buffer, "SHUTDOWN") != NULL) { // verifica se o cliente escreveu "SHUTDOWN"
			write_log(&server_log, recv_buffer); // escreve o SHUTDOWN no log
            sem_wait(&semaphore); // espera o semaforo

            shutdown_server++; // -----SEÇÂO CRITICA----- incrementa uma variável global

            sem_post(&semaphore); // libera o semaforo

            if(shutdown_server >= 3) { // se a variável for maior ou igual a 3, encerra o servidor
				fprintf(server_log.file, "[log] SHUTTING DOWN SERVER\n"); // escreve no arquivo de log
                fclose(server_log.file); // fecha o arquivo
                exit(0); // encerra o servidor
            }
			break;
        } else { // se o cliente mandar qualquer outra string, ela será espelhada
            write_log(&server_log, recv_buffer); // escreve no log a string original

            mirror_string(recv_buffer); // inverte a string

            write_log(&server_log, recv_buffer); // escreve o no log a string invertida

            write(client_socket, recv_buffer, strlen(recv_buffer)); // retorna para o cliente a strig invertida
			write(client_socket, "\n", strlen("\n")); // pula a linha para organizar as informações exibidas ao cliente
        }
    }
    close(client_socket); // fecha o socket do cliente
}

int main(void) {
    sem_init(&semaphore, 0, 1);
	int server_socket;
	long client_socket[MAX_CLIENTS];
	int client_counter = 0;
	pthread_t client_threads[MAX_CLIENTS];
 	struct sockaddr_in server_params;
	struct sockaddr_in client_params[MAX_CLIENTS];
	int client_params_size[MAX_CLIENTS];

    server_log.file = fopen("log.txt", "w"); // cria o arquivo de log chamado log.txt para escrita
	if (server_log.file == NULL) { // verifica se o arquivo não foi aberto
		perror("fopen() error! ");
		exit(EXIT_FAILURE);
	}

	server_params.sin_family = AF_INET;
	server_params.sin_addr.s_addr = INADDR_ANY;
	server_params.sin_port = htons(SERVER_PORT);

	server_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (server_socket < 0) {
		perror("socket() error! ");
		write_log(&server_log, "socket() error!"); // escreve o erro no log
		exit(EXIT_FAILURE);
	}

	int bind_ret = bind(server_socket, (struct sockaddr*)&server_params, sizeof(server_params));

	if (bind_ret < 0) {
		perror("bind() error! ");
		write_log(&server_log, "bind() error!"); // escreve o erro no log
		exit(EXIT_FAILURE);
	}

	int listen_ret = listen(server_socket, MAX_CLIENTS);

	if (listen_ret < 0) {
		perror("listen() error! ");
		write_log(&server_log, "listen() error!"); // escreve o erro no log
		exit(EXIT_FAILURE);
	}

	char banner[MSG_SIZE];
	sprintf(banner, "[server] waiting for connections on: %d\n", SERVER_PORT);
	write_log(&server_log, banner); // escreve no log que esta esperando conexões

	while(1) {
		client_socket[client_counter] = accept(server_socket, 
						(struct sockaddr*)&client_params[client_counter],
						(socklen_t*) &client_params_size[client_counter]);

		if (client_socket[client_counter] < 0) {
			perror("accept() error! ");
			write_log(&server_log, "accept() error!"); // escreve o erro no log
			exit(EXIT_FAILURE);
		}
		char msg[MSG_SIZE];
		sprintf(msg, "[server] new connection: %d", client_params[client_counter].sin_addr.s_addr);
		write_log(&server_log, msg); // escreve no log que houve uma nova conexão no servidor
		
		pthread_create(&client_threads[client_counter], NULL, handle_client, 
							(void*) client_socket[client_counter]); // cria as threads

		client_counter = (client_counter + 1) % MAX_CLIENTS; // verifica se o numero de clientes chegou no limite
	}
	return 0;
}
