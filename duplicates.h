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

struct FileHashPair
{
	char path[1000];
	char hash[256];
};
typedef struct FileHashPair FileHashPair;

extern char *strSHA2(char *filename);
extern int main(int argc, char **argv);
extern long int getFileSize(char file_name[]);
extern int getTotalFileSize();
extern int getLowestFileSize();
