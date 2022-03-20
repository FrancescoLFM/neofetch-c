#include <stdio.h>

#define LEFT                0
#define RIGHT               1
#define MAX_CHARLINE        125

#define ARRAYDIM(array, type) (sizeof(array)/sizeof(type))

struct strarr {
    char **array;
    size_t len;
};

char *strsel(struct strarr *ptr, size_t n);
struct strarr *strsplit(char* str, const char *delim);
struct strarr *read_lines(FILE *fp, int *line_selector, size_t nmemb);
void free_strarr(struct strarr *ptr);
char *strstrip(char *s);
