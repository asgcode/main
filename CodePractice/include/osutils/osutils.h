#include <process.h>
#include <windows.h>

#ifdef _WIN32
typedef HANDLE OS_HANDLE;
typedef LONG OS_LONG;
typedef LPLONG OS_LPLONG;

typedef void( __cdecl *FNPTHREAD_ROUTINE_POINTER )( void * );

#else
#endif


void* OsReleaseSemaphore(OS_HANDLE hSema, OS_LONG  releaseCount, OS_LPLONG prevCount)
{
#ifdef _WIN32
    ReleaseSemaphore(hSema, releaseCount, prevCount);
#else
#endif
    /*FixME: Return proper value*/
    return NULL;
}

void TerminateThread()
{
#ifdef _WIN32
    _endthread();
#else
#endif
}

int* CreateThread(FNPTHREAD_ROUTINE_POINTER fnpThreadRoutine, unsigned int stackSize, void* arglist)
{
    int* Ret = NULL;
#ifdef _WIN32
    _beginthread(fnpThreadRoutine, stackSize, arglist);
#else
#endif
    /*FixME: Return proper value*/
    return Ret;
}

unsigned int  OsWaitForSingleObject(OS_HANDLE hnd, unsigned int waitTimeInMs)
{
    unsigned int Ret = 0;
#ifdef _WIN32
    Ret = WaitForSingleObject( hnd, waitTimeInMs);
#else
#endif
    return Ret;
     
}

void  OsCloseHandle(OS_HANDLE hnd)
{
#ifdef _WIN32
    CloseHandle(hnd);
#else
#endif
}