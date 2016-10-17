#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
    int     *index;                      			// создаем указатель на shared memory
    int     shmid;                         			// IPC дескриптор для области shared memory
    char    pathname[] = "key";
    key_t   key;
    
    if((key = ftok(pathname,0)) < 0){       			// генерируем IPC ключ
        printf("Can\'t generate key\n");
        exit(-1);
    }
    
    if((shmid = shmget(key, 4096*sizeof(int), 0666|IPC_CREAT|IPC_EXCL)) < 0){         	// пытаемся создать shared memory
        
        if(errno != EEXIST){
            printf("Can\'t create shared memory\n");            			// не удалось создать
            exit(-1);
        } else {
            if((shmid = shmget(key, 3*sizeof(int), 0)) < 0){   				// уже создана
                printf("Can\'t find shared memory\n");
                exit(-1);
            } 
        }
    }
    
    if((index = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)){      			// берем адресс shared memory
        printf("Can't attach shared memory\n");
        exit(-1);
    }
    
    printf ("%s", (char *)index);
	printf("\n");  
    
    if(shmdt(index) < 0){                                				// удаляем адрессное пространство из текущего процесса
        printf("Can't detach shared memory\n");
        exit(-1);
    }

    if (shmctl(shmid, IPC_RMID, NULL) < 0){
	printf("Can not remove shared memory");
    }
    
    return 0;
}

