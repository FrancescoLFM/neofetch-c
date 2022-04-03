#include <include/utils.h>
#include <stdlib.h>

char *get_distro() 
{   
    FILE *distrofile;
    char *distroline, *distroname, *distro;

    distrofile = fopen("/etc/os-release", "r");
    distroline = read_line(distrofile, "NAME", 4);
    distro = strsel(strsplit(distroline, "="), RIGHT);
    
    if(distroline != NULL) {
        distroname = strstrip(distro, "\"\n");
        free(distroline);
        free(distro);
        fclose(distrofile);

        return distroname;
    }
    return NULL;
}