#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define Gb 1073741824

void create_file(int);

int main(){
    srand(time(NULL));
    create_file(0.01*Gb);
    return 1;
}

void create_file(int file_size){
    int temp_size = 0;
    int digits = 0;
    int rand_num = 0;
    FILE *file = fopen("..\\text_files\\A.txt", "w");
    while (temp_size < file_size){
        rand_num = rand() + 1;
        fprintf(file, "%d\n", rand_num);
        digits = (int)(log10(rand_num)) + 1;
        temp_size += sizeof(char) * (digits + 2);
    }
    fclose(file);
    printf("Size of file: %d bytes", temp_size);
}

