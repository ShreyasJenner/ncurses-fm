/*
 * main c program
 * calls menu display
 */

// STANDARD HEADER FILES //
#include <stdio.h>
#include <stdlib.h>
// STANDARD HEADER FILES //

// HEADER FILES //
#include "menu_display.h"
#include "print_title.h"
// HEADER FILES //


int main() {
    FILE *file,*f;
    file = fopen("directories.txt","r");

    int argc,i;
    fscanf(file,"%d",&argc);

    char **argv = (char **)(calloc(argc,sizeof(char*)));
    char *s;


    for(i=0;i<argc;i++) {
        argv[i] = calloc(1,sizeof(char)*10);
        fscanf(file,"%s",argv[i]);
    }

    menu_display(argc, argv);

    fclose(file);
    return 0;
}
