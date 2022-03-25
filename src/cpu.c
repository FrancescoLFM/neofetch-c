#include <include/cpu.h>
#include <include/utils.h>
#include <stdlib.h>
#include <cpuid.h>
#include <string.h>

char *init_vendor()
{
    __uint32_t vendor[5];
    unsigned int a;

    __get_cpuid(0, &a, vendor, vendor+2, vendor+1);

    char *ret = strdup((char *) vendor);

    return ret;
}

char *init_devicename()
{
    __uint32_t devicename[12];

    __get_cpuid(0x80000002, devicename, devicename+1, devicename+2, devicename+3);
    __get_cpuid(0x80000003, devicename+4, devicename+5, devicename+6, devicename+7);
    __get_cpuid(0x80000004, devicename+8, devicename+9, devicename+10, devicename+11);

    char *ret = strdup((char *) devicename);

    return ret;
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
    char *lines[] = {"cpu MHz", "cpu cores", NULL};
    struct strarr infoptr = {
        .array = lines,
        .len = 3
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
    char *devicename = init_devicename();
    char *vendor = init_vendor();
    int frequency = init_frequency(cpuinfo->array[FREQUENCY]);
    int cores = init_cores(cpuinfo->array[CORES]);

    ptr->modelname = devicename;
    ptr->vendor = vendor;
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
