#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/sem.h>



int main()
{
    int     fd[2];                                              // Создаем один массив под pipe


    const char pathname[] = "key";                              // имя файла для генерации ключа

    key_t key;
    if ((key = ftok(pathname , 0)) < 0 )                        // генерируем ключ
    {
        printf("Can`t generate key\n");
        exit(-1);
    }


    if(pipe(fd) < 0)                                            // создаем pipe
    {
        printf("Can\'t open pipe1\n");
        exit(-1);
    }


    int result = 0;                                             // идентификатор родителя и ребенка

    if( (result = fork()) < 0)                                  // Рождение ребенка
    {
        printf("Can\'t fork child\n");
        exit(-1);
    }

    int semid;
    if ((semid = semget(key , 2 , 0666 | IPC_CREAT)) < 0)       //создание массива семафоров или обращение к существующему
    {
        printf("Can`t get semid\n");
        exit(-1);
    }

    struct sembuf mybuf;


    mybuf.sem_op = 1;							                // заполняем структуру под операцию A(S2, 1)
    mybuf.sem_flg = 0;
    mybuf.sem_num = 1;

    if (semop(semid , &mybuf , 2) < 0)                          // выполняем операцию A
    {
        printf("Can`t wait for condition0\n");
        exit(-1);
    }


    if (result > 0) {                                           // Действия родителя



        for ( int i = 0; i < 100; i++)
        {
            int number1[2] = {2, 2};                                    // создаем массив для двух чисел a=2 и b=2

            if( write(fd[1], number1, 8) != 8)                  // Пишем два числа в pipe
            {
                printf("Can\'t write a and b to pipe\n");
                exit(-1);
            }


            mybuf.sem_op = -1;							        // заполняем структуру под операцию D(s1, 1)
            mybuf.sem_flg = 0;
            mybuf.sem_num = 0;

            if (semop(semid , &mybuf , 2) < 0)                  // выполняем операцию D
            {
                printf("Can`t wait for condition1\n");
                exit(-1);
            }

            int answer1 = 0;

            if (read(fd[0], &answer1, 4) != 4)
            {
                printf("Can\'t read answer1 to pipe\n");
                exit(-1);
            }


            mybuf.sem_op =  1;							        // заполняем структуру под операцию A(s2, 1)
            mybuf.sem_flg = 0;
            mybuf.sem_num = 1;

            if (semop(semid , &mybuf , 2) < 0)                  // выполняем операцию A
            {
                printf("Can`t wait for condition2\n");
                exit(-1);
            }

        }

    }



    else {                                                      // Действия ребенка

        for (int i = 0; i < 100; i++)
        {
            mybuf.sem_op = -1;							        // заполняем структуру под операцию D(s2, 1)
            mybuf.sem_flg = 0;
            mybuf.sem_num = 1;

            if (semop(semid , &mybuf , 2) < 0)                  // выполняем операцию D
            {
                printf("Can`t wait for condition3\n");
                exit(-1);
            }

            int namber2[2] = {0};                               // создаем массив для чтения чисел a и b

            if(read(fd[0], namber2, 8) < 0)
            {
                printf("Can\'t read a и b from pipe\n");
                exit(-1);
            }

            int answer2 = namber2[0] + namber2[1];

            if( write(fd[1], &answer2, 4) != 4)                 // Пишем ответ сложения в pipe
            {
                printf("Can\'t write answer2 to pipe\n");
                exit(-1);
            }

            mybuf.sem_op = 1;							        // заполняем структуру под операцию A(s1, 1)
            mybuf.sem_flg = 0;
            mybuf.sem_num = 0;

            if (semop(semid , &mybuf , 2) < 0)                  // выполняем операцию A
            {
                printf("Can`t wait for condition4\n");
                exit(-1);
            }

        }


    }


    printf ("Good job!\n");                                     // должно вывести дважды!



    return 0;
}
