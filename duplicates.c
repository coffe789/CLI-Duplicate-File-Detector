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


int my_strcmp(const void *p1, const void *p2)
{
	char *str1 = (char*) malloc(64*sizeof(char));//64 is size of hash
	char *str2 = (char*) malloc(64*sizeof(char));
	strcpy(str1,((FileHashPair*)p1)->hash);
	strcpy(str2,((FileHashPair*)p2)->hash);
	int i = 0;
	while (str1[i] !='\0' && str2[i] != '\0')
	{
		if (str1[i] < str2[i]) {free(str1);free(str2);return -1;}
		if (str1[i] > str2[i]) {free(str1);free(str2);return 1;}
		++i;
	}
	if (strlen(str1) < strlen(str2)) {free(str1);free(str2);return -1;}
	if (strlen(str2) > strlen(str1)) {free(str1);free(str2);return 1;}
	free(str1);free(str2);return 0;
}

int main(int argc, char **argv)
{
	int dupcount = 0;
	int total_size = 0;
	if (argc < 2)
	{
		printf("Please supply a directory path\nUsage:\t./duplicates directory_path <-flags>\n");
		exit(EXIT_FAILURE);
	}
	int count = 0;
	listFiles(argv[1], &count);
	qsort(pairList, pairListIndex, sizeof(FileHashPair),my_strcmp);
	for (int i = 0; i <pairListIndex; i++)
	{
		total_size += getFileSize(pairList[i].path);
	}
	for (int i = 0; i < pairListIndex-1; i++) //Count duplicates
	{	
		if (strcmp(pairList[i].hash,pairList[i+1].hash)==0)
		{
			dupcount++;
		//	printf("duplicate:\n%s\n", pairList[i+1].hash);
		//	printf("%s\n", pairList[i+1].path);
		}
	}
	printf("Total number of files: %d\nNumber of duplicate files: %d\nTotal file size: %d bytes\n", count,dupcount,total_size);


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
