#include "parseArgs.h"
#include <dirent.h>
#include <getopt.h>
#include <limits.h>
#include <pwd.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void initializePara(PARA *parameter)
{
    /**
     * initialize struct parameters to default values
     *
     * @param parameter     struct to initialize
     */
    parameter->name = dynamicStringCopy("");
    parameter->sort = 0;
    parameter->checkMask = 0;
    parameter->mask = 0;
    parameter->checkUser = 0;
    parameter->user = NULL;
    parameter->minDepth = 0;
    parameter->maxDepth = UINT_MAX;
    parameter->all = 0;
    parameter->nullNewLine = '\n';
}

void freePara(PARA *parameter)
{
    /**
     * free necessary things of struct
     *
     * @param parameter     struct to free
     */
    free(parameter->name);
    free(parameter->user);
}

char *getFilePath(int argc, char *argv[])
{
    /**
     * go through all arguments and check if there is path and if it's directory, if no, path will be set
     * to actual directory
     *
     * @param argc      number of arguments
     * @param *argv[]   array of pointers to each argument
     */

    int i = 1;
    while (i < argc) {
        if (strcmp(argv[i], "-n") == 0 || strcmp(argv[i], "-m") == 0 ||
                strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "-u") == 0 ||
                strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "-t") == 0) {
            i += 2;
        } else if (strcmp(argv[i], "-a") == 0 || strcmp(argv[i], "-0") == 0) {
            i++;
        } else if (argv[i][0] != '-') {
            struct stat file;
            lstat(argv[i], &file);

            if (!S_ISDIR(file.st_mode)) {
                errorMessages(9, "");
                return NULL;
            }

            DIR *pDir = NULL;
            pDir = opendir(argv[i]);
            if (pDir == NULL) {
                errorMessages(10, argv[i]);
                return NULL;
            }
            closedir(pDir);
            return dynamicStringCopy(argv[i]);
        } else
            i += 1;
    }

    return dynamicStringCopy(".");
}

int8_t loadArguments(int argc, char *argv[], PARA *para)
{
    /**
     * goes through all arguments and if finds switch adds it to struct para
     */

    int opt;

    while ((opt = getopt(argc, argv, "n:s:m:u:f:t:ah0")) != -1) {
        switch (opt) {
        case 'n':
            free(para->name);
            para->name = dynamicStringCopy(optarg);
            break;
        case 's':
            if (strcmp(optarg, "f") == 0) {
                para->sort = 1;
            } else if (strcmp(optarg, "s") == 0) {
                para->sort = 2;
            } else {
                errorMessages(1, optarg);
                return 0;
            }
            break;
        case 'm':
            para->checkMask = 1;
            para->mask = strtol(optarg, NULL, 10);
            if ((void *) para->mask == NULL && strcmp(optarg, "0") != 0) {
                errorMessages(2, optarg);
                return 0;
            }
            break;
        case 'u':
            para->checkUser = 1;
            if (getpwnam(optarg) == NULL) {
                errorMessages(4, optarg);
                return 0;
            }
            free(para->user);
            para->user = dynamicStringCopy(optarg);
            break;
        case 'f':
            para->minDepth = strtol(optarg, NULL, 10);
            if ((void *) para->minDepth == NULL && strcmp(optarg, "0") != 0) {
                errorMessages(3, optarg);
                return 0;
            }
            break;
        case 't':
            para->maxDepth = strtol(optarg, NULL, 10);
            if ((void *) para->maxDepth == NULL && strcmp(optarg, "0") != 0) {
                errorMessages(3, optarg);
                return 0;
            }
            break;
        case 'a':
            para->all = 1;
            break;
        case '0':
            para->nullNewLine = '\0';
            break;
        case 'h':
            fprintf(stderr, "-n ATTR (search based on a substring in the file name)\n"
                            "-s f|s (sort the list of paths by full path (f) or size (s))\n"
                            "-m MASK (searching for a file based on access rights)\n"
                            "-u USER (search for a file based on username)\n"
                            "-f NUM (minimum distance of nodes from the root)\n"
                            "-t NUM (maximum distance of nodes from the root)\n"
                            "-a (also searches in hidden files and subdirectories)\n"
                            "-0 (use '\\0' instead of '\\n')\n"
                            "-h (print help)");
            return 0;
        case '?':
            return 0;
        default:
            printf("something went wrong!\n");
            return 0;
        }
    }
    return 1;
}
