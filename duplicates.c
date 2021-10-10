#include "duplicates.h"
#include<string.h>
#include <unistd.h>
#include <getopt.h>

bool a,A,f,h,l,m,q = false;//optional flags


struct FileHashPair
{
	char path[1000];
	char hash[256];
};

typedef struct FileHashPair FileHashPair;
FileHashPair pairList[1000];
int pairListIndex = 0;

char pathList[1000][1000];//1000 strings of size 1000
int pathListIndex = 0;

//Sets optional flags, and sets path to the path argument
void set_opts(int argc, char *argv[], char *path)
{
    int opt;
    while ((opt = getopt(argc, argv, OPTLIST)) != -1)
    {
        switch (opt)
        {
        case 'a': //all files considered including . files
		a = true;
        	break;
        case 'A': //indicates if using advanced options by exiting
        	exit(EXIT_FAILURE);//we are not
        case 'f'://finds duplicates of argument file. Exits depending on if found
		f = true;
		printf("f in unimplemented, arg: %s\n",optarg);
        	break;
        case 'h'://find all files with hash arg. Exits depending on if found
		h = true;
		printf("h in unimplemented, arg: %s\n",optarg);
        	break;
        case 'l'://lists duplicates
		l = true;
        	break;
        case 'm'://advanced
		m = true;
        	break;
        case 'q'://no printing. Exit depending on if duplicates found
		q = true;
        	break;
        }
    }

    // The path is an unparsed argument. If more than one is supplied, it will only take the last one
    for (; optind < argc; optind++)
    {
	strcpy(path,argv[optind]);
    }
}

// Fills pairList[] with all files, and sets count to #files
void listFiles(const char *rootPath, int *count)
{
	struct dirent *dp;
	char path[PATH_BUFSIZE];
	DIR *dir = opendir(rootPath);

	if (dir == NULL)
	{
		return;	//Does not try to open files as directories & doesn't include directories it can't access
	}

	while ( (dp = readdir(dir)) != NULL)
	{
		if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
		{
			if (*(dp->d_name)=='.' && !a){continue;} //skip hidden files unless -a
			*count += 1;
			strcpy(path,rootPath);
			strcat(path,"/");
			strcat(path, dp->d_name);
			strcpy(pairList[pairListIndex].path, path);
			strcpy(pairList[pairListIndex].hash, strSHA2(path));


			//printf("%s\n", path);		//print file path
			//printf("%s\n", strSHA2(path)); //Print hash
			pairListIndex++;
			listFiles(path,count);
		}
	}
}


int hashcmp(const void *p1, const void *p2)
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
int track_duplicates()
{
	strcpy(pathList[0],pairList[0].path);
	pathListIndex++;
	int dupcount = 0;
	for (int i = 0; i < pairListIndex-1; i++) //Count duplicates
	{	
		if (strcmp(pairList[i].hash,pairList[i+1].hash)==0)
		{
			dupcount++;
		//	printf("duplicate:\n%s\n", pairList[i+1].hash);
		//	printf("%s\n", pairList[i+1].path);
		}
		else
		{
			strcpy(pathList[pathListIndex],pairList[i+1].path);
			pathListIndex++;
		}
	}
	return dupcount;
}

// Should put this in the same .c file as getFileSize()
int getTotalFileSize()
{
	int totalSize = 0;
	for (int i = 0; i <pairListIndex; i++)
	{
		totalSize += getFileSize(pairList[i].path);
	}
	return totalSize;
}

int getLowestFileSize()
{
	int lowestSize = 0;
	for (int i = 0; i <pathListIndex; i++)
	{
		lowestSize += getFileSize(pathList[i]);
	}
	return lowestSize;
}

int main(int argc, char **argv)
{
	char path[1000]; //path argument from terminal
	set_opts(argc, argv, path);
	int dupcount;
	int lowestSize;
	int totalSize;
	if (argc < 2)
	{
		printf("Please supply a directory path\nUsage:\t./duplicates directory_path <-flags>\n");
		exit(EXIT_FAILURE);
	}
	int count = 0;
	listFiles(path, &count);//Fill PairList[]
	qsort(pairList, pairListIndex, sizeof(FileHashPair),hashcmp);//sort PairList[]
	dupcount = track_duplicates(); 
	totalSize = getTotalFileSize();
	lowestSize = getLowestFileSize();

	printf("Total number of files: %d\nNumber of duplicate files: %d\nTotal file size: %d bytes\nSize without duplicates: %d bytes\n", count,dupcount,totalSize,lowestSize);
}
