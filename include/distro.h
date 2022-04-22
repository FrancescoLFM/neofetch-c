#ifndef DISTRO_NEOFETCH_H
#define DISTRO_NEOFETCH_H

#define DRAWROWS        20
#define DRAWCOLUMNS     50

char *get_distro();
int fetch_distrodraw(char *distroname, struct strarr **distrodraw);

#endif