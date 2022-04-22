#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <include/neofetch.h>
#include <include/cpu.h>
#include <include/utils.h>
#include <include/distro.h>
#include <include/colors.h>

#include <sys/utsname.h>
#include <sys/sysinfo.h>
#include <cpuid.h>


int main()
{
    __uint64_t usedram;
    struct utsname *os, os_info;
    struct time *uptime; 
    struct cpu *processor;
    struct sysinfo *system, sys_info;
    char *distroname;
    char *uptimestr;
    struct strarr *fetchinfo;
    struct strarr *distrodraw;
    int error_flag = 0;

    // fetchalloc(fetchinfo); broken
    system = &sys_info;
    os = &os_info;
    uptime = init_time();

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
    distroname = get_distro();
    if (!*distroname) {
        fputs("No distro recognized\n", stderr);
        return DISTRO_ERR;
    }

    processor = malloc(sizeof(struct cpu));
    error_flag = init_cpu(processor);
    if (!~error_flag) {
        fputs("An error occured trying to get cpu informations\n", stderr);
        return CPU_ERR;
    }

    convert_uptime(uptime, system->uptime);
    uptimestr = timetostr(uptime);
    usedram = (system->totalram - system->freeram) * system->mem_unit / 1000000;
    system->totalram /= 1000000 * system->mem_unit;

    fetch_distrodraw(distroname, &distrodraw);
    
    fetchinfo = alloc_strarr(7, 50);

    char **fetcharr = fetchinfo->array;

    sprintf(fetcharr[0], BLU "%s" reset, os->nodename);
    strcpy(fetcharr[1], "------------------------");
    sprintf(fetcharr[2], "OS: %s", distroname);
    sprintf(fetcharr[3], "Kernel: %s", os->release);
    sprintf(fetcharr[4], "Uptime: %s", uptimestr);
    sprintf(fetcharr[5], "CPU: %s", processor->modelname);
    sprintf(fetcharr[6], "Memory: %luMB/%luMB", usedram, system->totalram);

    printffetch(distrodraw, fetchinfo);

    free_strarr(fetchinfo);
    free_strarr(distrodraw);
    free_cpuinfo(processor);

    free(uptimestr);
    free(uptime);
    free(distroname);

    return SUCCESS;
}