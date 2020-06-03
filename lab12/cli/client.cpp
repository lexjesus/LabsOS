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


void recvFile(int sock, char* filename, long fileSize)
{
    
}

int ipget(char* arg, char* ip)
{
    int iplen;
    int i = 0;
    while(arg)
    {
        if(arg[i] == ':')
        {
            iplen = i;
            break;
        }i++;
    } i = 0;
    for (char* l = ip; l < ip + iplen ; l++)
    {
        *l = *(arg + i);
        i++;
    }*(ip + i) = '\0';

    return iplen;
}

int main(int argc, char* argv[])
{
    if(argc != 2 )
    {cout << "Error: program needs socket." << endl;}
    int ipl; char ip[15];
    ipl = ipget(argv[1], ip);
    char* prt = new char[strlen(argv[1]) - ipl + 1];
    prt = argv[1] + ipl+1;
    cout << "IP: " << ip << endl;
    cout << "Port: " << prt << endl;

    struct sockaddr_in server_addr;
    int sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("socket() error");
        exit(-1);
    }

    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = PF_INET;
    server_addr.sin_port = htons(atoi(prt));
    inet_aton(ip, &server_addr.sin_addr);

   if(connect(sockfd, (struct sockaddr*) &server_addr, sizeof(server_addr)) < 0)
    {
        perror("connect() error");
        close(sockfd);
        exit(-1);
    }

    cout << "You are connected!" << endl;
    cout << "Enter the filename or '/exit': " << endl;

    char message[128];
    while (1)
    {
        cin >> message;

        if (!strcmp(message,"/exit"))
        {
            cout << "Good bye!" << endl; 
            close(sockfd);
            exit(0);
        }
        else
        {
            long fileSize;
            write(sockfd, message, strlen(message) + 1);
            int n = read(sockfd, &fileSize, sizeof(long));
            if (n == 0)
            {
                close(sockfd);
                perror("Connection lost");
                exit(0);
            }
            if (fileSize < 0)
            {
                cout << "The file is busy with another program or does not exist" << endl;
            }
            else
            {
                recvFile(sockfd, message, fileSize);


                char buffer[fileSize];
                bzero(buffer, fileSize);
                FILE* file = fopen(message, "w");
                cout << endl;
                int n = 0;
                while (n < fileSize)
                {
                    n += read(sockfd, buffer, sizeof(buffer));
                    fprintf(file, "%s", buffer);
                }
                for(int i = 0; i < fileSize; i++)
                {
                    cout << buffer[i];
                }
                truncate(message, fileSize);
                cout << endl;
                fclose(file);
                cout << "File received successfully as " << message << endl;
            }
        }
    }
}
