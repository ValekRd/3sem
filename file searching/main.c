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
#include <sys/stat.h>

struct dirent *dent;
struct stat buff;

int main ()
{
    DIR *dir = opendir("/Users/admin/Desktop/UNIX");        //имеем указатель dir на открытый поток дирректории
    if (dir == NULL)
    {
        printf ("сan't open dir\n");
        exit (-1);
    }


    if(dir != NULL)
    {
        while((dent = readdir(dir))!= NULL)
        {
            stat(dent -> d_name, &buff);
            printf ("[%s]\t", dent->d_name);

            if(buff.st_mode & S_IFREG)                      // это regular file?
                printf ("regular file\t");
            else                                            // иначе directory (S_IFDIR)
                printf ("directory\t");

            printf("size = %lld \n", buff.st_size);
            
        }
    }
    close(dir);

}

