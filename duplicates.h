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
#define ARRAY_BUFSIZE 100
#define PATH_BUFSIZE 256
#define HASH_SIZE 256

struct FileInfo
{
	char path[PATH_BUFSIZE];
	char hash[HASH_SIZE];	//unique identifier for the contents of a file
	long int fileID;	//unique identifier for the file itself
	bool isDuplicate;	//Has same contents as a previous file
	bool isIdentical;	//Has same fileID as a previous file
};
typedef struct FileInfo FileInfo;

extern char *strSHA2(char *filename);
extern int main(int argc, char **argv);
extern int getFileSize(char file_name[]);
extern int getTotalFileSize();
extern int getLowestFileSize();
