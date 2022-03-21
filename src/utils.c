#include <include/utils.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Returns how many times a character is repeated in a string
int charcount(char *str, const char c) 
{
    int n = 0;
    
    for(int i=0; str[i]; i++)
        if(str[i] == c) n++;
    
    return n;
}

struct strarr *strsplit(char* str, const char *delim)
{
    char *token;
    struct strarr *strings;
    int ndelim = charcount(str, delim[0]);

    strings = malloc(sizeof(struct strarr));
    if (strings == NULL)
        return NULL;
    strings->array = calloc(ndelim+1, sizeof(char *));
    if (strings->array == NULL) {
        free(strings);
        return NULL;
    }
    strings->len = 0;

    for (int i=0; ; i++, str = NULL) {
        token = strtok(str, delim);
        if (token == NULL)
            break;
        
        strings->array[i] = strdup(token);
        if (strings->array[i] == NULL) {
            free_strarr(strings);
            return NULL;
        }
        strings->len++;
    }
    
    return strings;
}

char *strsel(struct strarr *ptr, size_t n) 
{
    char *str;

    if (n >= ptr->len)
        return NULL;
    
    str = strdup(ptr->array[n]);
    free_strarr(ptr);
    if (str == NULL)
        return NULL;

    return str;
}

struct strarr *read_lines(FILE *fp, int *line_selector, size_t nmemb)
{
    struct strarr *lines;
    char buffer[MAX_CHARLINE];

    lines = malloc(sizeof(struct strarr));
    if (lines == NULL) 
        return NULL;
    lines->array = calloc(nmemb, sizeof(char *));
    if (lines->array == NULL) {
        free(lines);
        return NULL;
    }

    lines->len = 0;

    // Stops when reached eof or there is no more values to be analyzed in line_selector
    for (int line=0; fgets(buffer, sizeof(buffer), fp) && *line_selector; line++) {
        if (line == *line_selector) {
            lines->array[lines->len] = strdup(buffer);
            if (lines->array[lines->len] == NULL) {
                free_strarr(lines);
                return NULL;
            }
            line_selector++;
            lines->len++;
        }
    }

    return lines;
}

void free_strarr(struct strarr *ptr)
{
    for (size_t i=0; i < ptr->len; i++)
        free(ptr->array[i]);

    free(ptr->array);
    free(ptr);
}

// from linux kernel
char *strstrip(char *s) 
{
    size_t size;
        char *end;

        size = strlen(s);

        if (!size)
                return s;

        end = s + size - 1;
        while (end >= s && isspace(*end))
                end--;
        *(end + 1) = '\0';

        while (*s && isspace(*s))
                s++;

        return s;
}

struct time *init_time()
{
    struct time *ptr, clk;
    ptr = &clk;

    ptr->hours = 0;
    ptr->minutes = 0;
    ptr->seconds = 0;

    return ptr;
}

// struct should be initialized with init_time() before calling this fun
void convert_uptime(struct time *ptr, size_t s)
{
    ptr->minutes = s/60;

    if (ptr->minutes) {
        ptr->seconds -= ptr->minutes*60;
        ptr->hours = ptr->minutes/60;
        if (ptr->hours)
            ptr->minutes -= ptr->hours*60;
    }
}
