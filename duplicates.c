#include "duplicates.h"
#include<string.h>

void listFiles(const char *rootPath)
{
	struct dirent *dp;
	char path[PATH_BUFSIZE];
	DIR *dir = opendir(rootPath);

	if (dir == NULL)
	{
		return;	//Does not try to open files as directories & doesn't include directories it can't access
		perror(""); printf("%s",rootPath);
		exit(EXIT_FAILURE);
	}

	while ( (dp = readdir(dir)) != NULL)
	{
		if (strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
		{
			printf("%s\n", dp->d_name);

			strcpy(path,rootPath);
			strcat(path,"/");
			strcat(path, dp->d_name);

			listFiles(path);
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
	listFiles(argv[1]);
//	printf("%s\n", strSHA2("."));
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
