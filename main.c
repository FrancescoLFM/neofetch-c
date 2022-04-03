#include <stdio.h>
#include <stdlib.h>

#include <include/neofetch.h>
#include <include/cpu.h>
#include <include/utils.h>
#include <include/distro.h>

#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <cpuid.h>


int main()
{
    __uint64_t usedram;
    struct utsname *os, os_info;
    struct time *uptime, uptime_info; 
    struct cpu *processor = malloc(sizeof(struct cpu));
    struct sysinfo *system, sys_info;
    char *uptimestr;
    char *distroname;
    int error_flag = 0;

    system = &sys_info;
    os = &os_info;
    uptime = &uptime_info;

    error_flag = sysinfo(system);
    if (!~error_flag) {
        fputs("An error occured trying to get system informations\n", stderr);
        return SYS_ERR;
    }
    error_flag = uname(os);
    if (!~error_flag) {
        fputs("An error occured trying to get os informations\n", stderr);
        return OS_ERR;
    }
    error_flag = init_cpu(processor);
    if (!~error_flag) {
        fputs("An error occured trying to get cpu informations\n", stderr);
        return CPU_ERR;
    }
    distroname = get_distro();
    if (!*distroname) {
        fputs("No distro recognized\n", stderr);
        return DISTRO_ERR;
    }

    convert_uptime(uptime, system->uptime);
    uptimestr = timetostr(uptime);
    usedram = (system->totalram - system->freeram) * system->mem_unit / 1000000;
    system->totalram /= 1000000 * system->mem_unit;

    printf("\033[0;32m%s\n", os->nodename);
    puts("------------------------------------------------------------\033[0m");
    printf("OS:\t\t%s\n", distroname);
    printf("Kernel:\t\t%s\n", os->release);
    printf("Uptime:\t\t%s\n", uptimestr);
    printf("CPU:\t\t%s (%d)\n", processor->modelname, processor->cores);
    printf("Memory:\t\t%luMB/%luMB\n", usedram, system->totalram);

    free_cpuinfo(processor);
    free(distroname);
    free(uptimestr);

    return SUCCESS;
}