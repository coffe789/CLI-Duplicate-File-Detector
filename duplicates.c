//  CITS2002 Project 2 2021
//  Name(s):            Jonathan Davey, JJ Jun
//  Student number(s):   22887169, 22763977
#include "duplicates.h"
#include<string.h>
#include <unistd.h>
#include <getopt.h>

bool aFlag,fFlag,hFlag,lFlag,mFlag,qFlag = false;//optional flags
char fArgument[ARRAY_BUFSIZE];//command line arguments
char hArgument[ARRAY_BUFSIZE];


FileHashPair pairList[ARRAY_BUFSIZE];
int pairListIndex = 0;

char pathList[ARRAY_BUFSIZE][ARRAY_BUFSIZE];//Will contain all paths without duplicates
int pathListIndex = 0;

//Sets optional flags and sets the path argument as the supplied directory
void setOpts(int argc, char *argv[], char *path)
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
				exit(EXIT_FAILURE);//we are not
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
	// The path is an unparsed argument. If more than one is supplied, it will only take the last one
	for (; optind < argc; optind++)
	{
		if (opendir(argv[optind])==NULL)
		{
			printf("'%s' is an Invalid Path!\n",argv[optind]);
			exit(EXIT_INVALID_DIRECTORY);
		}
		strcpy(path,argv[optind]);
	}
}

// Fills pairList[] with all file/hash pairs, and sets count to the total # of files
void listFiles(const char *rootPath, int *count)
{
	struct dirent *dp;
	char fullPath[ARRAY_BUFSIZE];
	DIR *dir = opendir(rootPath);

	if (dir == NULL)//Does not try to open files as directories & doesn't include directories it can't access
	{
		return;	
	}

	while ( (dp = readdir(dir)) != NULL)
	{
		if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
		{
			if (*(dp->d_name)=='.' && !aFlag){continue;} //skip hidden files unless -a
			strcpy(fullPath,rootPath);
			strcat(fullPath,"/");
			strcat(fullPath, dp->d_name);
			DIR *dp2 = opendir(fullPath);
			if (dp2 ==NULL)
			{

				*count += 1;
				strcpy(pairList[pairListIndex].path, fullPath);
				strcpy(pairList[pairListIndex].hash, strSHA2(fullPath));
				pairListIndex++;
				closedir(dp2);
			}

			listFiles(fullPath,count);
		}
	}
}

// Used by qsort(), compares fileHashPairs
int qHashcmp(const void *p1, const void *p2)
{
	char *str1 = (char*) malloc(64*sizeof(char));//64 is size of hash
	char *str2 = (char*) malloc(64*sizeof(char));
	strcpy(str1,((FileHashPair*)p1)->hash);
	strcpy(str2,((FileHashPair*)p2)->hash);
	int cmp = strcmp(str1,str2);
	free(str1);free(str2);
	return cmp;
}

// Creates an array without duplicates, return dupcount
// May perform an action for each duplicate found, depending on the flags
int trackDuplicates()
{
	bool isOnDupStreak = false;
	bool fSuccess = false;
	int dupcount = 0;

	strcpy(pathList[0],pairList[0].path);
	pathListIndex++;
	if (fFlag && strcmp(fArgument,pairList[0].hash)==0)//doesn't check index 0 otherwise
	{
		fSuccess = true;
		printf("%s\t",pairList[0].path);
	}
	for (int i = 0; i < pairListIndex-1; i++) //Count duplicates
	{	
		if (strcmp(pairList[i].hash,pairList[i+1].hash)==0)
		{
			dupcount++;
			if (qFlag)
			{
				exit(EXIT_FAILURE);
			}
			if (fFlag && strcmp(fArgument,pairList[i+1].hash)==0)
			{
				fSuccess = true;
				printf("%s\n",pairList[i+1].path);
			}
			if (lFlag)
			{
				if(!isOnDupStreak)//The first in a set of duplicates isn't treated as one. This makes sure it is still printed for l flag
				{
					printf("%s\t", pairList[i].path);
				}
				isOnDupStreak = true;
				printf("%s\t", pairList[i+1].path);
			}
		}
		else
		{
			isOnDupStreak = false;

			strcpy(pathList[pathListIndex],pairList[i+1].path);
			pathListIndex++;
		}
	}
	if (qFlag) exit(EXIT_SUCCESS);
	if (fSuccess) exit(EXIT_SUCCESS);
	if ((lFlag && dupcount != 0)) printf("\n");
	if (fFlag && !fSuccess) exit(EXIT_FAILURE);
	return dupcount;
}


//relates to -h flag. Assume -h is true.
void findHashMatch()
{
	bool exitVal = EXIT_FAILURE;
	for (int i = 0; i < pairListIndex; i++)
	{
		if (strcmp(hArgument,pairList[i].hash)==0)
		{
			printf("%s\n",pairList[i].path);
			exitVal = EXIT_SUCCESS;
		}
	}
	exit(exitVal);
}

int main(int argc, char **argv)
{
	char path[ARRAY_BUFSIZE]; //path argument from terminal
	setOpts(argc, argv, path);
	int dupcount;
	int lowestSize;
	int totalSize;
	if (argc < 2)
	{
		printf("Incorrect program invocation!\nUsage:\t./duplicates directory_path <-flags>\n");
		exit(EXIT_FAILURE);
	}
	int count = 0;
	listFiles(path, &count);//Fill PairList[]
	if (count == 0)
	{
		printf("No files inside supplied directory!\n");
		exit(EXIT_FAILURE);
	}
	qsort(pairList, pairListIndex, sizeof(FileHashPair),qHashcmp);//sort PairList[]
	if (hFlag) findHashMatch(); //Do -h flag
	dupcount = trackDuplicates(); 
	totalSize = getTotalFileSize(pairList,pairListIndex);
	lowestSize = getLowestFileSize(pathList,pathListIndex);
	printf("Total number of files:\t\t%d\nNumber of unique files:\t\t%d\nTotal file size:\t\t%d bytes\nSize without duplicates:\t%d bytes\n", count,count - dupcount,totalSize,lowestSize);
}
