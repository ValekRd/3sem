#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main()
{
    const char pathname[] = "key";

    struct sembuf mybuf;

    key_t key;
    if ((key = ftok(pathname , 0)) < 0 )
    {
        printf("Can`t generate key\n");
        exit(-1);
    }

    int fd;
    if((fd = open("file", O_WRONLY | O_APPEND )) < 0)
    {
        printf("Can\'t open file\n");
        exit(-1);
    }

    if (write(fd, "Second!", 8) != 8)
    {
        printf ("Cant not write <second>\n");
    }

    if (close(fd) < 0)
    {
        printf ("Can not close file\n");
    }

    int semid;
    if ((semid = semget(key , 1, 0666 | IPC_CREAT)) < 0)
    {
        printf("Can`t get semid\n");
        exit(-1);
    }


    mybuf.sem_op = -1;							// выполняем операцию B
    mybuf.sem_flg = 0;
    mybuf.sem_num = 0;

    if (semop(semid , &mybuf , 1) < 0)
    {
        printf("Can`t wait for condition\n");
        exit(-1);
    }

    if (semctl(semid, 0, IPC_RMID, 0) < 0)
    {
        printf ("Can not delet semafor.\n");
    }

    printf ("Good!\n");

    return 0;
}
