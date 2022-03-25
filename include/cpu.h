#ifndef CPU_NEOFETCH_H
#define CPU_NEOFETCH_H

#define FREQUENCY           0
#define CORES               1

struct cpu {
    int cores;
    int frequency;
    char *modelname;
    char *vendor;
};

// Fetch
struct strarr *get_cpuinfo();
void free_cpuinfo(struct cpu *ptr);

char *init_devicename();
int init_frequency(char *frequency);
int init_cores(char *cores);
int init_cpu(struct cpu *ptr);

#endif