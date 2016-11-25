#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 1024


<<<<<<< HEAD
int main(int argc, char* argv[])
=======
/*
 * это копия семинарского упражнения, а не file searching
 */

int main ()
>>>>>>> origin/master
{
    char *directory = argv[1];
    int depth = atoi(argv[2]);                                      // приводим строку с глубиной в числовой вид
    char* filename = argv[3];

    DIR *dir;
    struct dirent *dept;
    char *path = (char *)malloc(MAX_LENGTH * sizeof(char));
    if ((dir = opendir(directory)) == NULL)                         // открваем поток с нужной директорией
    {
        printf("direct not open\n");
        exit(1);
    }
    while((dept = readdir(dir)) != NULL)
    {
        if (dept->d_type != DT_DIR)
        {
            if(strcmp(filename, dept->d_name) == 0)                 // сравниваем искомое имя файла с присутствующими
            {
                printf("Path:  %s\n", directory);
                free(path);
                return 0;
            }
        }
        if((dept->d_type == DT_DIR) && (depth > 0) && (strcmp(dept -> d_name, ".") && strcmp(dept -> d_name, "..")))
        {
            strcpy(path, directory);
            strcat(path, "/");
            strcat(path, dept->d_name);
            if ((path, depth - 1, filename) != 0 )
                printf ("Find file\n");
            else
                printf("File not found\n");
        }
    }
    free(path);
    return 0;
}