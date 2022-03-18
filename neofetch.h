#define DEVICENAME_LINE     4
#define FREQUENCY_LINE      7
#define CORES_LINE          12

#define DEVICENAME          0
#define FREQUENCY           1
#define CORES               2

#define LEFT                0
#define RIGHT               1
#define MAX_CHARLINE        125

struct cpu {
    int cores;
    int frequency;
    char* modelname;
};

struct memory {
    int totalmem;
    int freemem;
};

