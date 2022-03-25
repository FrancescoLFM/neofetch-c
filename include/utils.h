#ifndef UTILS_NEOFETCH_H
#define UTILS_NEOFETCH_H

#include <stdio.h>

#define LEFT                0
#define RIGHT               1
#define MAX_CHARLINE        125

#define ARRAYDIM(array, type) (sizeof(array)/sizeof(type))

struct strarr {
    char **array;
    size_t len;
};

struct time {
    unsigned short seconds;
    unsigned short minutes;
    unsigned hours;
};

char *strsel(struct strarr *ptr, size_t n);
struct strarr *strsplit(char* str, const char *delim);
struct strarr *read_lines(FILE *fp, struct strarr *line_selector);
void free_strarr(struct strarr *ptr);
char *strstrip(char *s);
void convert_uptime(struct time *ctime, size_t s);
struct time *init_time();

#endif