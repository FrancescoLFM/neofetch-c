#define DEVICENAME          0
#define FREQUENCY           1
#define CORES               2

struct cpu {
    int cores;
    int frequency;
    char* modelname;
};

// Fetch
char *init_devicename(char *device_name);
int init_frequency(char *frequency);
int init_cores(char *cores);

int get_cpuinfo(struct cpu *cpu); 
void free_cpuinfo(struct cpu *ptr);