#include <stdio.h>

int main(){
    int byte_count = -1;
    while(!feof(stdin)){
        fgetc(stdin);
        byte_count++;
    }
    fprintf(stdout,"%d\n",byte_count);
    return 0;
}