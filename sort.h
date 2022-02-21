#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>

#ifndef HW05_SORT_H
#define HW05_SORT_H

char *allocString(size_t length);

char *dynamicStringCopy(const char *str);

char *getFileName(const void *path);

int nameCmp(char *a, char *b);

size_t getFileSize(char *path);

int sizeCmp(char *a, char *b);

void lexicographicSort(char **allFiles, int numberOfLines);

void fSort(char **allFiles, int numberOfLines);

void sSort(char **allFiles, int numberOfLines);

void sort(int type, char **allFiles, int numberOfLines);

#endif //HW05_SORT_H
