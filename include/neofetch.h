#ifndef NEOFETCH_NEOFETCH_H
#define NEOFETCH_NEOFETCH_H

#include <include/utils.h>

#define FINFO_ERR   -6
#define UPTIME_ERR  -5
#define DISTRO_ERR  -4
#define OS_ERR      -3
#define SYS_ERR     -2
#define CPU_ERR     -1
#define SUCCESS     0

void printffetch(struct strarr *distro, struct strarr *fetchinfo);

#endif