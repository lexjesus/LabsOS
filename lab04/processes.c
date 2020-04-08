#include <stdio.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

void Sort(int data[], int lenD)
{
  int j = 0;
  int tmp = 0;
  for(int i=0; i<lenD; i++){
    j = i;
    for(int k = i; k<lenD; k++){
      if(data[j]>data[k]){
        j = k;
      }
    }
    tmp = data[i];
    data[i] = data[j];
    data[j] = tmp;
  }
}
const size_t memSize = 20*sizeof(int);

int compare_ints(const void *a, const void *b)
{
    return (*((int *)a) - *((int *)b));
}

void sortAndPrint(int *mem)
{
    Sort(mem, 20);
    for (int i = 0; i < 20; i++)
    {
        printf("%i ", mem[i]);
    }
    printf("\n");
}

int main()
{
    srand((unsigned)(time(0)));

    int memId = shmget(IPC_PRIVATE, memSize, 0600 | IPC_CREAT | IPC_EXCL);

    int *mem = (int *)shmat(memId, 0, 0);

    for (int i = 0; i < 20; i++)
    {
        mem[i] = rand() % 100;
    }

    for (int i = 0; i < 20; i++)
    {
        printf("%d ", mem[i]);
    }

    printf("\n");

    int child_id = fork();

    if (child_id == 0)
    {
        sortAndPrint(mem);
    }
    else
    {
        waitpid(child_id, NULL, 0);
    }

    shmdt(mem);

    return 0;
}