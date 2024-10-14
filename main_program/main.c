#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EXTRA_FNUM 5

struct Files{
    FILE *filesB[EXTRA_FNUM];
    char **filesB_names[EXTRA_FNUM];
    FILE *filesC[EXTRA_FNUM];
    char **filesC_names[EXTRA_FNUM];
};

void ExtraFilesCreating(struct Files*, short);

void FileADividing(struct Files*, short);

int main(){
    struct Files files_struct;
    ExtraFilesCreating(&files_struct, EXTRA_FNUM);
    for (int i=0; i<EXTRA_FNUM; i++){
        printf("NameB%d: %s", i, files_struct.filesB_names[i]);
        printf("NameC%d: %s", i, files_struct.filesC_names[i]);
    }
    return 1;
}

void ExtraFilesCreating(struct Files *files_struct, short fnum){
    char *file_name = (char*)calloc((int)(log10(fnum) + 2), sizeof(char));
    for (int i=0; i<fnum; i++){
        sprintf(file_name, "B%d.txt", i);
        files_struct->filesB[i] = fopen(file_name, "w+");
        files_struct->filesB_names[i] = (char**)calloc((int)(log10(fnum) + 2), sizeof(char));
        files_struct->filesB_names[i] = file_name;
        printf("File %s was created\n", file_name);
        fclose(files_struct->filesB[i]);
    }
    for (int i=0; i<fnum; i++){
        sprintf(file_name, "C%d.txt", i);
        files_struct->filesC[i] = fopen(file_name, "w+");
        files_struct->filesB_names[i] = (char**)calloc((int)(log10(fnum) + 2), sizeof(char));
        files_struct->filesB_names[i] = file_name;
        printf("\nFile %s was created", file_name);
        fclose(files_struct->filesC[i]);
    }
    free(file_name);
}

void FileADividing(struct Files *files_struct, short fnum){
    int *x = (int*)calloc(1, sizeof(int));
    FILE *file_a = fopen("D:\\Education\\Algorithms\\lab1\\text_files\\A.txt", "r");
    FILE *temp_file;
    while (!feof(file_a)){
        for (int i=0; i<fnum; i++){
            if (!feof(file_a)){
                fscanf(file_a, "%d", x);
                printf("%d\n", x);
                fprintf(files_struct->filesB[i], "%d\n", x);
            }
            else{
                break;
            }
        }
    }
    fclose(file_a);
    free(x);
}