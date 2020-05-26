#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <iostream>

using namespace std;

int sig1 = 0, sig2 = 0, sig3 = 0;

void handler(int nsig)
{
    if (nsig == SIGINT)
    {
        sig1++;
    }
    else if (nsig == SIGUSR1)
    {
        sig2++;
    }
    else if(nsig == SIGUSR2)
    {
        sig3++;
    }
}

int main()
{
    signal(SIGINT, handler);
    signal(SIGUSR1, handler);
    signal(SIGUSR2, handler);
    while(1)
    {
        if (sig1 < 5)
        {
            cout << endl;
            cout << "SIGINT: " << sig1 << endl;
            cout << "SIGUSR1: " << sig2 << endl;
            cout << "SIGUSR2: " << sig3 << endl;
        }
       else
       {
            cout << endl << "Results:" << endl;
            cout << "SIGINT: " << sig1 << endl;
            cout << "SIGUSR1: " << sig2 << endl;
            cout << "SIGUSR2: " << sig3 << endl;
            break;
       }
       
        sleep(1);
        time_t curTime = time(NULL);
        cout << endl;
        cout << "Current time: " << ctime(&curTime) << endl;
    }
    return 0;
}