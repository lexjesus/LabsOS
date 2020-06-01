#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <chrono>

using namespace std;
using namespace std::chrono;



void randrand(int* arr, int minn, int maxn, int n)
{
	srand(time(NULL));
	int r = maxn - minn + 1;
    for (int i = 0; i < n; i++)
    {
        arr[i] = minn + rand() % r;
    }
}

void printmas(int* arr, int nn)
{
	for(int i = 0; i < nn; i++)
	{
		cout << arr[i] << " ";
	}
	cout << endl;
}

int ipget(char* arg, char* ip)
{
	int iplen;
	int i = 0;int ii = 0;
	while(arg)
	{
		if(arg[i] == ':')
		{
			iplen = i;
			break;
		}
		i++;
	}
	for (char* l = ip; l < ip + iplen ; l++)
	{
		*l = *(arg + ii);
		ii++;
	}
	*(ip + ii) = '\0';

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

//----------------------------------------------------------
	int sockfd, len, n, maslen, min, max;
	struct sockaddr_in servaddr, cliaddr;
	cout << "Enter massive size: "; 
	cin >> maslen;
	int sendline[maslen],recvline[maslen];

	cout << "Enter min and max of random numbers: ";
	cin >> min >> max;

	randrand(sendline, min, max, maslen);

	cout << "Source massive: ";
	printmas(sendline, maslen);

	sockfd = socket(PF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
	{
		perror("socket() error");
		exit(-1);
	}

//----------------------------------------------------------
	
	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = PF_INET;
	servaddr.sin_port = htons(atoi(prt));
	if (inet_aton(ip, &servaddr.sin_addr) == 0)
	{
		cout << "invalid IP!" << endl;
		close(sockfd);
		exit(-1);
	}
	
//------------------------------------------------------------
	if(connect(sockfd, (struct sockaddr*) &servaddr, sizeof(servaddr)) < 0)
	{
		perror("Connect() error");
		close(sockfd);
		exit(-1);
	}
//------------------------------------------------------------
	steady_clock::time_point start,end;
    


if(write(sockfd, (void*)&maslen, 4)< 0)
	{
		perror("write()1 error");
		close(sockfd);
		exit(-1);
	}

	if(write(sockfd, sendline, sizeof(int)*maslen)< 0)
	{
		perror("write()2 error");
		close(sockfd);
		exit(-1);
	}
	start = steady_clock::now();
//------------------------------------------------------------
	int r = read(sockfd, recvline, sizeof(int)*maslen);
	end = steady_clock::now();
	if (r < 0)
	{
		cout << "Error with receiving!" << endl;
		close(sockfd);
		exit(-1);
	}
	
    duration<double, milli> waittime = end - start;
     cout << endl << "Sorted massive: ";
    printmas(recvline, maslen);
        cout << "Time of wait: " << to_string(waittime.count()) << "ms" << endl;
   
	
    close(sockfd);
	return 0;
}