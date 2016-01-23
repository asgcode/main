
#include <stdio.h>
#include "osutils.h"

static OS_HANDLE  hExitMainThreadSema;
static OS_HANDLE  hEvenSema;
static OS_HANDLE  hOddSema;

void printEvenBusyWait(void* arg)
{
    int* pInt = (int*)arg;
    while ((*pInt) <= 1000)
    {
        if (((*pInt) & 1) == 0)
        {
            printf("Even %d \n", (*pInt));
            (*pInt)++;
        }
    }
    OsReleaseSemaphore(hExitMainThreadSema, 1, NULL);
    printf("Exit even thread\n");
    TerminateThread();
}

void printOddBusyWait(void* arg)
{
    int* pInt = (int*)arg;
    while ((*pInt) < 1000)
    {
        if (((*pInt) & 1) == 1)
        {
            printf("Odd %d \n", (*pInt));
            (*pInt)++;
        }
    }
    printf("Exit odd thread\n");
    TerminateThread();
}


void printOddEvenBusyWait()
{
    static int i = 0;

    // Create two threads, both sharing global data 
     CreateThread( printEvenBusyWait, 0, &i );
     CreateThread( printOddBusyWait, 0, &i );

     OsWaitForSingleObject( hExitMainThreadSema, INFINITE );
     OsCloseHandle(hExitMainThreadSema);
     printf("Exiting main thread\n");
}



void printEvenNoBusyWait(void* arg)
{
    int* pInt = (int*)arg;
    do{
        if (((*pInt) & 1) == 0)
        {
            printf("Even %d \n", (*pInt));
            (*pInt)++;
            ReleaseSemaphore(hOddSema, 1, NULL);
        }
        else
        {
            WaitForSingleObject( hEvenSema, INFINITE );
        }
    }while((*pInt) <= 1000);

    OsReleaseSemaphore(hExitMainThreadSema, 1, NULL);
    printf("Exit even thread\n");
    TerminateThread();
}

void printOddNoBusyWait(void* arg)
{
    int* pInt = (int*)arg;
    do
    {
        if (((*pInt) & 1) == 1)
        {
            printf("Odd %d \n", (*pInt));
            (*pInt)++;
            OsReleaseSemaphore(hEvenSema, 1, NULL);
        }
        else
        {
            OsWaitForSingleObject(hOddSema, INFINITE );
        }
    }while((*pInt) < 1000);
    printf("Exit odd thread\n");
    TerminateThread();
}

void printOddEvenNoBusyWait()
{
    static int i = 0;

     hEvenSema = CreateSemaphore( NULL, 1,1, NULL );
     hOddSema = CreateSemaphore( NULL, 0,1, NULL );
     hExitMainThreadSema = CreateSemaphore( NULL, 0,1, NULL );

    // Create two threads, both sharing global data 
     CreateThread( printEvenNoBusyWait, 0, &i );
     CreateThread( printOddNoBusyWait, 0, &i );

     OsWaitForSingleObject( hExitMainThreadSema, INFINITE );
     OsCloseHandle(hEvenSema);
     OsCloseHandle(hOddSema);
     OsCloseHandle(hExitMainThreadSema);
     printf("Exiting main thread\n");
}

void printEvenParallelSema(void* arg)
{
    int i = 0;
    while (i <= 20)
    {
        OsWaitForSingleObject( hEvenSema, INFINITE );
        printf("Even %d \n", i);
        i = i+2;
        OsReleaseSemaphore(hOddSema, 1, NULL);
    }

    OsReleaseSemaphore(hExitMainThreadSema, 1, NULL);
    printf("Exit even thread\n");
    TerminateThread();
}

void printOddParallelSema(void* arg)
{
    int i = 1;
    while (i <= 19)
    {
        OsWaitForSingleObject( hOddSema, INFINITE );
        printf("Odd %d \n", i);
        i = i+2;
        OsReleaseSemaphore(hEvenSema, 1, NULL);
    }
    printf("Exit odd thread\n");
    TerminateThread();
}


void printOddEvenWithoutSharedMemWithSemaphore()
{
     hEvenSema = CreateSemaphore( NULL, 1,1, NULL );
     hOddSema = CreateSemaphore( NULL, 0,1, NULL );
     hExitMainThreadSema = CreateSemaphore( NULL, 0,1, NULL );

    // Create two threads, both sharing global data 
     CreateThread( printEvenParallelSema, 0, NULL);
     CreateThread( printOddParallelSema, 0, NULL);

     OsWaitForSingleObject( hExitMainThreadSema, INFINITE );
     OsCloseHandle(hEvenSema);
     OsCloseHandle(hOddSema);
     OsCloseHandle(hExitMainThreadSema);
     printf("Exiting main thread\n");

}