#include<stdio.h>
#include<time.h>
#include<sys/param.h>
#include<stdlib.h>
#include<dirent.h>
#include<sys/types.h>
#include<sys/stat.h>
#include <stdlib.h>

#define OPTLIST "Aaf:h:mlq"

const int PATH_BUFSIZE = 1000;

extern char *strSHA2(char *filename);
extern int main(int argc, char **argv);
extern long int getFileSize(char file_name[]);
