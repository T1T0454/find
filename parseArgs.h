#include <dirent.h>
#include <stdint.h>
#include <stddef.h>

#ifndef HW05_PARSEARGS_H
#define HW05_PARSEARGS_H

typedef struct
{
    char *name;
    int sort;
    int checkMask;
    long mask;
    int checkUser;
    char *user;
    long minDepth;
    long maxDepth;
    int all;
    char nullNewLine;
} PARA;

char *allocString(size_t length);

char *dynamicStringCopy(const char *str);

void initializePara(PARA *parameter);

void freePara(PARA *parameter);

void errorMessages(int arg, char *error);

char *getFilePath(int argc, char *argv[]);

int8_t loadArguments(int argc, char *argv[], PARA *para);

#endif //HW05_PARSEARGS_H
