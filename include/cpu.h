#ifndef CPU_NEOFETCH_H
#define CPU_NEOFETCH_H

#define DEVICENAME          0
#define FREQUENCY           1
#define CORES               2

struct cpu {
    int cores;
    int frequency;
    char* modelname;
};

// Fetch
struct strarr *get_cpuinfo();
void free_cpuinfo(struct cpu *ptr);

char *init_devicename(char *device_name);
int init_frequency(char *frequency);
int init_cores(char *cores);
int init_cpu(struct cpu *ptr);

#endif