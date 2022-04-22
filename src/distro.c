#include <include/utils.h>
#include <stdlib.h>
#include <string.h>
#include <include/distro.h>

int fetch_distrodraw(char *distroname, struct strarr **distrodraw) 
{
    FILE *infile;
    uint8_t done = 0;
    size_t size = strlen(distroname);
    char *filename = malloc(size + 15);
    if (filename == NULL)
        return -1;

    sprintf(filename, "distros/%s.txt", distroname);

    infile = fopen(filename, "r");
    if (infile == NULL) {
        free(filename);
        return -1;
    }

    *distrodraw = alloc_strarr(DRAWROWS, DRAWCOLUMNS);
    if (*distrodraw == NULL) {
        free(filename);
        fclose(infile);
        return -1;
    }

    for (size_t i=0; i < (*distrodraw)->len && !done; i++) {
        done = fgets((*distrodraw)->array[i], DRAWCOLUMNS, infile) == NULL;
        if (!done) {
            char *temp = strstrip((*distrodraw)->array[i], "\n");
            free((*distrodraw)->array[i]);
            (*distrodraw)->array[i] = temp;
        }
    }

    free(filename);
    fclose(infile);
    return 0;
}

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
