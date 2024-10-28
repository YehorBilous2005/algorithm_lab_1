#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PATH_TO_A "D:\\Education\\Algorithms\\lab1\\text_files\\A.txt"
#define PATH_TO_FOLDER "D:\\Education\\Algorithms\\lab1\\text_files\\"

#define GB 1 << 30
#define MB 1 << 20
#define KB 1 << 10
#define B 1

#define EXTRAFNUM 3

struct Names{
    char *b[EXTRAFNUM];
    char *c[EXTRAFNUM];
};

void initNamesStruct(struct Names *name, int fnum);
void freeNamesStruct(struct Names *name, int fnum);

int main(){
    struct Names name;
    initNamesStruct(&name, EXTRAFNUM);

    createBCFiles(&name, EXTRAFNUM);

    freeNamesStruct(&name, EXTRAFNUM);
    return 0;
}

void initNamesStruct(struct Names *name, int fnum){
    int digits;
    digits = (int)log10(fnum) + 3 + strlen(PATH_TO_FOLDER);

    for (int i = 0; i < fnum; i++){
        name->b[i] = (char *) calloc(digits, sizeof(char));
        if (name->b[i] == NULL){
            printf("Error: memory allocation failed for name.b[%d]\n", i);
            return;
        }
        name->c[i] = (char *) calloc(digits, sizeof(char));
        if (name->c[i] == NULL){
            printf("Error: memory allocation failed for name.c[%d]\n", i);
            return;
        }
    }
}

void freeNamesStruct(struct Names *name, int fnum){
    for (int i = 0; i < fnum; i++){
        free(name->b[i]);
        free(name->c[i]);
    }
}

void createBCFiles(struct Names *name, int fnum){
    
}