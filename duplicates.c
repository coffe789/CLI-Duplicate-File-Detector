#include "duplicates.h"
#include<string.h>

struct FileHashPair
{
	char path[1000];
	char hash[256];
};
typedef struct FileHashPair FileHashPair;

void getFileList(const char *rootPath, FileHashPair *array, int *index)
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
			strcpy(path,rootPath);
			strcat(path,"/");
			strcat(path, dp->d_name);
		//printf("root path is %s\nindex is %d\npath is %s\n\n",rootPath,*index,path);
			strcpy((array + (*index) * sizeof(FileHashPair))->path, path);
			//printf("path is: %s\n",(array[*index]).path);
			strcpy((array + *index * sizeof(FileHashPair))->hash, strSHA2(path));


			(*index)+=1;
			printf("%s\n",(array + (*index) * sizeof(FileHashPair))->path);
			printf("%stest\n",path);//(array[0])->hash);
			getFileList(path,array,index);
		}
	}
}


int my_strcmp(const void *p1, const void *p2)
{
	char *str1 = (char*) malloc(64*sizeof(char));//Create space for the string. 64 is size of hash
	char *str2 = (char*) malloc(64*sizeof(char));
	strcpy(str1,((FileHashPair*)p1)->hash);//Set the strings to the hashes we are comparing
	strcpy(str2,((FileHashPair*)p2)->hash);
	int i = 0;
	while (str1[i] !='\0' && str2[i] != '\0')//my implementation of strcmp. Should probably just replace this with the lib function.
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
	FileHashPair *pairList = malloc(1000*sizeof(FileHashPair));
	int pairListIndex = 0;
	if (argc < 2)
	{
		printf("Please supply a directory path\nUsage:\t./duplicates directory_path <-flags>\n");
		exit(EXIT_FAILURE);
	}
	int count = 0;
	getFileList(argv[1], pairList,&pairListIndex);
	qsort(pairList, pairListIndex, sizeof(FileHashPair),my_strcmp);
	for (int i = 0; i < pairListIndex-1; i++)
	{	
		if (strcmp(pairList[i].hash,pairList[i+1].hash)==0)
		{
			dupcount++;
			printf("duplicate:\n%s\n", pairList[i+1].hash);
			printf("%s\n", pairList[i+1].path);
		}
	}
	printf("%d\n",pairListIndex);
	printf("Total number of files: %d\nDupcount: %d\n", count,dupcount);


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
