#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>


char message[] = "Mom soap frame";
char result[256];
int pointer_bit = 0;
pid_t ppid;

void my_handler_receive(int signal);            //полльзовательская функция обработки сигнала (получатель)
void my_handler_send();                         //пользовательская функция обработки сигнала (отправитель)


int main()
{
    signal(SIGUSR1, my_handler_receive);        //обработка пользовательского сигнала SIGUSR1 функцией my_handler_receive
    signal(SIGUSR2, my_handler_receive);
    signal(SIGINT, my_handler_send);            //SIGINT - сигнал прерывания (клавиши Ctrl-C)

    ppid = getpid();                            
    pid_t pid = fork();

            if (pid != 0)                       //действие родителя
                {
                    ppid = pid;
                    my_handler_send();
                }
    while(1);
}

void my_handler_receive(int nsig)               //nsig - номер возникающего сигнала (данная фунция может обрабатывать несколько сигналов
{
    int i = pointer_bit / 8;                    // i - номер байта
    int position = pointer_bit % 8;             // position - указывает на текущий бит

        if (nsig == SIGUSR2)                    // если сигнал ознает 1
        {
            result[i] |= (1 << position);       // прибавляем бит 1 посредствомсдвига и записываем в результат
        }

    pointer_bit++;

    kill(ppid, SIGINT);                         //передаем сигнал SIGINT процессу родителю

        if (i == strlen(message))               // если передали всю строку (все кол-во байтов)
        {
            printf("%s\n", result);
        }
}

void my_handler_send()
{
    char bit;

    int i = pointer_bit / 8;
    int position = pointer_bit % 8;

    bit = message[i] & (char)(1 << position);       //кладем в переменную бит текущий бит в message[i]

    pointer_bit++;

    kill(ppid, bit ? SIGUSR2 : SIGUSR1);        //если bit = 0 выполняем SIGUSR1, иначе SIGUSR2

        if (i == strlen(message))
        {
            exit(0);
        }
}