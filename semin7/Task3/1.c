#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[])
{
    const char pathname[] = "key";                      // имя файла для генерации ключа

    key_t key;
    if ((key = ftok(pathname , 0)) < 0 )                // генерируем ключ
    {
        printf("Can`t generate key\n");
        exit(-1);
    }


    for (int i = 0; i < 10000; i ++)                          // зацикливаем программу
    {
        int semid;
        if ((semid = semget(key , 1 , 0666 | IPC_CREAT)) < 0)    //создание массива семафоров или обращение к существующему
        {
            printf("Can`t get semid\n");
            exit(-1);
        }

        struct sembuf mybuf;

        mybuf.sem_op = 0;							        // заполняем структуру под операцию Z
        mybuf.sem_flg = 0;
        mybuf.sem_num = 0;

        if (semop(semid , &mybuf , 1) < 0)                  // выполняем операцию Z
        {
            printf("Can`t wait for condition\n");
            exit(-1);
        }

        int fd;
        if((fd = open("file", O_WRONLY | O_APPEND) < 0))               // берем файловый дескриптор файла для записи
        {
            printf("Can\'t open file\n");
            exit(-1);
        }

        if (write(fd, "First!", 7) != 7)                    // пишем текст в файл
        {
            printf ("Cant not write <first>\n");
        }
        if (close(fd) < 0)                                  // завершаем работу с файлом
        {
            printf ("Can not close file\n");
        }


        mybuf.sem_op = 1;							        // заполняем структуру под операцию A
        mybuf.sem_flg = 0;
        mybuf.sem_num = 0;

        if (semop(semid , &mybuf , 1) < 0)                  // выполняем операцию А
        {
            printf("Can`t wait for condition\n");
            exit(-1);
        }
    }



    return 0;
}