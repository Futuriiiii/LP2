// Nome: Carlos Rafael Torres Miranda Novack
// Mat.: 20210066961
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1" // endereço IP do servidor (localhost)
#define SERVER_PORT 9999      // porta do servidor (deve ser igual a porta informada no servidor_espelha.c)

#define BUFFER_SIZE 1024
#define MSG_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;

    // cria o socket do cliente
    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        perror("Falha ao criar o socket"); // exibe um erro caso nao tenha sido possível criar o socket do cliente
        exit(EXIT_FAILURE);
    }

    // configura o endereço do servidor
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    if (inet_pton(AF_INET, SERVER_IP, &(server_addr.sin_addr)) <= 0) {
        perror("Falha ao configurar o endereço do servidor"); // exibe um erro caso nao tenha sido possível configurar o endereço do servidor
        exit(EXIT_FAILURE);
    }

    // conecta ao servidor
    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Falha ao conectar com o servidor"); // exibe um erro caso nao tenha sido possível conectar com o servidor
        exit(EXIT_FAILURE);
    }
    printf("Conexão com o servidor bem-sucedida\n");

    // Loop para enviar mensagens ao servidor
    while (1) {
        char send_buffer[MSG_SIZE] = {0};
        char recv_buffer[BUFFER_SIZE] = {0};

        printf("Digite uma mensagem para enviar ao servidor: ");
        fgets(send_buffer, MSG_SIZE, stdin);

        // envia a mensagem ao servidor
        if (send(client_socket, send_buffer, strlen(send_buffer), 0) < 0) {
            perror("Falha ao enviar a mensagem ao servidor");  // exibe um erro caso nao tenha sido possível enviar a mensagem ao servidor
            exit(EXIT_FAILURE);
        }

        if (strstr(send_buffer, "EXIT") != NULL) { // verifica se o cliente quer encerrar a conexão com EXIT
            printf("Encerrando a conexão...\n");
            break;
        }
        if (strstr(send_buffer, "SHUTDOWN") != NULL) { // verifica se o cliente quer encerrar a conexão SHUTDOWN
            printf("Encerrando a conexão...\n");
            break;
        }

        // recebe a resposta do servidor
        if (recv(client_socket, recv_buffer, BUFFER_SIZE, 0) < 0) {
            perror("Falha ao receber a resposta do servidor"); // exibe um erro caso nao tenha sido possível receber a resposta do servidor
            exit(EXIT_FAILURE);
        }
        printf("Resposta do servidor: %s\n", recv_buffer); // printa a resposta do servidor
    }

    close(client_socket); // fecha o socket do cliente

    return 0;
}
