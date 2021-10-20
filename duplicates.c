//  CITS2002 Project 2 2021
//  Name(s):            Jonathan Davey, JJ Jun
//  Student number(s):   22887169, 22763977
#include "duplicates.h"
#include<string.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/stat.h>

bool aFlag,fFlag,hFlag,lFlag,mFlag,qFlag = false;//optional flags
char fArgument[ARRAY_BUFSIZE];//command line flag arguments
char hArgument[ARRAY_BUFSIZE];

char inputPaths[ARRAY_BUFSIZE][ARRAY_BUFSIZE];
int inputPathsIndex= 0;

FileInfo fileInfoList[ARRAY_BUFSIZE];
int fileInfoListIndex = 0;

//Set optional flags and fill inputPaths[] 
void setOpts(int argc, char *argv[])
{
	int opt;
	while ((opt = getopt(argc, argv, OPTLIST)) != -1)
	{
		switch (opt)
		{
			case 'a': //all files considered including . files
				aFlag = true;
				break;
			case 'A': //indicates if using advanced options by exiting
				exit(EXIT_SUCCESS);
			case 'f'://finds duplicates of argument file. Exits depending on if found
				fFlag = true;
				FILE *fp = fopen(optarg,"r");
				if (fp==NULL)
				{
					printf("'%s' is an Invalid File!\n",optarg);
					exit(EXIT_INVALID_FILE);
				}
				fclose(fp);
				strcpy(fArgument,strSHA2(optarg));
				break;
			case 'h'://find all files with hash arg. Exits depending on if found
				hFlag = true;
				strcpy(hArgument,optarg);
				break;
			case 'l'://lists duplicates
				lFlag = true;
				break;
			case 'm'://advanced
				mFlag = true;
				break;
			case 'q'://no printing. Exit depending on if duplicates found
				qFlag = true;
				break;
		}
	}
	// The path is an unparsed argument, supports multiple paths. 
	for (; optind < argc; optind++)
	{
		if (opendir(argv[optind])==NULL)
		{
			printf("'%s' is an Invalid Path!\n",argv[optind]);
			exit(EXIT_INVALID_DIRECTORY);
		}
		strcpy(inputPaths[inputPathsIndex],argv[optind]);
		inputPathsIndex++;
	}
}

// Combines a file's file ID & file system ID to create a unique value
long int create_fileUID(char *filePath)
{
	struct stat buf;
	stat(filePath, &buf);
	long int a = buf.st_ino;
	int b = buf.st_dev;
	return (long int) (a * 10 + b);	//concatenate values to create unique combination of ino & dev 
}

// Fills fileInfoList[] with all file/hash pairs, and sets count to the total # of files
void listFiles(const char *rootPath)
{
	struct dirent *dp;
	char fullPath[ARRAY_BUFSIZE];
	DIR *dir = opendir(rootPath);

	while ( (dp = readdir(dir)) != NULL)
	{
		if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
		{
			if (*(dp->d_name)=='.' && !aFlag) continue; //skip hidden files unless -a
			strcpy(fullPath,rootPath);
			strcat(fullPath,"/");
			strcat(fullPath, dp->d_name);
			DIR *dp2 = opendir(fullPath);
			if (dp2 ==NULL)	//if it is a file and not a directory
			{
				strcpy(fileInfoList[fileInfoListIndex].path, fullPath);
				strcpy(fileInfoList[fileInfoListIndex].hash, strSHA2(fullPath));
				fileInfoList[fileInfoListIndex].fileID = create_fileUID(fullPath);

				fileInfoListIndex++;
				continue; // Don't do directory recursion on a file 
			}
			closedir(dp2);
			listFiles(fullPath);
		}
	}
}

// Used by qsort(), compares fileHashPairs and fileID's
int fileInfoCmp(const void *p1, const void *p2)
{
	int hashCmp = strcmp(((FileInfo*)p1)->hash,((FileInfo*)p2)->hash);
	if (hashCmp == 0)//if hashes are equal, sort by fileID
	{
		int IDCmp = 0;
		int ID_a = ((FileInfo*)p1)->fileID; 
		int ID_b = ((FileInfo*)p2)->fileID; 
		if (ID_a > ID_b) IDCmp = 1;
		else if (ID_a < ID_b) IDCmp = -1;
		return IDCmp;
	}
	return hashCmp;
}

// Marks each FileInfo entry as a duplicate or exact file copy
// Count total & duplicate files, ignoring exact copies
// May perform an action for each duplicate found, depending on the flags
void trackDuplicates(int *totalCount, int *dupCount)
{
	bool isOnDupStreak = false;
	bool fSuccess = false;

	if (fFlag && strcmp(fArgument,fileInfoList[0].hash)==0)//doesn't check index 0 otherwise
	{
		fSuccess = true;
		printf("%s\t",fileInfoList[0].path);
	}
	if (*fileInfoList[0].path != '\0')//doesn't count index 0 otherwise
	{
		*totalCount += 1;
	}
	for (int i = 0; i < fileInfoListIndex-1; i++) //Count duplicates
	{	
		if (fileInfoList[i].fileID==fileInfoList[i+1].fileID)//skip identical files from overlapping directories
		{
			fileInfoList[i+1].isIdentical = true;
			continue;
		}
		*totalCount += 1;
		if (strcmp(fileInfoList[i].hash,fileInfoList[i+1].hash)==0)
		{
			fileInfoList[i+1].isDuplicate = true;
			*dupCount+=1;
			if (qFlag)
			{
				exit(EXIT_FAILURE);
			}
			if (fFlag && strcmp(fArgument,fileInfoList[i+1].hash)==0)
			{
				fSuccess = true;
				printf("%s\n",fileInfoList[i+1].path);
			}
			if (lFlag)
			{
				if(!isOnDupStreak)//The first in a set of duplicates isn't counted as one. This makes sure it is still printed for l flag
				{
					printf("%s\t", fileInfoList[i].path);
				}
				isOnDupStreak = true;
				printf("%s\t", fileInfoList[i+1].path);
			}
		}
		else
		{
			isOnDupStreak = false;

//			fileInfoList[i+1].isDuplicate = true;
		}
	}
	if (qFlag) exit(EXIT_SUCCESS);
	if (fSuccess) exit(EXIT_SUCCESS);
	if ((lFlag && dupCount != 0)) printf("\n");
	if (fFlag && !fSuccess) exit(EXIT_FAILURE);
//	return dupcount;
}


//Print the path of every hash match of 'hArgument'
void findHashMatch()
{
	bool exitVal = EXIT_FAILURE;
	for (int i = 0; i < fileInfoListIndex; i++)
	{
		if (strcmp(hArgument,fileInfoList[i].hash)==0)
		{
			printf("%s\n",fileInfoList[i].path);
			exitVal = EXIT_SUCCESS;
		}
	}
	exit(exitVal);
}

int main(int argc, char **argv)
{
	setOpts(argc, argv);
	int dupCount = 0;
	int lowestFileSize;
	int totalFileSize;
	if (argc < 2)
	{
		printf("Incorrect program invocation!\nUsage:\t./duplicates directory_path <-flags>\n");
		exit(EXIT_FAILURE);
	}
	int count = 0;
	while(inputPathsIndex > 0)//allows use of multiple paths
	{
		listFiles(inputPaths[inputPathsIndex-1]);//Fill fileInfoList[]
		inputPathsIndex-=1;
	}
	qsort(fileInfoList, fileInfoListIndex, sizeof(FileInfo),fileInfoCmp);//sort fileInfoList[] such that we can track duplicates or identical files
	if (hFlag) findHashMatch(); //Do -h flag
	trackDuplicates(&count, &dupCount); 
	totalFileSize = getTotalFileSize(fileInfoList,fileInfoListIndex);
	lowestFileSize = getLowestFileSize(fileInfoList,fileInfoListIndex);
	printf("Total number of files:\t\t%d\nNumber of unique files:\t\t%d\nTotal file size:\t\t%d bytes\nSize without duplicates:\t%d bytes\n",\
		count,count - dupCount,totalFileSize,lowestFileSize);
}
