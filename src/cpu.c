#include <include/cpu.h>
#include <include/utils.h>
#include <stdlib.h>

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

struct strarr *get_cpuinfo() 
{
    char *lines[] = {"model name", "cpu MHz", "cpu cores", NULL};
    struct strarr infoptr = {
        .array = lines,
        .len = 4
    };
    struct strarr *cpuinfo;

    FILE *infofp = fopen("/proc/cpuinfo", "r");
    if (infofp == NULL) 
        return NULL;

    cpuinfo = read_lines(infofp, &infoptr);
    if (cpuinfo == NULL || cpuinfo->array[0] == NULL) { 
        fclose(infofp);
        return NULL;
    }

    fclose(infofp);
    return cpuinfo;
}

int init_cpu(struct cpu *ptr)
{
    struct strarr *cpuinfo = get_cpuinfo();
    if (cpuinfo == NULL) {
        return -1;
    }
    char *devicename = init_devicename(cpuinfo->array[DEVICENAME]);
    int frequency = init_frequency(cpuinfo->array[FREQUENCY]);
    int cores = init_cores(cpuinfo->array[CORES]);

    ptr->modelname = devicename;
    ptr->frequency = frequency;
    ptr->cores = cores;

    free_strarr(cpuinfo);

    return 0;
}

void free_cpuinfo(struct cpu *ptr)
{
    if (ptr->modelname != NULL)
        free(ptr->modelname);
    
    free(ptr);
}
