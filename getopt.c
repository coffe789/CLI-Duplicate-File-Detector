#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <getopt.h>

#define OPTLIST "Aaf:h:mlq"

int main(int argc, char *argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, OPTLIST)) != -1)
    {
        switch (opt)
        {
        case 'a': //all file considered
            break;
        case 'A': //return EXIT_SUCCESS or EXIT_FAIRLURE
            break;
        case 'f':
	    printf("%s\n",optarg);
            break;
        case 'h':
	    printf("%s\n",optarg);
            break;
        case 'l':
            break;
        case 'm':
            break;
        case 'q':
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
