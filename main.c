#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

const char alphabet[]="abcdefghijklmnopqrst";
const int delay = 19;

pthread_t thread1, thread2, thread3; //идентификаторы потоков

void printErrorCreatingThread(int threadId);
void procthread1(void *arg);
void procthread2(void *arg);
void procthread3(void *arg);
void start();
void tryCancelThread(int id);
int tryCreateThreads();

int main()
{
    printf("\033[H\033[2J");    //отчистка экрана
    if (tryCreateThreads()) 
    {
        return 0;
    }

    start();

    getchar();
    printf("\033[37m");//очистка экрана

    return 0;
}

void procthread1(void *arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL); //запрет на отмену
    for (int i = 0; i <= delay; i++)
    {
        printf("\033[%d;20H\033[33m", i + 1);
        for (int j = 0; j < 2*(intptr_t)arg; j++) printf("%c", alphabet[i]);
        printf("\n");
        usleep(1011000);
    }
    pthread_exit(0);
}

void procthread2(void *arg)
{
    for (int i = 0; i <= delay; i++)
    {
        printf("\033[%d;40H\033[31m", i+1);
        for (int j = 0; j < 2*(intptr_t)arg; j++) printf("%c", alphabet[i]);
        printf("\n");
        usleep(1022000);
    }
    pthread_exit(0);
}

void procthread3(void *arg)
{
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    for (int i = 0; i <= delay; i++)
    {
        if (i == 12) 
        {
            pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);   //Отмена в отложенном режиме
            pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);   //разрешение на отмену
        }
        if (i == 16) 
        {
            printf("\033[%d;60H\033[35m", i+1);
            for (int j = 0; j < 2*(intptr_t)arg; j++) 
            {
                printf("%c", alphabet[i]);
            }
        }
        if (i < 12) {
            printf("\033[%d;60H\033[35m", i+1);
            for (int j = 0; j < 2*(intptr_t)arg; j++) 
            {
                printf("%c", alphabet[i]);
            }
            printf("\n");
            usleep(1033000);
        }
    }
    pthread_exit(0);
}

void printErrorCreatingThread(int threadId)
{
    printf("\033[21;1H ");
    printf("\033[21;1H\033[34mНе удалось создать нить %d \n", threadId);
}

void start()
{
    for (int i = 0; i <= delay; i++)
    {
        printf("\033[%d;1H\033[37m", i+1);
        printf("%d %c\n", i+1, alphabet[i]);
        if (i == 5)
        {
            tryCancelThread(1);
        }
        if (i == 10)
        {
            tryCancelThread(3);
        }
        usleep(1000000);
    }
    printf("\033[21;1H ");
    printf("\033[21;1H\033[34mНажмите любую кнопку для завершения...\n");
}

void tryCancelThread(int id)
{
    switch (id)
    {
    case 2: return;
    case 1:
        pthread_cancel(thread1);
        break;
    case 3:
        pthread_cancel(thread3);
        break;
    }
    printf("\033[21;1H ");
    printf("\033[21;1H\033[34mПопытка закрытия потока %d\n", id);
}

int tryCreateThreads()
{
    int result = 0;
    if ( pthread_create(&thread1, NULL, (void*)procthread1, (void*)2) )
    {
        printErrorCreatingThread(1);
        result = 1;
    }
    if ( pthread_create(&thread2, NULL, (void*)procthread2, (void*)3) )
    {
        printErrorCreatingThread(2);
        result = 1;
    }
    if ( pthread_create(&thread3, NULL, (void*)procthread3, (void*)4) )
    {
        printErrorCreatingThread(3);
        result = 1;
    }

    return result;
}