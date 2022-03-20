#include <stdio.h>
#include "include/neofetch.h"
#include "include/cpu.h"
#include <include/utils.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    struct cpu *processor = malloc(sizeof(struct cpu));

    get_cpuinfo(processor);
    printf("CPU:\t\t%sFrequency:\t %dMHz\nCore number:\t %d\n", 
           processor->modelname, processor->frequency, processor->cores);
    free_cpuinfo(processor);

    return 0;
}