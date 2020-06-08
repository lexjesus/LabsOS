#include <iostream>
#include <sys/shm.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <chrono>

using namespace std;
using namespace std::chrono;

void initSem(int semId, int n)
{
    struct sembuf op;
    op.sem_op = 1;
    op.sem_flg = 0;
    op.sem_num = n;
    semop(semId, &op, 1);
}

void tryToGetSem(int semId, int n)
{
    struct sembuf op;
    op.sem_op = -1;
    op.sem_flg = 0;
    op.sem_num = n;
    semop(semId, &op, 1);
}

int main(int argv, char *argc[])
{
    int n = atoi(argc[1]);
    int minn = atoi(argc[2]);
    int maxn = atoi(argc[3]);

    int memId = shmget(IPC_PRIVATE, sizeof(int) * n, 0600|IPC_CREAT|IPC_EXCL);
    int semId = semget(IPC_PRIVATE, n, 0600 | IPC_CREAT);
    int* arr = (int*)shmat(memId, 0, 0);

    int r = maxn - minn + 1;
    for (int i = 0; i < n; i++)
    {
        arr[i] = minn + rand() % r;
    }


    cout << "Source: " << endl;
    for (int i = 0; i < n; ++i)
        {
            cout << arr[i] << " ";
        }
    cout << endl;
    for (int i = 0; i < n; ++i)
    {
        initSem(semId, i);
    }
     
    int ch = fork();
    if (ch == 0)
    {
        int *arr = (int*)shmat(memId, 0, 0);

    for (int i = 0; i < n; ++i)
    {
        int mInd = i;
        for (int j = i + 1; j < n; ++j)
        {
            tryToGetSem(semId,i);
            tryToGetSem(semId,j);
            if (arr[j] < arr[mInd])
            {
                mInd = j;
                usleep(400000);
            }
            initSem(semId, i);
            initSem(semId, j);
        }
        if (i != mInd)
        {
            tryToGetSem(semId,i);
            tryToGetSem(semId,mInd);
            int temp = arr[i];
            arr[i] = arr[mInd];
            arr[mInd] = temp;
            usleep(400000);
            initSem(semId, i);
            initSem(semId, mInd);
        }
    }
    }
    else
    {
        int i = 0; int g;
        int status; 
        steady_clock::time_point start,end;
        duration<double> waittime;
        do
        {
            cout << "Iter: "<< i << endl;
            for (int j = 0; j < n; ++j)
            {   
                start = steady_clock::now();
                g = semctl(semId, j, GETVAL, 0);
                tryToGetSem(semId, j);
                 
                end = steady_clock::now();
                duration<double, milli> waittime = end - start;
                if(g == 0)
                {
                cout << arr[j] << "(" << to_string(waittime.count()) << "ms) " << endl;
                }
                else
                {
                    cout << arr[j] << " " << endl;
                }
                initSem(semId, j);
                usleep(40000);
            }
            cout << endl;
            status = waitpid(ch, NULL, WNOHANG);
            i++; fflush(stdout);
            usleep(100000);
        } while(!status);

        cout << "Result: ";
        for (int i = 0; i < n; ++i)
        {
            cout << arr[i] << " ";
        }
        cout << endl;

        shmctl(memId, 0, IPC_RMID);
        semctl(semId, 0, IPC_RMID);
    }
}