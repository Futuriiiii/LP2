#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_CLIENTS 2
#define BUFFER_SIZE 1024
#define MSG_SIZE 1024
#define SERVER_PORT 9999

typedef struct {
	FILE* file;
	pthread_mutex_t mutex;

} t_log_monitor;

t_log_monitor server_log;

void write_log(t_log_monitor* log, char* msg) {
	pthread_mutex_lock(&log->mutex);
	//escreve no arquivo de log
	printf("[log] %s\n", msg);
	pthread_mutex_unlock(&log->mutex);
}

int is_palindrome(char* str) {
    int len = strlen(str)-2;
    int i, j;

    for (i = 0, j = len - 1; i < j; i++, j--) {
        if (str[i] != str[j]) {
            return 0;
        }
    }
    return 1;
}

void* handle_client(void* p) {
	int client_socket = (int)p;
	
	while(1) {
		char recv_buffer[BUFFER_SIZE] = {0};

		read(client_socket, recv_buffer, BUFFER_SIZE);

		write_log(&server_log, recv_buffer);

		//recv_buffer[strlen(recv_buffer)-1] = '\0';

		int palindrome = is_palindrome(recv_buffer);

		if (palindrome) {
			char msg[MSG_SIZE];
			sprintf(msg, "%s IS A PALINDROME!\n", recv_buffer);
			write(client_socket, msg, strlen(msg));
		}
		else {
			char msg[MSG_SIZE];
			sprintf(msg, "%s IS NOT A PALINDROME!\n", recv_buffer);
			write(client_socket, msg, strlen(msg));
		}
	}
}

int main(void) {
	int server_socket;
	int client_socket[MAX_CLIENTS];
	int client_counter = 0;
	pthread_t client_threads[MAX_CLIENTS];
 	struct sockaddr_in server_params;
	struct sockaddr_in client_params[MAX_CLIENTS];
	int client_params_size[MAX_CLIENTS];

	server_params.sin_family = AF_INET;
	server_params.sin_addr.s_addr = INADDR_ANY;
	server_params.sin_port = htons(SERVER_PORT);

	server_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (server_socket < 0) {
		perror("socket() error! ");
		write_log(&server_log, "socket() error!");
		exit(EXIT_FAILURE);
	}

	int bind_ret = bind(server_socket, (struct sockaddr*)&server_params, sizeof(server_params));

	if (bind_ret < 0) {
		perror("bind() error! ");
		write_log(&server_log, "bind() error!");
		exit(EXIT_FAILURE);
	}

	int listen_ret = listen(server_socket, MAX_CLIENTS);

	if (listen_ret < 0) {
		perror("listen() error! ");
		write_log(&server_log, "listen() error!");
		exit(EXIT_FAILURE);
	}

	char banner[MSG_SIZE];
	sprintf(banner, "[server] waiting for connections on: %d\n", SERVER_PORT);
	write_log(&server_log, banner);

	while(1) {
		client_socket[client_counter] = accept(server_socket, 
						(struct sockaddr*)&client_params[client_counter],
						(socklen_t*) &client_params_size[client_counter]);

		if (client_socket[client_counter] < 0) {
			perror("accept() error! ");
			write_log(&server_log, "accept() error!");
			exit(EXIT_FAILURE);
		}
		char msg[MSG_SIZE];
		sprintf(msg, "[server] new connection: %d", client_params[client_counter].sin_addr.s_addr);
		write_log(&server_log, msg);
		
		pthread_create(&client_threads[client_counter], NULL, handle_client, 
							(void*) client_socket[client_counter]); 

		client_counter = (client_counter + 1) % MAX_CLIENTS;
	}


	return 0;

}
