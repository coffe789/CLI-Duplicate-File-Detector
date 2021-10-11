#include<stddef.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include "duplicates.h"

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

int getTotalFileSize(FileHashPair *pairList,int pairListIndex)
{
	int totalSize = 0;
	for (int i = 0; i <pairListIndex; i++)
	{
		totalSize += getFileSize(pairList[i].path);
	}
	return totalSize;
}

int getLowestFileSize(char **pathList,int pathListIndex)
{
	printf("pathlist pointer is %p\n",(void*)pathList);
	printf("pathlist index 0 is %s\n",pathList[0]);
	int lowestSize = 0;
	for (int i = 0; i <pathListIndex; i++)
	{
		lowestSize += getFileSize(pathList[i]);
	}
	return lowestSize;
}
