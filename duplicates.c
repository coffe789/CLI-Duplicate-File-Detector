//  CITS2002 Project 2 2021
//  Name(s):            Jonathan Davey, JJ Jun
//  Student number(s):   22887169, 22763977
#include "duplicates.h"
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <sys/stat.h>

bool aFlag,fFlag,hFlag,lFlag,mFlag,qFlag = false;// Optional flags
char hArgument[PATH_BUFSIZE];
FileInfo fArgument;

char inputPaths[ARRAY_BUFSIZE][PATH_BUFSIZE];// Holds paths from terminal
int inputPathsIndex= 0;

FileInfo *fileInfoList;
int fileInfoListIndex = 0;
int arraySizeMultiplier = 1; // Used to dynamically increase array size

// Combines a file's ID & file system ID to create a unique value
long int createFileUID(char *filePath)
{
	struct stat buf;
	stat(filePath, &buf);
	long int a = buf.st_ino;
	int b = buf.st_dev;
	return (long int) (a * 100 + b);// Concatenate values to create unique combination of ino & dev 
}

void doubleArraySize(FileInfo **a)
{
	*a = realloc(*a,sizeof(FileInfo) * PATH_BUFSIZE * arraySizeMultiplier * 2);
	arraySizeMultiplier *= 2;
}

// Set optional flags and fill inputPaths[] 
void setOpts(int argc, char *argv[])
{
	int opt;
	while ((opt = getopt(argc, argv, OPTLIST)) != -1)
	{
		switch (opt)
		{
			case 'a': // Examine all files considered including . files
				aFlag = true;
				break;
			case 'A': // Indicates if using advanced options by exiting
				exit(EXIT_SUCCESS);
			case 'f':// Prints duplicates of argument file. Exits depending on if found
				fFlag = true;
				FILE *fp = fopen(optarg,"r");
				if (fp==NULL)
				{
					printf("'%s' is an Invalid File!\n",optarg);
					exit(EXIT_INVALID_FILE);
				}
				fclose(fp);
				strcpy(fArgument.hash,strSHA2(optarg));
				fArgument.fileID = createFileUID(optarg);
				break;
			case 'h':// Print all files with hash arg. Exits depending on if matches found
				hFlag = true;
				strcpy(hArgument,optarg);
				break;
			case 'l':// Print all duplicates
				lFlag = true;
				break;
			case 'm':
				printf("Note: -m flag is not implemented!\n\n");
				mFlag = true;
				break;
			case 'q':// No printing. Exit depending on if duplicates found
				qFlag = true;
				break;
		}
	}
	// All unparsed arguments are assumed paths, supports multiple paths. 
	for (; optind < argc; optind++)
	{
		if (opendir(argv[optind])==NULL)
		{
			printf("'%s' is an Invalid Directory!\n",argv[optind]);
			exit(EXIT_INVALID_DIRECTORY);
		}
		strcpy(inputPaths[inputPathsIndex],argv[optind]);
		inputPathsIndex++;
	}
}


// Fills fileInfoList[] with every file's path, hash, and uid within supplied directory
void retrieveFileInfo(const char *rootPath)
{
	struct dirent *dp;
	char fullPath[PATH_BUFSIZE];
	DIR *dir = opendir(rootPath);

	while ( (dp = readdir(dir)) != NULL)
	{
		if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
		{
			if (*(dp->d_name)=='.' && !aFlag) continue; // Skip hidden files unless -a
			strcpy(fullPath,rootPath);
			strcat(fullPath,"/");
			strcat(fullPath, dp->d_name);
			DIR *dp2 = opendir(fullPath);
			if (dp2 ==NULL)	// If it is a file and not a directory
			{
				if (fileInfoListIndex >= (ARRAY_BUFSIZE * arraySizeMultiplier))// Ensure array is always big enough
				{
					doubleArraySize(&fileInfoList);
				}
				strcpy(fileInfoList[fileInfoListIndex].path, fullPath);
				strcpy(fileInfoList[fileInfoListIndex].hash, strSHA2(fullPath));
				fileInfoList[fileInfoListIndex].fileID = createFileUID(fullPath);

				fileInfoListIndex++;
				continue; // Don't do directory recursion on a file 
			}
			closedir(dp2);
			retrieveFileInfo(fullPath);
		}
	}
}

// Used by qsort(), compares fileInfo structs
int fileInfoCmp(const void *p1, const void *p2)
{
	int hashCmp = strcmp(((FileInfo*)p1)->hash,((FileInfo*)p2)->hash);
	if (hashCmp == 0)// If hashes are equal, sort by fileID
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
void handleDuplicates(int *totalCount, int *dupCount)
{
	bool isOnDupStreak = false;
	bool fSuccess = false;

	if (fFlag && strcmp(fArgument.hash,fileInfoList[fileInfoListIndex].hash)==0
			&& fileInfoList[fileInfoListIndex].fileID != fArgument.fileID)//fFlag doesn't check final index otherwise
	{
		fSuccess = true;
		printf("%s\t",fileInfoList[fileInfoListIndex].path);
	}
	if (*fileInfoList[0].path != '\0')// Doesn't count index 0 otherwise
	{
		*totalCount += 1;
	}
	for (int i = 0; i < fileInfoListIndex-1; i++)
	{	
		if (fileInfoList[i].fileID==fileInfoList[i+1].fileID)//Skip identical files (either from overlapping directories or soft links)
		{
			fileInfoList[i+1].isIdentical = true;
			continue;
		}
		*totalCount += 1;
		if (fFlag && strcmp(fArgument.hash,fileInfoList[i].hash)==0 && fileInfoList[i].fileID != fArgument.fileID)// Has same hash as fArgument
		{
			fSuccess = true;
			printf("%s\n",fileInfoList[i].path);
		}
		if (strcmp(fileInfoList[i].hash,fileInfoList[i+1].hash)==0) // Duplicate found
		{
			fileInfoList[i+1].isDuplicate = true;
			*dupCount+=1;
			if (qFlag)
			{
				exit(EXIT_FAILURE);
			}
			if (lFlag)
			{
				if(!isOnDupStreak)// The first in a set of duplicates isn't counted as one. This makes sure it is still printed for l flag
				{
					printf("%s\t", fileInfoList[i].path);
				}
				printf("%s\t", fileInfoList[i+1].path);
			}
			isOnDupStreak = true;
		}
		else // Not a duplicate
		{
			isOnDupStreak = false;
		}
	}
	if (qFlag) exit(EXIT_SUCCESS);
	if (fSuccess) exit(EXIT_SUCCESS);
	if ((lFlag && *dupCount != 0)) printf("\n");
	if (fFlag && !fSuccess) exit(EXIT_FAILURE);
}


// Print the path of every hash match of 'hArgument'
void findHashMatch()
{
	bool exitVal = EXIT_FAILURE;
	for (int i = 0; i < fileInfoListIndex; i++)
	{
		if (strcmp(hArgument,fileInfoList[i].hash)==0 && !fileInfoList[i].isIdentical)
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
	int fileCount = 0;
	int dupCount = 0;
	int lowestFileSize;
	int totalFileSize;
	fileInfoList = (FileInfo*) malloc(ARRAY_BUFSIZE * sizeof(FileInfo));
	
	if (argc < 2)
	{
		printf("Incorrect program invocation!\nUsage:\t./duplicates directory_path <-flags>\n");
		exit(EXIT_FAILURE);
	}
	while(inputPathsIndex > 0)// Loop allows use of multiple paths
	{
		retrieveFileInfo(inputPaths[inputPathsIndex-1]);// Fill fileInfoList[]
		inputPathsIndex-=1;
	}
	qsort(fileInfoList, fileInfoListIndex, sizeof(FileInfo),fileInfoCmp);// Sort fileInfoList[] such that we can track duplicate or identical files
	handleDuplicates(&fileCount, &dupCount); 
	if (hFlag) findHashMatch(); //Do -h flag
	totalFileSize = getTotalFileSize(fileInfoList,fileInfoListIndex);
	lowestFileSize = getLowestFileSize(fileInfoList,fileInfoListIndex);
	printf("Total number of files:\t\t%d\nNumber of unique files:\t\t%d\nTotal file size:\t\t%d bytes\nSize without duplicates:\t%d bytes\n",
		fileCount,fileCount - dupCount,totalFileSize,lowestFileSize);
}
