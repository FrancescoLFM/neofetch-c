#ifndef UTILS_NEOFETCH_H
#define UTILS_NEOFETCH_H

#include <stdio.h>
#include <stdint.h>

#define LEFT                0
#define RIGHT               1
#define MAX_CHARLINE        125

#define CURS_ADJ(str)       (str--)
#define ARRAYDIM(array, type) (sizeof(array)/sizeof(type))

struct strarr {
    char **array;
    size_t len;
};

struct time {
    uint16_t seconds;
    uint16_t minutes;
    uint32_t hours;
};

char *strsel(struct strarr *ptr, size_t n);
struct strarr *strsplit(char* str, const char *delim);
struct strarr *read_lines(FILE *fp, struct strarr *line_selector);
void free_strarr(struct strarr *ptr);
char *strstrip(char *s, const char *c);
void convert_uptime(struct time *ctime, size_t s);
struct time *init_time();
char *read_line(FILE *fp, char *word_selector, size_t nmemb);
char *timetostr(struct time *ptr);

#endif