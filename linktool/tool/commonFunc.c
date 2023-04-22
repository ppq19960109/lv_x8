#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <limits.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>

#include "commonFunc.h"

void cjson_to_str(cJSON *in, char *out)
{
    if (in->valuestring != NULL)
    {
        strcpy(out, in->valuestring);
    }
    else
    {
        sprintf(out, "%d", in->valueint);
    }
}

int split_str(char *str, const char *delim, void (*cb)(const char *, unsigned int))
{
    char *token = strtok(str, delim);
    int i = 0;
    for (i = 0; token != NULL; ++i)
    {
        cb(token, i);
        token = strtok(NULL, delim);
    }
    return i;
}

long getFileSize(const char *path)
{
    struct stat statbuff;
    if (stat(path, &statbuff) < 0)
    {
        return -1;
    }
    else
    {
        return statbuff.st_size;
    }
}

int operateFile(int action, const char *path, char *buf, int len)
{
    if (buf == NULL || len == 0)
        return -1;
    int ret = -1;
    int fd;
    if (action)
    {
        fd = open(path, O_RDWR | O_CREAT, 0777);
        if (fd < 0)
        {
            printf("open %s fail\n", path);
            return -1;
        }
        ret = write(fd, buf, len);
        if (ret < 0)
        {
            printf("write %s fail,%s\n", path, strerror(ret));
        }
    }
    else
    {
        fd = open(path, O_RDWR, 0777);
        if (fd < 0)
        {
            printf("open %s fail\n", path);
            return -1;
        }
        ret = read(fd, buf, len);
        if (ret < 0)
        {
            printf("read %s fail,%s\n", path, strerror(ret));
        }
    }
    close(fd);
    return ret;
}

void readFileList(const char *path, int (*readFileFunc)(const char *))
{
    DIR *dir;
    struct dirent *ptr;
    char base[64] = {0};

    if ((dir = opendir(path)) == NULL)
    {
        perror("Open dir error...");
        return;
    }

    while ((ptr = readdir(dir)) != NULL)
    {
        if (strcmp(ptr->d_name, ".") == 0 || strcmp(ptr->d_name, "..") == 0) ///current dir OR parrent dir
            continue;
        else if (ptr->d_type == 8) ///file
        {
            strcpy(base, path);
            strcat(base, "/");
            strcat(base, ptr->d_name);
            printf("d_name:%s,path:%s\n", ptr->d_name, base);
            readFileFunc(base);
        }
        else if (ptr->d_type == 10) ///link file
            printf("d_name:%s\n", ptr->d_name);
        else if (ptr->d_type == 4) ///dir
        {
            memset(base, 0, sizeof(base));
            strcpy(base, path);
            strcat(base, "/");
            strcat(base, ptr->d_name);
            readFileList(base, readFileFunc);
        }
    }
    closedir(dir);
}

void *get_dev_profile(const char *path, void *input, const char *modelId, void *(*func)(void *, const char *))
{
    char filePath[64] = {0};
    snprintf(filePath, sizeof(filePath), "%s/%s", path, modelId);
    struct stat statfile;
    if (stat(filePath, &statfile) < 0)
    {
        printf("stat %s error\n", filePath);
        return NULL;
    }
    if (statfile.st_size == 0)
    {
        printf("file size 0\n");
        return NULL;
    }
    printf("filePath:%s,file size:%ld\n", filePath, statfile.st_size);
    int fd = open(filePath, O_RDONLY);
    if (fd < 0)
    {
        perror("open file fail\n");
        return NULL;
    }
    void *buf = malloc(statfile.st_size);
    if (buf == NULL)
    {
        goto fail;
    }
    memset(buf, 0, statfile.st_size);

    int res = read(fd, buf, statfile.st_size);
    if (res != statfile.st_size)
    {
        printf("read file size :%d\n", res);
        goto fail;
    }
    close(fd);
    void *dev = func(input, buf);

    free(buf);
    return dev;
fail:
    close(fd);
    if (buf != NULL)
    {
        free(buf);
    }
    return NULL;
}

long *stol(const char *str, int base, long *out)
{
    errno = 0;
    char *endptr;
    long val = strtol(str, &endptr, base);
    if ((errno == ERANGE) || (errno != 0 && val == 0))
    {
        perror("strtol error");
        printf("strtol endptr:%s,%ld\n", endptr, val);
        return NULL;
    }
    if (out != NULL)
    {
        *out = val;
    }
    return out;
}

unsigned long long int *stoull(const char *str, int base, unsigned long long int *out)
{
    errno = 0;
    char *endptr;
    unsigned long long int val = strtoull(str, &endptr, base);
    if (errno != 0)
    {
        perror("stoull error");
        printf("stoull endptr:%s,%lld\n", endptr, val);
        return NULL;
    }
    if (out != NULL)
    {
        *out = val;
    }
    return out;
}

double *stod(const char *str, double *out)
{
    errno = 0;
    char *endptr;
    double val = strtod(str, &endptr);
    if (errno != 0)
    {
        perror("strtod error");
        printf("strtod endptr:%s,%f\n", endptr, val);
        return NULL;
    }
    if (out != NULL)
    {
        *out = val;
        return out;
    }
    return NULL;
}

int findStrIndex(const char *key, char *const *array, const int arrayLen)
{
    if (key == NULL || array == NULL || arrayLen == 0)
        return -1;

    int i;

    for (i = 0; i < arrayLen; i++)
    {
        if (strcmp(*array++, key) == 0)
        {
            return i;
        }
    }

    return -1;
}

int findStrnIndex(const char *key, const int n, char *const *array, const int arrayLen)
{
    if (key == NULL || array == NULL || arrayLen == 0)
        return -1;

    int i;

    for (i = 0; i < arrayLen; i++)
    {
        if (strncmp(*array++, key, n) == 0)
        {
            return i;
        }
    }

    return -1;
}

char getByteForJson(cJSON *json, const char *src, char *dst)
{
    if (json == NULL || src == NULL || dst == NULL)
        return -1;
    cJSON *obj = cJSON_GetObjectItem(json, src);
    if (obj != NULL)
    {
        *dst = atoi(obj->valuestring);
        return *dst;
    }
    return -1;
}

int getNumForJson(cJSON *json, const char *src, int *dst)
{
    if (json == NULL || src == NULL || dst == NULL)
        return -1;
    cJSON *obj = cJSON_GetObjectItem(json, src);
    if (obj != NULL)
    {
        *dst = atoi(obj->valuestring);
        return *dst;
    }
    return -1;
}

char *getStrForJson(cJSON *json, const char *src, char *dst)
{
    if (json == NULL || src == NULL || dst == NULL)
        return NULL;
    cJSON *obj = cJSON_GetObjectItem(json, src);
    if (obj != NULL)
    {
        strcpy(dst, obj->valuestring);
        return dst;
    }
    return NULL;
}
