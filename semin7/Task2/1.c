#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    int semid; 						// IPC-дескриптор для массива IPC семафоров

    const char pathname[] = "key"; 
    
    key_t key; 

    struct sembuf mybuf; 

    if ( (key = ftok(pathname , 0)) < 0 ) {
        printf("Can`t generate key\n");
        exit(-1);
    }

    
    if ((semid = semget(key , 1 , 0666 | IPC_CREAT)) < 0) {
        printf("Can`t get semid\n");
        exit(-1);
    }

     

    mybuf.sem_op = -5;							// сначала заполним структуру mybuf
    mybuf.sem_flg = 0;
    mybuf.sem_num = 0;

    if (semop(semid , &mybuf , 1) < 0) {				// Выполним операцию D(semid , 1) для нашего массива семафоров
        printf("Can`t wait for condition\n");
        exit(-1);
    }

    printf("Condition is present\n");
    return 0;
}
