#include "sort.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>

char *getFileName(const void *path)
{
    /**
     * gets tokens of string by slicing of char '/'
     *
     * @param path      source path
     * @return          pointer to file name
     */

    char *copy = dynamicStringCopy(path);
    char *tmp = strtok(copy, "/");
    char *prev = tmp;

    while (tmp != NULL) {
        prev = tmp;
        tmp = strtok(NULL, "/");
    }

    prev = dynamicStringCopy(prev);
    free(copy);

    return prev;
}

int nameCmp(char *a, char *b)
{
    /**
     * firstly compare file names
     * if matching compare whole path
     *
     * @param a     one of paths
     * @param b     one of paths
     */
    char *first = getFileName(a);
    char *second = getFileName(b);

    int res = strcasecmp(first, second);

    free(first);
    free(second);
    if (res != 0) {
        return res;
    }

    return strcmp(a, b);
}

size_t getFileSize(char *path)
{
    /**
     * gets file's size
     *
     * @param path      path to file
     * @return          size of file
     */
    struct stat file;
    lstat(path, &file);
    return file.st_size;
}

int sizeCmp(char *a, char *b)
{
    /**
     * get file's size and compare it
     * if sizes are equal do comparison based on name
     *
     * @param a     one of paths
     * @param b     one of paths
     */
    size_t first = getFileSize(a);
    size_t second = getFileSize(b);

    if (first < second) {
        return 1;
    }
    if (second < first) {
        return -1;
    }

    return nameCmp(a, b);

}

void lexicographicSort(char **allFiles, int numberOfLines)
{
    /**
     * basic sort if there isn't used switch -s
     *
     * @param allFiles          array of pointers to paths
     * @param numberOfLines     count of paths in array
     */
    char *temp = NULL; // temporary string
    for (int i = 0; i < numberOfLines; i++) {
        for (int j = i + 1; j < numberOfLines; j++) {

            if (nameCmp(allFiles[i], allFiles[j]) > 0) {
                free(temp);
                temp = dynamicStringCopy(allFiles[i]);
                free(allFiles[i]);
                allFiles[i] = dynamicStringCopy(allFiles[j]);
                free(allFiles[j]);
                allFiles[j] = dynamicStringCopy(temp);
            }
        }
    }
    free(temp);
}

void fSort(char **allFiles, int numberOfLines)
{
    /**
     * sorting based on whole path
     * it's case sensitive
     *
     * @param allFiles          array of pointers to paths
     * @param numberOfLines     count of paths in array
     */
    char *temp = NULL; // temporary string
    for (int i = 0; i < numberOfLines; i++) {
        for (int j = i + 1; j < numberOfLines; j++) {
            if (strcmp(allFiles[i], allFiles[j]) > 0) {
                free(temp);
                temp = dynamicStringCopy(allFiles[i]);
                free(allFiles[i]);
                allFiles[i] = dynamicStringCopy(allFiles[j]);
                free(allFiles[j]);
                allFiles[j] = dynamicStringCopy(temp);
            }
        }
    }
    free(temp);
}

void sSort(char **allFiles, int numberOfLines)
{
    /**
     * sorting base on file's size
     * if sizes are equal it compares files names
     *
     * @param allFiles          array of pointers to paths
     * @param numberOfLines     count of paths in array
     */
    char *temp = NULL; // temporary string
    for (int i = 0; i < numberOfLines; i++) {
        for (int j = i + 1; j < numberOfLines; j++) {
            if (sizeCmp(allFiles[i], allFiles[j]) > 0) {
                free(temp);
                temp = dynamicStringCopy(allFiles[i]);
                free(allFiles[i]);
                allFiles[i] = dynamicStringCopy(allFiles[j]);
                free(allFiles[j]);
                allFiles[j] = dynamicStringCopy(temp);
            }
        }
    }
    free(temp);
}

void sort(int type, char **allFiles, int numberOfLines)
{
    switch (type) {
    case 2:
        sSort(allFiles, numberOfLines);
        break;
    case 1:
        fSort(allFiles, numberOfLines);
        break;
    default:
        lexicographicSort(allFiles, numberOfLines);
        break;
    }
}
