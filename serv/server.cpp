#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <iostream>

using namespace std;

long getFileSize(FILE* file)
{
    fseek(file, 0L, SEEK_END);
    long size = ftell(file);
    rewind(file);
    return size;
}


int main(int argc, char* argv[])
{
    int port = atoi(argv[1]);
    struct sockaddr_in server_addr;

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = PF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    int sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket() error");
        exit(-1);
    }
    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        server_addr.sin_port = 0;
        if (bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
        {
            perror("bind() error");
            exit(-1);
        }
    }

    socklen_t server_len = sizeof(server_addr);
    listen(sock, 100);
    getsockname(sock, (struct sockaddr *)&server_addr, &server_len);
    cout << "Listening on port: " << ntohs(server_addr.sin_port) << endl;

    if (fork() == 0)
    {
        while (1)
        {
            struct sockaddr_in client_addr;
            socklen_t client_len = sizeof(client_addr);
            int newSock = accept(sock, (struct sockaddr *)&client_addr, &client_len);
            
            if (fork() == 0)
            {
                continue;
            }
            while (1)
            {
                char input[128];
                int n = read(newSock, input, sizeof(input));
                if (n == 0)
                {
                    close(newSock);
                    exit(0);
                }

                FILE* file = fopen(input, "r");
                if (!file)
                {
                    long fileSize = -1 ;
                    write(newSock, &fileSize, sizeof(fileSize));
                }
                else
                {
                    
                    long fileSize = getFileSize(file);
                    write(newSock, &fileSize, sizeof(fileSize));
                    char message[fileSize];
                    fread(message, sizeof(char), fileSize, file);
                    fclose(file);
                    write(newSock, message, sizeof(message));
                }
            }
        }
    }
    else
    {
        cout << "Enter '/exit' to close program" << endl;
        char message[100];
        while (1)
        {
            cin >> message;

            if (!strcmp(message, "/exit"))
            {
                close(sock);
                exit(0);
            }
            else
            {
                cout << "You entered a nonexistent command" << endl;
            }
        }
    }
}