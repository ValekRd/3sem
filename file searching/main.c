#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#define MAX_LENGTH 1024

struct dirent *dept;
struct stat buff;

int Find (char * filename, int depth, char * direct);


int main(int argc, char *argv[])
{
    Find (argv[1], atoi (argv[2]), argv[3]);                // 1 - имя искомого файла. 2 - глубина. 3 - директория поиска

    return 0;
}

int Find (char * filename, int depth, char * direct)
{
    DIR *dir;

    char *path = (char *)malloc(MAX_LENGTH * sizeof(char));


    if ((dir = opendir (direct)) == NULL)
    {
        printf ("сan't open dir\n");
        exit (-1);
    }

    if (depth == 0)
        return;


    while ((dept = readdir (dir)) != NULL)
    {
        strcpy (path, direct);
        strcat (path, "/");
        strcat (path, dept -> d_name);

        stat (path, &buff);

        if (((buff.st_mode & S_IFMT) == S_IFREG) && (strcmp (filename, dept -> d_name) == 0))
            printf("File was found in %s\n", path);

        if (((buff.st_mode & S_IFMT) == S_IFDIR) && (strcmp (".", dept -> d_name) != 0) && (strcmp ("..", dept -> d_name) != 0))
            Find (filename, depth - 1, path);
    }
}