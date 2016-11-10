#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#define TABLE_LIMIT 21                                      // вместительность стола
#define NUMBER_DISH 6                                       // колличество типов посуды



struct sembuf mybuf;
void sem_oper(int n_sem, int n);                                 // объявление функции "семафор"



struct info                                                 // структура (информация) для мойщика и вытиральщика
{
    char type_of_dish[15]; 	                                // 1. Тип посуды
    int time;		                                        // 2. Затраченное время
}wash[NUMBER_DISH], wip[NUMBER_DISH];



int semid;



int main()
{
    /* создаем ключ */

    char pathname[] = "key";
    key_t key;
    if ((key = ftok(pathname, 0)) < 0)
    {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    /* создание семафора */

    if ((semid = semget(key , 1 , 0666 | IPC_CREAT)) < 0)
    {
        printf("Can`t get semid\n");
        exit(-1);
    }

    /* открываем файл с информацией для мойщика */

    FILE* fp;                                               // входной поток

    if ((fp = fopen("washer.txt","r")) == NULL)
    {
        printf("File not found.\n");
    }

    /* считываем информацию для мойщика в его структуру */

    for(int i = 0; i < NUMBER_DISH; i++)
    {
        fscanf(fp, "%s %*c %d", wash[i].type_of_dish, &wash[i].time);
    }
    fclose(fp);

    /* открываем файл с информацией для вытиральщика */

    if ((fp = fopen("wiper.txt","r"))==NULL)
    {
        printf("File not found.\n");
    }

    /* считываем информацию для вытиральщика в его структуру */

    for(int i = 0; i < NUMBER_DISH; i++)
    {
        fscanf(fp, "%s %*c %d", wip[i].type_of_dish, &wip[i].time);
    }
    fclose(fp);


    sem_oper(0,TABLE_LIMIT);               //создаем место на столе для посуды

    /* воспользуемся pipe для передачи посуды */
    int fd[2];
    pipe(fd);

    /* создадим процесса ребенка */
    int result = fork();

    if(result > 0)           // действия мойщика
    {
        if ((fp = fopen("dishes.txt","r")) == NULL)         //открываем фаил с посудой
        {
            printf("File not found.\n");
        }

        char name_dish[15]; // название посуды

        int n = 0;          // кол-во посуды

        while (fscanf(fp, "%s %*c %d", name_dish, &n) < NUMBER_DISH + 1) //читаем файл с кол-вом посуды

            for (int i = 0; i < NUMBER_DISH; i++)
            {
                if(strcmp(name_dish, wash[i].type_of_dish) == 0)        // смотрим информацию в структуре данные для данной посуды
                {
                    for(int j = 0; j < n; j++)                          //делаем для n посуд этого типа
                    {
                        sleep(wash[i].time);                            // моем

                        sem_oper(0, -1);                                //кладем на стол

                        printf("Dishwasher washed the %s\n\n", wash[i].type_of_dish);

                        write (fd[1], wash[i].type_of_dish, 15);        // используем pipe для передачи названия
                    }

                }
            }

        fclose(fp);
    }

    else                    // действия вытиральщика
    {
        char name_dishes[15];

        while(1)
        {
            read (fd[0], name_dishes, 15);

            for(int i = 0; i < NUMBER_DISH; i++)
            {

                if(strcmp(name_dishes, wip[i].type_of_dish) == 0)   // смотрим информацию в структуре данные для данной посуды
                {
                    sem_oper(0,1);                               //добавляем место на столе

                    sleep (wip[i].time);                    //вытираем

                    printf("Wipe wiped the %s\n\n", wip[i].type_of_dish);
                }
            }
        }
    }
    return 0;
}


void sem_oper(int n_sem, int n)
{
    struct sembuf mybuf;
    mybuf.sem_op = n;
    mybuf.sem_flg = 0;
    mybuf.sem_num = n_sem;
    if (semop(semid , &mybuf , 1) < 0)
    {
        printf("Can`t wait for condition\n");
        exit(-1);
    }
}