#include <stdio.h>

int main() {
    int i,no;
    FILE *items_file = fopen("data/opts","r");
    char buff[10][10];
        for(i=0;i<6;i++) {
            fscanf(items_file,"%s",buff[i]);
            printf("%s\n",buff[i]);
        }
}
