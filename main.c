#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

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
    uint64_t used_ram;
    struct utsname os_info;
    struct cpu *processor;
    struct sysinfo sys_info;
    char *distroname;
    char *uptimestr = NULL;
    struct strarr *fetchinfo = NULL;
    struct strarr *distrodraw = NULL;
    int error_flag = SUCCESS;

    /**
     * Initialization
     */

    error_flag = sysinfo(&sys_info);
    if (!~error_flag) {
        fputs("[ERROR] An error occured trying to get system informations\n", stderr);
        return SYS_ERR;
    }

    error_flag = uname(&os_info);
    if (!~error_flag) {
        fputs("[ERROR] An error occured trying to get os informations\n", stderr);
        return OS_ERR;
    }

    distroname = get_distro();
    if (!distroname || !*distroname) {
        fputs("[ERROR] No distro recognized\n", stderr);
        return DISTRO_ERR;
    }

    processor = malloc(sizeof(struct cpu));
    error_flag = processor ? init_cpu(processor) : -1;
    if (!~error_flag) {
        fputs("[ERROR] An error occured trying to get cpu informations\n", stderr);
        error_flag = CPU_ERR;
        goto free_distroname;
    }

    uptimestr = init_uptimestr(sys_info.uptime);
    if (!uptimestr) {
        fputs("[ERROR] Failed to initialize uptime string\n", stderr);
        error_flag = UPTIME_ERR;
        goto free_processor;
    }

    error_flag = fetch_distrodraw(distroname, &distrodraw);
    if (error_flag) {
        fputs("[ERROR] Failed to retrieve distro draw\n", stderr);
        error_flag = DISTRO_ERR;
        goto free_uptimestr;
    }
    
    fetchinfo = alloc_strarr(7, 50);
    if (!fetchinfo) {
        fputs("[ERROR] Failed to allocate fetch info\n", stderr);
        error_flag = FINFO_ERR;
        goto free_distrodraw;
    }
    /**
     * Initialize fetch info
     */
    {
        char **fetcharr = fetchinfo->array;

        used_ram = (sys_info.totalram - sys_info.freeram) * sys_info.mem_unit / 1000000;
        sys_info.totalram /= 1000000 * sys_info.mem_unit;
        
        sprintf(fetcharr[0], BLU "%s" reset, os_info.nodename);
        strcpy(fetcharr[1], "------------------------");
        sprintf(fetcharr[2], "OS: %s", distroname);
        sprintf(fetcharr[3], "Kernel: %s", os_info.release);
        sprintf(fetcharr[4], "Uptime: %s", uptimestr);
        sprintf(fetcharr[5], "CPU: %s", processor->modelname);
        sprintf(fetcharr[6], "Memory: %luMB/%luMB", used_ram, sys_info.totalram);
    }

    printffetch(distrodraw, fetchinfo);

free_distroname:
    free(distroname);
free_processor:
    free_cpuinfo(processor);
free_uptimestr:
    free(uptimestr);
free_distrodraw:
    free_strarr(distrodraw);
    free_strarr(fetchinfo);

    return error_flag;
}