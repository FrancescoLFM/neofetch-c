#include <include/utils.h>
#include <include/neofetch.h>
#include <include/distro.h>

#include <stdlib.h>

void printffetch(struct strarr *distro, struct strarr *fetchinfo)
{
    for (size_t i=0; i < MAX(distro->len, fetchinfo->len);i++) {
        if (i < distro->len && i < fetchinfo->len)
            printf("%-44s%s\n", distro->array[i], fetchinfo->array[i]);
        else if (i < distro->len)
            printf("%-44s\n", distro->array[i]);
        else
            printf("%-44s%s\n", "", fetchinfo->array[i]);
    }
}
