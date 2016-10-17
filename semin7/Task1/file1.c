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
    int     shmid;                                      // IPC дескриптор для области shared memory
    int     *index;                                     // создаем указатель на shared memory
    char    pathname[] = "key";
    key_t   key;
    int     fd;                                         // дескриптор для последующей работы с исходным файлом
    char    size;
    
      
    if((key = ftok(pathname,0)) < 0){                   // генерируем IPC ключ
        printf("Can't generate key\n");
        exit(-1);
    }
    
    if((shmid = shmget(key, 4096*sizeof(int), 0666|IPC_CREAT|IPC_EXCL)) < 0){           // пытаемся создать shared memory
        if(errno != EEXIST){
            printf("Can\'t create shared memory\n");                                    // не удалось создать
            exit(-1);
        } else {
            if((shmid = shmget(key, 3*sizeof(int), 0)) < 0){                            // уже создана
                printf("Can\'t find shared memory\n");
                exit(-1);
            }            
        }
    }
        
    if((index = (int *)shmat(shmid, NULL, 0)) == (int *)(-1)){      // берем адресс shared memory
        printf("Can't attach shared memory\n");
        exit(-1);
    }
       
    if((fd = open("file1.c", O_RDONLY, 0666)) < 0){                 // берем дескриптор исходного файла
        printf("Can\'t open file\n");
        exit(-1);      
    }
        
    if (read(fd, index, 4096) < 0){                        	    // записываем исходный текст в shared memory
        printf ("Can not read\n");
        exit(-1);
    }
       
    if(close(fd) < 0){                                      
        printf("Can not close file\n");
    }
        
    if(shmdt(index) < 0){                                           // удаляем адрессное пространство из текущего процесса
        printf("Can't detach shared memory\n");
        exit(-1);
    }
    
    return 0;
}

