#include <stdio.h>
#include "duplicates.h"
#include <string.h>

void listFiles(const char *rootPath, int *count)
{
    struct dirent *dp;
    char path[PATH_BUFSIZE];
    DIR *dir = opendir(rootPath);

    if (dir == NULL)
    {
        return; //Does not try to open files as directories & doesn't include directories it can't access
    }

    while ((dp = readdir(dir)) != NULL)
    {
        if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
        {
            *count += 1;
            printf("%s\n", dp->d_name);

            strcpy(path, rootPath);
            strcat(path, "/");
            strcat(path, dp->d_name);

            listFiles(path, count);
        }
    }
}

void Array_Files(const char *rootPath, int *count)
{
}

long int findSize(char file_name[])
{
    // opening the file in read mode
    FILE *fp = fopen(file_name, "r");

    // checking if the file exist or not
    if (fp == NULL)
    {
        printf("The file does not exist!\n");
        return -1;
    }
    //moving file pointer to the end
    fseek(fp, 0L, SEEK_END);

    // calculating the size of the file
    long int size = ftell(fp);

    // closing the file
    fclose(fp);

    return size;
}

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        printf("Please supply a directory path\nUsage:\t./duplicates directory_path <-flags>\n");
        exit(EXIT_FAILURE);
    }
    int count = 0;
    listFiles(argv[1], &count);
    printf("Total number of files: %d\n", count);

    char file_name[];
    long int res = findSize(file_name);
    if (res != -1)
        printf("Size of the file is %ld bytes \n", res);
    return 0;
}
