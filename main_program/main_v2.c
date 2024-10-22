#include <stdio.h>
#include <stdlib.h>

#define PATH_TO_A "D:\\Education\\Algorithms\\lab1\\text_files\\A.txt"

#define GB 1 << 30
#define MB 1 << 20
#define KB 1 << 10
#define B 1

struct Files{
    char **b;
    char **c;
};

void initFilesStruct(struct Files *files, int size);

int main(){
    int used_ram = 10*B; //
    int required_files = findFilesAmount(used_ram);
    printf("Files Amount: %d", required_files);
    struct Files files;
    initFilesStruct(&files, 10);
    return 0;
}

void initFilesStruct(struct Files *files, int size);

int findFilesAmount(int used_ram){
    FILE *file_a = fopen(PATH_TO_A, "r");
    fseek(file_a, 0, SEEK_END);
    long long required_files = ftell(file_a) / used_ram + 1;
    fclose(file_a);
    return required_files;
}
