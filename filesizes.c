#include<stddef.h>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include "duplicates.h"

int getFileSize(char file_name[])
{
    FILE *fp = fopen(file_name, "r");
    if (fp == NULL)
    {
        printf("The file does not exist!\n");
        return -1;
    }
    fseek(fp, 0L, SEEK_END); //Move stream to end of file
    long int size = ftell(fp); //Get file size

    fclose(fp);
    return size;
}

int getTotalFileSize(FileHashPair *pairList, int pairListIndex)
{
	int totalSize = 0;
	for (int i = 0; i <pairListIndex; i++)
	{
		totalSize += getFileSize(pairList[i].path);
	}
	return totalSize;
}

int getLowestFileSize(FileHashPair *pairList, int pairListIndex)
{
	int lowestSize = 0;
	for (int i = 0; i <pairListIndex; i++)
	{
		if (!pairList[i].isDuplicate)
		{
			lowestSize += getFileSize(pairList[i].path);
		}
	}
	return lowestSize;
}
