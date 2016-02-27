#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <string.h>

sem_t sem1, sem2;



void* calculate_number(void* pArg)
{
	int* pX = (int*) pArg;

	for (;;)
	{
		(*pX)++;
        printf("\n child thread waiting");
        sem_wait(&sem2);
        printf("\n child thread signaled");
        sem_post(&sem1);

 	}



    return NULL;
}

void main(int argc, char* argv[])
{
    int X = atoi(argv[1]);
    pthread_t thread_id;

    sem_init(&sem1, 0, 0);
    sem_init(&sem2, 0, 0);

    printf("\n argv %s %d", argv[1], X);


    pthread_create(&thread_id, NULL, &calculate_number, (void*)&X);

	while(X < 3)
	{ 
        printf("\n child thread signaled");
        sem_post(&sem2);
		printf("\n main thread waiting %d", X);
    	sem_wait(&sem1);    
  	}

    printf("\n main thread waiting to join");

    pthread_join(thread_id, NULL);

    printf("\n child thread joined");
    sem_destroy(&sem1);
    sem_destroy(&sem2);
}