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

void free_cpuinfo(struct cpu *ptr)
{
    if (ptr->modelname != NULL)
        free(ptr->modelname);
    
    free(ptr);
}
