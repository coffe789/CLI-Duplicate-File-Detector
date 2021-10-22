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
        printf("The file '%s' does not exist!\n", file_name);
        return -1;
    }
    fseek(fp, 0L, SEEK_END); //Move stream to end of file
    long int size = ftell(fp); //Get file size

    fclose(fp);
    return size;
}

int getTotalFileSize(FileInfo *FileInfoList, int FileInfoListIndex)
{
	int totalSize = 0;
	for (int i = 0; i <FileInfoListIndex; i++)
	{
		if (!FileInfoList[i].isIdentical)
		{
			totalSize += getFileSize(FileInfoList[i].path);
		}
	}
	return totalSize;
}

int getLowestFileSize(FileInfo *FileInfoList, int FileInfoListIndex)
{
	int lowestSize = 0;
	for (int i = 0; i <FileInfoListIndex; i++)
	{
		if (!FileInfoList[i].isDuplicate && !FileInfoList[i].isIdentical)
		{
			lowestSize += getFileSize(FileInfoList[i].path);
		}
	}
	return lowestSize;
}
