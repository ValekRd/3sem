#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define LENGTH 555
#define THREAD_NUM 77

void* Sum(void *args);

int RESULT = 0;
int Num[THREAD_NUM];

int M [LENGTH];

int main()
{
    //===============================                           // рандомим массив
    for (int i = 0; i < LENGTH; i ++ )
    {
        M[i] = rand() %LENGTH;
        printf ("%d\t", M[i]);
    }
    printf ("\n");

    //===============================


    pthread_t thread[THREAD_NUM];

    //===============================

    for (int i = 0; i < THREAD_NUM; i++)
    {
        Num[i] = i;
    }
    //==============================



    for (int i = 0; i < THREAD_NUM; i++)
    {
        int status = 0;

        status = pthread_create(&thread[i], (pthread_attr_t *)NULL, Sum, &Num[i]);

        if (status != 0)
        {
            printf("main error: can't create thread, status = %d\n", status);
            exit(-1);
        }

    }

    for(int i = 0; i < THREAD_NUM; i ++)
    {
        pthread_join (thread[i], (void **)NULL);
    }

    printf ("Result = %d\n", RESULT);

    //====================================

    int comparison = 0;
    for (int i = 0; i < LENGTH; i ++)
    {
        comparison = comparison + M[i];
    }

    printf ("В действительности имеем %d\n", comparison);
    return 0;
}



void* Sum(void *args)
{
    int *arg = (int*) args;

        int left = *arg * (LENGTH/THREAD_NUM);
        int right = (1 + *arg) * (LENGTH/THREAD_NUM);

        int Summa = 0;

        for (int i = left; i < right; i ++)
        {
                Summa = Summa + M[i];
        }

    if (*arg == THREAD_NUM - 1)
    {
        int begin = LENGTH - (LENGTH%THREAD_NUM) - 1;
        for (int i = LENGTH - 1; i > begin; i --)
        {
            Summa = Summa + M[i];
        }
    }

        printf ("Я [%d] нить, моя сумма [%d]\n", *arg, Summa);

        RESULT = RESULT + Summa;


    return NULL;
}