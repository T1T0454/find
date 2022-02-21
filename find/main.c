#include "parseArgs.h"
#include "sort.h"
#include <dirent.h>
#include <pwd.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

char *allocString(size_t length)
{
    /**
     * allocate enough space to new string
     *
     * @param length    len of string
     */
    return (char *) malloc(sizeof(char) * (length + 1));
}

char *dynamicStringCopy(const char *str)
{
    /**
     * makes copy of string to new allocated memory
     * if alloc fails return NULL
     *
     * @param *str  pointer to original string
     */
    if (!str) {
        return NULL;
    }

    size_t size = strlen(str);

    char *buffer = allocString(size);
    if (!buffer) {
        return NULL;
    }

    strncpy(buffer, str, size);
    buffer[size] = '\0';

    return buffer;
}

void errorMessages(int arg, char *error)
{
    /**
     * prints some error message according to number
     *
     * @param arg       specific number of argument
     * @param error     additional text
     */
    switch (arg) {
    case 1:
        fprintf(stderr, "%s: No such comparator\n", error);
        break;
    case 2:
        fprintf(stderr, "%s: Invalid permission mask\n", error);
        break;
    case 3:
        fprintf(stderr, "%s: Invalid depth level!\n", error);
        break;
    case 4:
        fprintf(stderr, "%s: No such user!\n", error);
        break;
    case 5:
        fprintf(stderr, "Allocation failed!\n");
        break;
    case 6:
        fprintf(stderr, "Reallocation failed!\n");
        break;
    case 7:
        fprintf(stderr, "%s: Permission denied\n", error);
        break;
    case 8:
        fprintf(stderr, "%s: Couldn't read file info!\n", error);
        break;
    case 9:
        fprintf(stderr, "No such file or directory!\n");
        break;
    case 10:
        fprintf(stderr, "%s: Can't open directory!\n", error);
        break;
    default:
        fprintf(stderr, "Something went wrong!\n");
    }
}

int8_t addToFiles(char ***allFiles, char *path, int *numberOfLines)
{
    /**
     * reallocate allFiles and allocate enough space for newly added path
     *
     * @return      0 - alloc fails, 1 - everything is ok
     */
    char **pOld = NULL;
    pOld = realloc(*allFiles, (*numberOfLines + 1) * sizeof(char *));

    if (pOld == NULL) {
        errorMessages(6, "");
        return 0;
    }

    *allFiles = pOld;


    (*allFiles)[*numberOfLines] = dynamicStringCopy(path);
    if ((*allFiles)[*numberOfLines] == NULL) {
        errorMessages(5, "");
        return 0;
    }

    (*numberOfLines)++;
    free(path);

    return 1;
}

char *createCurrentPath(char *path, char *name)
{
    /**
     * creates path for actual directory or file
     *
     * @param path      path to parent
     * @param name      name of dir or file
     */
    char *curr;
    curr = allocString(strlen(path) + strlen(name) + 1);
    if (curr == NULL) {
        errorMessages(5, "");
        return NULL;
    }

    strcpy(curr, path);
    if (curr[strlen(path) - 1] != '/')
        strcat(curr, "/");
    strcat(curr, name);

    return curr;
}

int getPermissions(struct stat *file)
{
    /**
     * return value of file's permissions
     *
     * @param file      file to search
     */
    int res = 0;

    (file->st_mode & S_IRUSR) ? (res += 400) : (res += 0);
    (file->st_mode & S_IWUSR) ? (res += 200) : (res += 0);
    (file->st_mode & S_IXUSR) ? (res += 100) : (res += 0);
    (file->st_mode & S_IRGRP) ? (res += 40) : (res += 0);
    (file->st_mode & S_IWGRP) ? (res += 20) : (res += 0);
    (file->st_mode & S_IXGRP) ? (res += 10) : (res += 0);
    (file->st_mode & S_IROTH) ? (res += 4) : (res += 0);
    (file->st_mode & S_IWOTH) ? (res += 2) : (res += 0);
    (file->st_mode & S_IXOTH) ? (res += 1) : (res += 0);

    return res;
}

int checkConditions(struct stat *file, PARA *para, char *currentPath, char *entryName, int depth)
{
    /**
     * check necessary conditions according to user's switches
     * if some of those isn't completed returns 0 and skips iteration
     *
     * @param file      file's info
     * @param para      user's search parameters
     * @currentPath     actual path
     * @entryName       name of file or directory
     * @depth           recursive immersion
     */
    // if it isn't file or name doesn't contain necessary substring, skip iteration
    if (!S_ISREG(file->st_mode)) {
        free(currentPath);
        return 0;
    }

    // check if file's name contains necessary substring
    if (strstr(entryName, para->name) == NULL) {
        free(currentPath);
        return 0;
    }

    if (depth < para->minDepth || depth >= para->maxDepth) {
        free(currentPath);
        return 0;
    }

    // check if file is owned by user
    struct passwd *pw = getpwuid(file->st_uid);
    if (para->checkUser && strcmp(para->user, pw->pw_name) != 0) {
        free(currentPath);
        return 0;
    }

    // check if file has required permissions
    int per = getPermissions(file);
    if (para->checkMask && para->mask != per) {
        free(currentPath);
        return 0;
    }

    return 1;
}

int8_t list(char *path, char ***allFiles, int *numberOfLines, PARA *para, int depth)
{
    /**
     * @param allFiles          array of pointers to paths
     * @param *path             path of directory
     * @param numberOfLines     count of lines starting with 0
     * @param para              argument
     * @param depth             actual depth
     */

    // try to open source dir
    DIR *dir = opendir(path);
    if (dir == NULL) {
        errorMessages(7, path);
        return 0;
    }

    struct dirent *ent;
    char *currentPath = NULL;
    struct stat file;

    // read everything in directory
    while ((ent = readdir(dir)) != NULL) {

        char *entryName = ent->d_name;

        // check for parent or actual and if u want to show hidden files
        if ((!strcmp(entryName, ".") || !strcmp(entryName, "..")) || (!para->all && entryName[0] == '.')) {
            continue;
        }

        // create a new path to file or folder
        currentPath = createCurrentPath(path, entryName);
        if (currentPath == NULL) {
            closedir(dir);
            return 0;
        }

        // gets file's info
        if (lstat(currentPath, &file) < 0) {
            errorMessages(8, currentPath);
            free(currentPath);
            continue;
        }

        // if directory, recursive call
        if (S_ISDIR(file.st_mode)) {
            list(currentPath, allFiles, numberOfLines, para, depth + 1);
        }

        // check user's conditions
        if (!checkConditions(&file, para, currentPath, entryName, depth)) {
            continue;
        }

        // append path to list
        if (!addToFiles(allFiles, currentPath, numberOfLines)) {
            closedir(dir);
            return 0;
        }
    }

    closedir(dir);
    return 1;
}

void createList(char *path, PARA *para)
{
    /**
     * initialize necessary variables
     * creates list of paths and then sort it
     * print them in correct order and release every path
     * release source path
     *
     * @param path      source path
     * @param para      user's arguments
     */

    int numberOfLines = 0;
    char **allFiles = malloc(sizeof(char *));

    list(path, &allFiles, &numberOfLines, para, 0);

    sort(para->sort, allFiles, numberOfLines);

    for (int i = 0; i < numberOfLines; i++) {
        printf("%s", allFiles[i]);
        putchar(para->nullNewLine);
        free(allFiles[i]);
    }

    free(allFiles);
    free(path);

}

int main(int argc, char *argv[])
{

    // default settings for switchers
    PARA para;
    initializePara(&para);

    if (!loadArguments(argc, argv, &para)) {
        freePara(&para);
        return 1;
    }

    char *path = getFilePath(argc, argv);
    if (path == NULL) {
        freePara(&para);
        return 1;
    }

    createList(path, &para);
    freePara(&para);

    return 0;
}
