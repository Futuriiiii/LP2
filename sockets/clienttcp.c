#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>


#define SERVER_PORT 9999
#define BUFFER_SIZE 1024



int main(void){

    int client_fd = 0;
    struct sockaddr_in server_address;
    char buffer_in[BUFFER_SIZE];
    char buffer_out[BUFFER_SIZE];

    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(client_fd < 0) {
        perror("Socket error\n");
        exit(EXIT_FAILURE);
    }

    server_address.sin_family = AF_INET;
    int inet_pton_ret = inet_pton(AF_INET, "127.0.0.1", &server_address.sin_family);

    if(inet_pton_ret < 0){
        perror("inet_pton() error!");
        exit(EXIT_FAILURE);
    }

    server_address.sin_port = htons(SERVER_PORT);
    int con_ret = connect(client_fd, (struct sockaddr*)server_address, sizeof(server_address));

    if(con_ret < 0){
        perror("Erro ao conectar\n");
        exit(EXIT_FAILURE);

    }

    int read_ret = read(client_fd, buffer_in, sizeof(buffer_in));

    if(read_ret < 0){
        perror("Erro ao conectar\n");
        exit(EXIT_FAILURE);
    }


    printf("Recebi do servidor: %s\n", buffer_in);
    strcpy(buffer_out, "MENSAGEM DO SERVIDOR")

    int write_read = write(client_fd, buffer_out, strlen(buffer_out));

    if(write_read < 0){
        perror("Erro no write\n");
        exiit(EXIT_FAILURE);
    }

    printf("Recebi do servidor")
    return 0;
}