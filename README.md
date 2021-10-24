# Project2_CITS2002
This is a command line program created for UWA's Systems Programming unit.
It takes any number of directory paths as arguments, and within the directories detects all files with identical contents (duplicates).

The program will print:

   -  the total number of files found,
   - the total size (in bytes) of all files found
   - the total number of unique files (i.e. any duplicate is only counted once)
   - the possible minimum total size of all files found (i.e. the sizes of duplicated files are only counted once). 

The program supports a number of optional flags:

- -a 	By default, hidden and configuration files (conventionally those beginning with a '.' character) are not considered by duplicates. Providing the -a option requests that all files be considered. This is similar to the standard Linux utility ls.

- -A 	This option indicates if the program attempts the advanced version of the project.
duplicates -A produces no output at all, simply terminating with EXIT_SUCCESS (for advanced) or with EXIT_FAILURE (for basic).

- -f filename 	find and list, one per line, the relative pathnames of all files whose SHA2 hash matches that of the indicated file. The name of the indicated file is not listed.
duplicates -f terminates with EXIT_SUCCESS if any matching files are found, or with EXIT_FAILURE otherwise.

- -h hash 	find and list, one per line, the relative pathnames of all files with the indicated SHA2 hash.
duplicates -h terminates with EXIT_SUCCESS if any matching files are found, or with EXIT_FAILURE otherwise.

- -l 	duplicates lists all duplicate files found. Each line of output consists of the relative pathnames of two or more files that are duplicates of each other. The pathnames of duplicate files (on the same line line) must be separated by the TAB character.

- -q 	duplicates executes quietly, simply testing if the named directory contains any duplicate files.
duplicates -q produces no output at all, simply terminating with EXIT_SUCCESS if there are no duplicates (i.e. storage is already minimized), or with EXIT_FAILURE otherwise. 
