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

char *read_line(FILE *fp, char *word_selector, size_t nmemb)
{
    char buffer[MAX_CHARLINE];

    for (int l=0; fgets(buffer, sizeof(buffer), fp); l++) {
        if (!strncmp(buffer, word_selector, nmemb))
            return strdup(buffer);
    }

    return NULL;
}

struct strarr *read_lines(FILE *fp, struct strarr *line_selector)
{
    struct strarr *lines;
    char buffer[MAX_CHARLINE];

    lines = malloc(sizeof(struct strarr));
    if (lines == NULL) 
        return NULL;
    lines->array = calloc(line_selector->len, sizeof(char *));
    if (lines->array == NULL) {
        free(lines);
        return NULL;
    }

    lines->len = 0;

    // Stops when reached eof or there is no more values to be analyzed in line_selector
    for (int l=0; fgets(buffer, sizeof(buffer), fp) && *line_selector->array != NULL; l++) {
        if (!strncmp(buffer, *line_selector->array, strlen(*line_selector->array))) {
            lines->array[lines->len] = strdup(buffer);
            if (lines->array[lines->len] == NULL) {
                free_strarr(lines);
                return NULL;
            }
            line_selector->array++;
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

char *strlstrip(char *s, const char *c)
{
    uint8_t found = 1;
    char *ret;

    for (; *s && found; s++) {
        found = 0;
        for (int i=0; c[i] && !found; i++)
            found = (c[i] == *s);
    }
    
    // Retrocedi cursore quando incontra un carattere da non strippare
    if (!found)
        CURS_ADJ(s);
    
    ret = strdup(s);
    return ret;
}

char *strrstrip(char *s, const char *c) 
{
    char *ret = strdup(s);
    uint8_t found = 1;

    for (size_t size = strlen(ret)-1; size && found; size--) {
        found = 0;
        for (int i=0; c[i]; i++) {
            found = (c[i] == ret[size]);
            if (c[i] == ret[size])
                ret[size] = '\0';
        }
    }

    return ret;
}

char *strstrip(char *s, const char *c) 
{
    char *left, *right;
    
    left = strlstrip(s, c);

    if (left) {
        right = strrstrip(left, c);
        free(left);
    }

    return right;
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
    int th; // Truncated hours

    ptr->hours = s/3600;
    th = 3600 * ptr->hours;

    ptr->minutes = (s - th)/60;
    ptr->seconds = (s - th - (60 * ptr->minutes));
}

char *timetostr(struct time *ptr)
{
    char *ret = malloc(16);

    if (ptr->hours)
        sprintf(ret, "%u hours, %u mins", ptr->hours, ptr->minutes);
    else if (ptr->minutes)
        sprintf(ret, "%u mins, %u secs", ptr->minutes, ptr->seconds);
    else
        sprintf(ret, "%u secs", ptr->seconds);
    
    return ret;
}

