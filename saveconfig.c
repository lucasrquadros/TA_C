#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define BUFSIZE 1024

int main(int argc, char *argv[]) {
    int sock, port;
    struct sockaddr_in serv_addr;
    char buffer[BUFSIZE];
    char *username = "admin\n";
    char *password = "admin\n";
    char *enable = "enable\n";
    char *enable_password = "admin\n";
    char *copy = "copy running-config startup-config\n";
    char *confirm = "y\n";

   
    if (argc != 2) {
        printf("Usage: %s <IP address>\n", argv[0]);
        exit(1);
    }

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        exit(1);
    }

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(23);
    if (inet_pton(AF_INET, argv[1], &serv_addr.sin_addr) <= 0) {
        printf("Invalid IP address\n");
        exit(1);
    }

    // Conecta ao servidor Telnet
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        exit(1);
    }

    // Envia o nome de usuário
    if (send(sock, username, strlen(username), 0) != strlen(username)) {
        perror("send");
        exit(1);
    }

    // Envia a senha
    if (send(sock, password, strlen(password), 0) != strlen(password)) {
        perror("send");
        exit(1);
    }

    // Envia o comando "enable"
    if (send(sock, enable, strlen(enable), 0) != strlen(enable)) {
        perror("send");
        exit(1);
    }

    // Envia a senha do "enable"
    if (send(sock, enable_password, strlen(enable_password), 0) != strlen(enable_password)) {
        perror("send");
        exit(1);
    }

    // Envia o comando "copy running-config startup-config"
    if (send(sock, copy, strlen(copy), 0) != strlen(copy)) {
        perror("send");
        exit(1);
    }

    // Aguarda a confirmação
    sleep(1);
    if (send(sock, confirm, strlen(confirm), 0) != strlen(confirm)) {
        perror("send");
        exit(1);
    }

    // Lê a resposta do servidor
    memset(buffer, 0, BUFSIZE);
    if (recv(sock, buffer, BUFSIZE - 1, 0) < 0) {
        perror("recv");
        exit(1);
    }
    printf("%s", buffer);

    // Fecha o socket
    close(sock);

    return 0;
}