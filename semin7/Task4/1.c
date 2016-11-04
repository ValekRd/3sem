#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[], char *envp[])
{
    const char pathname[] = "key";

    struct sembuf mybuf;


    key_t key;
    if ((key = ftok(pathname , 0)) < 0 )
    {
        printf("Can`t generate key\n");
        exit(-1);
    }

    int semid;
    if ((semid = semget(key , 1 , 0666 | IPC_CREAT)) < 0)     //создание массива семафоров или обращение к существующему
    {
        printf("Can`t get semid\n");
        exit(-1);
    }

    mybuf.sem_op = 0;							        // выполняем операцию Z
    mybuf.sem_flg = 0;
    mybuf.sem_num = 0;

    if (semop(semid , &mybuf , 1) < 0)
    {
        printf("Can`t wait for condition\n");
        exit(-1);
    }

    int fd;
    if((fd = open("file", O_WRONLY) < 0))
    {
        printf("Can\'t open file\n");
        exit(-1);
    }

    if (write(fd, "First!", 7) != 7)
    {
        printf ("Cant not write <first>\n");
    }
    if (close(fd) < 0)
    {
        printf ("Can not close file\n");
    }



    mybuf.sem_op = 1;							// выполняем операцию A
    mybuf.sem_flg = 0;
    mybuf.sem_num = 0;

    if (semop(semid , &mybuf , 1) < 0) {
        printf("Can`t wait for condition\n");
        exit(-1);
    }

    int result;
    result = execle("/Users/admin/Desktop/1/2", "/Users/admin/Desktop/1/2", "1.c", 0, envp);
    if(result < 0)
    {
        printf("Error on program start\n");
        exit(0);
    }

    return 0;
}