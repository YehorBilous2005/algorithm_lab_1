#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <windows.h>

#define Gb 1 << 30

void create_file(long long);

int main(){
    srand(time(NULL));
    create_file(10LL*Gb);
    return 1;
}

void create_file(long long required_size){
    long long file_size = 0LL;
    int digits;
    int rand_num;
    FILE *file = fopen("..\\text_files\\A.txt", "w");
    while (file_size < required_size){
        rand_num = rand() + 1;
        fprintf(file, "%d\n", rand_num);
        digits = (int)log10(rand_num) + 1;
        file_size += (long long)(sizeof(char) * (digits + 2));
    }
    fclose(file);
    printf("Size of file: %lld bytes", file_size);
}

