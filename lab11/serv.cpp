#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>

using namespace std;

int compare_ints(const void *a, const void *b)
{
    return (*((int *)b) - *((int *)a));
}

void printmas(int* arr, int nn)
{
	for(int i = 0; i < nn; i++)
	{
		cout << arr[i] << " ";
	}
	cout << endl;
}

int main(int argc, char* argv[])
{
	int sockfd, newsockfd; socklen_t clilen;
	int line[1000];
	struct sockaddr_in servaddr, cliaddr;

	

	sockfd = socket(PF_INET, SOCK_STREAM, 0 );
	if (sockfd < 0)
	{
		perror("socket() error");
		exit(-1);
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = PF_INET;
	servaddr.sin_port = htons(atoi(argv[1]));
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);

	
	if(bind(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0)
	{
		servaddr.sin_port = htons(0);
		if(bind(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0)
		{
			perror("bind() error");
			close(sockfd);
			exit(-1);
		}
	}
	cout << ntohs(servaddr.sin_port) << " port is used."<< endl;


	if (listen(sockfd, 1) < 0)
	{
		perror(NULL);
		close(sockfd);
		exit(-1);
	}
	
	newsockfd = accept(sockfd, NULL, NULL);
	if ( newsockfd < 0)
	{
		perror("accept() error");
		close(sockfd);
		exit(-1);
	}
	int n;
	read(newsockfd, &n , 4);
	
	read(newsockfd, line, sizeof(int)*n);
	qsort(line, n,sizeof(int), compare_ints);
	write(newsockfd, line, sizeof(int)*n);

	close(sockfd);
	close(newsockfd);

	return 0;
}