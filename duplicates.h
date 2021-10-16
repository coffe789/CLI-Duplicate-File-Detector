#include<stdio.h>
#include<time.h>
#include<sys/param.h>
#include<stdlib.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <stdlib.h>
#include<stdbool.h>

#define OPTLIST "Aaf:h:mlq"
#define EXIT_INVALID_FILE 3
#define EXIT_INVALID_DIRECTORY 4
#define ARRAY_BUFSIZE 1000

struct FileHashPair
{
	char path[1000];
	char hash[256];
};
typedef struct FileHashPair FileHashPair;

extern char *strSHA2(char *filename);
extern int main(int argc, char **argv);
extern int getFileSize(char file_name[]);
extern int getTotalFileSize();
extern int getLowestFileSize();
