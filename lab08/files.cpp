#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
	char* input = argv[1];
	char* output = argv[2];

	int fin = open(input, O_RDONLY, 0666);
	int fout = open(output, O_CREAT | O_WRONLY, 0666);

	if (fin < 0 || fout < 0)
	{
		cout << "Error with openning files." << endl;
		exit(-1);
	}

	struct stat* statbuf;

	fstat(fin, statbuf);

	char* mapdata = (char*)mmap(NULL, statbuf->st_size, PROT_READ, MAP_SHARED, fin, 0);

	close(fin);

	lseek(fout, 0, SEEK_SET);
	write(fout, mapdata, statbuf->st_size);

	close(fout);

	return 0;
}