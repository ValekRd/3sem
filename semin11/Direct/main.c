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
#include <dirent.h>

struct dirent *dent;

int main ()
{
    DIR *dir = opendir("/Users/admin/Desktop/UNIX");
    if (dir == NULL)
    {
        printf ("сan't open dir\n");
        exit (-1);
    }

    if(dir != NULL)
    {
        while((dent = readdir(dir))!= NULL)
        {
            printf ("[%s]\n", dent->d_name);
            
        }
    }
    close(dir);

}

