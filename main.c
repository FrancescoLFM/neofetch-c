#include <stdio.h>
#include <include/neofetch.h>
#include <include/cpu.h>
#include <include/utils.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/sysinfo.h>

int main()
{
    __uint64_t usedram;
    struct time *uptime = init_time();
    struct cpu *processor = malloc(sizeof(struct cpu));
    struct sysinfo *system, sys_info;
    int error_flag = 0;

    system = &sys_info;

    error_flag = init_cpu(processor);
    if (!~error_flag) {
        fputs("An error occured trying to get cpu informations\n", stderr);
        return CPU_ERR;
    }
    error_flag = sysinfo(system);
    if (!~error_flag) {
        fputs("An error occured trying to get system informations\n", stderr);
        return SYS_ERR;
    }

    convert_uptime(uptime, system->uptime);
    usedram = (system->totalram - system->freeram) * system->mem_unit / 1000000;
    system->totalram /= 1000000 * system->mem_unit;

    printf("Uptime:\t\t %d h %d m %d s\nCPU:\t\t%sFrequency:\t %dMHz\nCore number:\t %d\nRAM:\t\t %ldMb/%ldMb\n",
           uptime->hours, uptime->minutes, uptime->seconds,
           processor->modelname, processor->frequency, processor->cores,
           usedram, system->totalram);

    free_cpuinfo(processor);
    return SUCCESS;
}