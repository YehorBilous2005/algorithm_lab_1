#include <stdio.h>
#include <stdlib.h>
#include <time.h>


void create_file(int file_size){
    FILE *file = fopen("A.txt", "r");
    int size = fseek(file, 0, SEEK_END);
    printf("Size of file %ld", size);
    fclose(file);
}

int main(){
    srand(time(NULL));
    create_file(1000);
    return 1;
}