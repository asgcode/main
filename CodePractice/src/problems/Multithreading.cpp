
#include <stdio.h>
#include "osutils.h"

static OS_HANDLE  hExitMainThreadSema;
static OS_HANDLE  hEvenSema;
static OS_HANDLE  hOddSema;

void PrintEvenBusyWait(void* arg)
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

void PrintOddBusyWait(void* arg)
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


void PrintOddEvenBusyWait()
{
    static int i = 0;

    // Create two threads, both sharing global data 
     CreateThread( PrintEvenBusyWait, 0, &i );
     CreateThread( PrintOddBusyWait, 0, &i );

     OsWaitForSingleObject( hExitMainThreadSema, INFINITE );
     OsCloseHandle(hExitMainThreadSema);
     printf("Exiting main thread\n");
}



void PrintEvenNoBusyWait(void* arg)
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

void PrintOddNoBusyWait(void* arg)
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

void PrintOddEvenNoBusyWait()
{
    static int i = 0;

     hEvenSema = CreateSemaphore( NULL, 1,1, NULL );
     hOddSema = CreateSemaphore( NULL, 0,1, NULL );
     hExitMainThreadSema = CreateSemaphore( NULL, 0,1, NULL );

    // Create two threads, both sharing global data 
     CreateThread( PrintEvenNoBusyWait, 0, &i );
     CreateThread( PrintOddNoBusyWait, 0, &i );

     OsWaitForSingleObject( hExitMainThreadSema, INFINITE );
     OsCloseHandle(hEvenSema);
     OsCloseHandle(hOddSema);
     OsCloseHandle(hExitMainThreadSema);
     printf("Exiting main thread\n");
}

void PrintEvenParallelSema(void* arg)
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

void PrintOddParallelSema(void* arg)
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


void PrintOddEvenWithoutSharedMemWithSemaphore()
{
     hEvenSema = CreateSemaphore( NULL, 1,1, NULL );
     hOddSema = CreateSemaphore( NULL, 0,1, NULL );
     hExitMainThreadSema = CreateSemaphore( NULL, 0,1, NULL );

    // Create two threads, both sharing global data 
     CreateThread( PrintEvenParallelSema, 0, NULL);
     CreateThread( PrintOddParallelSema, 0, NULL);

     OsWaitForSingleObject( hExitMainThreadSema, INFINITE );
     OsCloseHandle(hEvenSema);
     OsCloseHandle(hOddSema);
     OsCloseHandle(hExitMainThreadSema);
     printf("Exiting main thread\n");

}