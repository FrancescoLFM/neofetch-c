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

void free_strarr(char **strarr)
{
    for (char *str = *strarr; str; str++)
        free(str);

    free(strarr);
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

char **strsplit(char* str, const char *delim)
{
    char *token;
    char **strings;
    int ndelim = charcount(str, delim[0]);

    strings = calloc(ndelim, sizeof(char *));
    if (strings == NULL)
        return NULL;

    for (int i=0; ; i++, str = NULL) {
        token = strtok(str, delim);
        if (token == NULL)
            break;
        
        strings[i] = strdup(token);
        if (strings[i] == NULL) {
            free_strarr(strings);
            return NULL;
        }
    }
    
    return strings;
}

char **read_lines(FILE *fp, int *line_arr)
{
    int cl = 0;
    char **strarr = calloc(sizeof(line_arr), sizeof(int));
    char buffer[MAX_CHARLINE];
    
    if (buffer == NULL) return NULL;
    if (strarr == NULL) return NULL;

    // Stops when reached eof or there is no more values to be analyzed in line_arr
    for (int line=0; fgets(buffer, sizeof(buffer), fp) && cl < (int)(sizeof(line_arr)/2-1); line++) {
        if (line == line_arr[cl]) {
            strarr[cl] = strdup(buffer);
            cl++; 
        }
    }

    return strarr;
}

char *init_devicename(char *dn)
{
    char *devicename = strsplit(dn, ":")[RIGHT]; // Device name
    
    if (devicename == NULL)
        return "No device name";
    return devicename;
}

int init_frequency(char *freq)
{
    char *strfreq = strsplit(freq, ":")[RIGHT]; // Frequency in string
    if (strfreq == NULL)
        return -1;
    return atoi(strfreq);
}

int init_cores(char *cores)
{
    char *strcores = strsplit(cores, ":")[RIGHT];
    if (strcores == NULL)
        return -1;
    return atoi(strcores);
}

int get_cpuinfo(struct cpu *cpu) 
{
    int lines[] = {DEVICENAME_LINE, FREQUENCY_LINE, CORES_LINE};

    FILE *infofp = fopen("/proc/cpuinfo", "r");
    if (infofp == NULL) return -1;

    char **cpuinfo = read_lines(infofp, lines);
    if (cpuinfo == NULL) return -1;

    char *devicename = init_devicename(cpuinfo[DEVICENAME]);
    int frequency = init_frequency(cpuinfo[FREQUENCY]);
    int cores = init_cores(cpuinfo[CORES]);

    cpu->modelname = devicename;
    cpu->frequency = frequency;
    cpu->cores = cores;

    free(cpuinfo);

    return 0;
}

int main()
{
    struct cpu *processor = malloc(sizeof(struct cpu));

    get_cpuinfo(processor);
    printf("CPU:\t\t%sFrequency:\t %dMHz\nCore number:\t %d\n", 
           processor->modelname, processor->frequency, processor->cores);
    free(processor);

    return 0;
}