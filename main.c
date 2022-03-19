#include <stdio.h>
#include "neofetch.h"
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

// Returns how many times a character is repeated in a string
int charcount(char *str, const char c) 
{
    int n = 0;
    
    for(int i=0; str[i]; i++)
        if(str[i] == c) n++;
    
    return n;
}

void free_strarr(struct strarr *ptr)
{
    for (size_t i=0; i < ptr->len; i++)
        free(ptr->array[i]);

    free(ptr->array);
    free(ptr);
}

void free_cpuinfo(struct cpu *ptr)
{
    if (ptr->modelname != NULL)
        free(ptr->modelname);
    
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

char *init_devicename(char *dn)
{
    char *devicename = strsel(strsplit(dn, ":"), RIGHT); // Device name

    return devicename;
}

int init_frequency(char *freq)
{
    int freq_;
    char *strfreq = strsel(strsplit(freq, ":"), RIGHT); // Frequency in string
    if (strfreq == NULL)
        return -1;
    
    freq_ = atoi(strfreq);
    free(strfreq);

    return freq_;
}

int init_cores(char *cores)
{
    return init_frequency(cores);
}

int get_cpuinfo(struct cpu *cpu) 
{
    int lines[] = {DEVICENAME_LINE, FREQUENCY_LINE, CORES_LINE, 0};

    FILE *infofp = fopen("/proc/cpuinfo", "r");
    if (infofp == NULL) 
        return -1;

    struct strarr *cpuinfo = read_lines(infofp, lines, ARRAYDIM(lines, int));
    if (cpuinfo == NULL || cpuinfo->array[0] == NULL) { 
        fclose(infofp);
        return -1;
    }

    char *devicename = init_devicename(cpuinfo->array[DEVICENAME]);
    int frequency = init_frequency(cpuinfo->array[FREQUENCY]);
    int cores = init_cores(cpuinfo->array[CORES]);

    cpu->modelname = devicename;
    cpu->frequency = frequency;
    cpu->cores = cores;

    free_strarr(cpuinfo);
    fclose(infofp);

    return 0;
}

int main()
{
    struct cpu *processor = malloc(sizeof(struct cpu));

    get_cpuinfo(processor);
    printf("CPU:\t\t%sFrequency:\t %dMHz\nCore number:\t %d\n", 
           processor->modelname, processor->frequency, processor->cores);
    free_cpuinfo(processor);

    return 0;
}