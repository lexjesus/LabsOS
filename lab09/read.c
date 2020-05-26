#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    char* name = argv[1];
    int bytes = atoi(argv[2]);

    FILE* file = fopen(name, "r");
    if (file == NULL)
    {
        perror("Can't open file");
        exit(-1);
    }

    int data[bytes];
    int res = fread(data, sizeof(int), bytes, file);

    printf("Bytes read: %i\n", res);
    for (size_t i = 0; i < res; ++i)
    {
        printf("Read symbol: %i (0x%x)\n", data[i], data[i]);
    }
    fclose(file);
    return 0;
}