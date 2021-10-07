#include "duplicates.h"
#include<string.h>

struct FileHashPair
{
	char path[1000];
	char hash[256];
};
typedef struct FileHashPair FileHashPair;
FileHashPair pairList[1000];
int pairListIndex = 0;

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
			*count += 1;
			
			strcpy(path,rootPath);
			strcat(path,"/");
			strcat(path, dp->d_name);
			//pairList[pairListIndex].path = strdup(path);
			strcpy(pairList[pairListIndex].path, path);
			strcpy(pairList[pairListIndex].hash, strSHA2(path));


			//printf("%s\n", path);		//print file path
			//printf("%s\n", strSHA2(path)); //Print hash
			pairListIndex++;
			listFiles(path,count);
		}
	}
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
	for (int i = 0; i < pairListIndex; i++)
	{
		printf("%s\n", pairList[i].hash);
		printf("%s\n", pairList[i].path);
	}
	
//	int fileCount = 0;
//	DIR *dir = opendir(argv[1]);	//Points to DIR
//	if (dir == NULL){printf("Directory not found\n");exit(EXIT_FAILURE);}
//	struct dirent *entry;
//	while ((entry= readdir(dir))) //while entry is not NULL
//	{
//		char pathname[MAXPATHLEN];
//		sprintf(pathname,"%s/%s",argv[1],entry->d_name);
//		fileCount++;
//		struct stat statinfo;
//		if ((stat(pathname, &statinfo) != 0))
//		{
//			printf("err");
//			exit(EXIT_FAILURE);
//		}
//		printf("%s\t",pathname);
//	}
//	closedir(dir);
//	printf("%d files\n",fileCount);
}
