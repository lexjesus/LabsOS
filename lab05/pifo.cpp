#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

using namespace std;

int compare_ints(const void *a, const void *b)
{
    return (*((int *)b) - *((int *)a));
}

int main(int argv, char *argc[])
{
	int n, result; size_t size;
	int fdp[2];
	const char name[11] = "myfif.fifo";

	(void)umask(0);

	n = atoi(argc[1]);
	int* mas = new int[n];
	cout << "Random massive: ";
	for ( int i = 0; i < n; i++)
	{
		mas[i] = rand()%1000;
		cout << mas[i] << "  ";
	}
	cout << endl;
	if(mknod(name, S_IFIFO | 0666, 0))
		{
			cout << "Can`t create FIFO" << endl;
			exit(-1);
		}
	if (pipe(fdp) < 0)
	{
		cout << "Can`t creat the pipe!";	
		exit(-1);
	}
	

	size = sizeof(int)*n;
	result = fork();
	if ( result < 0)
	{
		cout << "Can`t fork child." << endl;
		exit(-1);
	} if(result > 0)
	{
		close(fdp[1]);
		int fifo = open(name, O_WRONLY);
		 if ( fifo  < 0)
		 	{
		 		cout << "Can`t open fifo." << endl;
		 	}
		int w = write(fifo, mas , size);
        if(w != size)
        {
			cout << "Can`t write all numbers." << endl;
		}
		close(fifo);
		int* sorted_mas = new int[n];
		if (read(fdp[0],sorted_mas, size) != size)
		{
			cout << "Can`t read all message." << endl;
		}
		close(fdp[0]);
		cout << "Sorted: ";
		for(int i = 0; i < n; i++)
		{
			cout << sorted_mas[i] << "  ";
		}
		cout << endl;
		delete[] sorted_mas;
		delete[] mas;
		
	remove("myfif.fifo");
		
	}else
	{
		close(fdp[0]);
		int r;
		int fifo = open(name, O_RDONLY);
		if ( fifo < 0)
		{
		 	cout << "Can`t open fifo." << endl;
		}
		int* rec_mas = new int[n];
		r = read(fifo,rec_mas, size);
		if ( r != size)
		{
			cout << "Can`t read all message." << endl;
		}
		close(fifo);
		qsort(rec_mas, n,sizeof(int), compare_ints);
		if (write(fdp[1],rec_mas, size) != size)
		{
			cout << "Can`t write all numbers." << endl;
		}
		close(fdp[1]);
		delete[] rec_mas;
		delete[] mas;

	}
	
	return 0;
}