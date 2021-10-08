#include<stddef.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>


long int getFileSize(char file_name[])
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

