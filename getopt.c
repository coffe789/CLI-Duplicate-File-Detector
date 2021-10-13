//  CITS2002 Project 2 2021
//  Name(s):            Jonathan Davey, JJ Jun
//  Student number(s):   22887169, 22763977
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <getopt.h>

#define OPTLIST "Aaf:h:mlq"
bool a, A, f, h, l, m, q = false; //optional flags
char farg[1000];
char harg[1000];

int main(int argc, char *argv[], char *path[])
{
    int opt;
    while ((opt = getopt(argc, argv, OPTLIST)) != -1)
    {
        switch (opt)
        {
        case 'a': //all files considered including . files
            a = true;
            break;
        case 'A':               //indicates if using advanced options by exiting
            exit(EXIT_FAILURE); //we are not
        case 'f':               //finds duplicates of argument file. Exits depending on if found
            f = true;
            strcpy(farg, strSHA2(optarg));
            break;
        case 'h': //find all files with hash arg. Exits depending on if found
            h = true;
            strcpy(harg, optarg);
            break;
        case 'l': //lists duplicates
            l = true;
            break;
        case 'm': //advanced
            m = true;
            break;
        case 'q': //no printing. Exit depending on if duplicates found
            q = true;
            break;
        }
    }

    // optind is for the extra arguments
    // which are not parsed
    for (; optind < argc; optind++)
    {
        printf("extra arguments: %s\n", argv[optind]);
    }

    return 0;
}
