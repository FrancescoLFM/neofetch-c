#ifndef UTILS_NEOFETCH_H
#define UTILS_NEOFETCH_H

#include <stdio.h>
#include <stdint.h>

#define LEFT                0
#define RIGHT               1
#define MAX_CHARLINE        125

#define MAX(A, B)           ((A) > (B) ? (A) : (B))
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
char *strstrip(char *s, const char *c);

struct strarr *read_lines(FILE *fp, struct strarr *line_selector);
char *read_line(FILE *fp, char *word_selector, size_t nmemb);

struct strarr *alloc_strarr(size_t rows, size_t maxlen);
struct strarr *alloc_sstrarr(size_t rows);
void free_sstrarr(struct strarr *ptr);
void free_strarr(struct strarr *ptr);

void convert_uptime(struct time *ctime, long s);
struct time *init_time();
char *init_uptimestr(long sys_uptime);

char *timetostr(struct time *ptr);

#endif