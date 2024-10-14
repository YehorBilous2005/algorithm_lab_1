#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EXTRAFNUM 5

struct NamesStruct{
    char *fileB[EXTRAFNUM];
    char *fileC[EXTRAFNUM];
};

void DivideFileA(struct NamesStruct *name, short fnum);

void CreateExtraFiles(struct NamesStruct *name, short fnum);

void AllocateStructMemory(struct NamesStruct *name, short fnum);

void FreeStructMemory(struct NamesStruct *name, short fnum);

int main(){
    struct NamesStruct name;
    AllocateStructMemory(&name, EXTRAFNUM);
    CreateExtraFiles(&name, EXTRAFNUM);
    DivideFileA(&name, EXTRAFNUM);
    
    FreeStructMemory(&name, EXTRAFNUM);
    return 0;
}

void AllocateStructMemory(struct NamesStruct *name, short fnum){
    // Dynamic memory allocation for digits
    int *digits = (int *) calloc(1, sizeof(int));
    if (!digits){
        printf("Memory allocation failed for digits\n");
        return 0;
    }
    *digits = (int)log10(EXTRAFNUM) + 2;

    // Dynamic memory allocation for name.files
    for (int i = 0; i < EXTRAFNUM; i++){
        name->fileB[i] = (char *) calloc(*digits, sizeof(char));
        if (!name->fileB[i]){
            printf("Memory allocation failed for names.fileB[%d]\n", i);
            return 0;
        }
        name->fileC[i] = (char *) calloc(*digits, sizeof(char));
        if (!name->fileC[i]){
            printf("Memory allocation failed for names.fileC[%d]\n", i);
            return 0;
        }
    }

    // Free dynamically allocated memory for digits
    free(digits);
    digits = NULL;
}

void FreeStructMemory(struct NamesStruct *name, short fnum){
    // Free dynamically allocated memory for name.files
    for (int i = 0; i < EXTRAFNUM; i++){
        free(name->fileB[i]);
        free(name->fileC[i]);
        name->fileB[i] = NULL;
        name->fileC[i] = NULL;
    }
}

void CreateExtraFiles(struct NamesStruct *name, short fnum){
    FILE *temp_file;

    for (int i = 0; i < fnum; i++){
        sprintf(name->fileB[i], "B%d.txt", i);
        // Temp_file opening
        temp_file = fopen(name->fileB[i], "w");

        if (temp_file == NULL) {
            printf("Error creating file %s\n", name->fileB[i]);
            // Optional free temp_file memory 
            free(temp_file);

            return;
        }
        printf("File %s was created\n", name->fileB[i]);
        // Temp_file closing
        fclose(temp_file);
        temp_file = NULL;

        sprintf(name->fileC[i], "C%d.txt", i);
        // Temp_file opening
        temp_file = fopen(name->fileC[i], "w");

        if (temp_file == NULL) {
            printf("Error creating file %s\n", name->fileC[i]);
            // Optional free temp_file memory 
            free(temp_file);

            return;
        }
        printf("File %s was created\n", name->fileC[i]);
        // Temp_file closing
        fclose(temp_file);
        temp_file = NULL;
    }
}

void DivideFileA(struct NamesStruct *name, short fnum){
    // File_a opening
    FILE *file_a = fopen("D:\\Education\\Algorithms\\lab1\\text_files\\A.txt", "r");
    if (!file_a){
        printf("Error opening file A.txt\n");
        return;
    }

    // Dynamic memory allocation for x
    int *x = (int *) calloc(1, sizeof(int));

    FILE *temp_file;
    while (file_a){
        for (int i = 0; i < fnum; i++){
            if (fscanf(file_a, "%d", x) != EOF){
                // Temp_file opening
                temp_file = fopen(name->fileB[i], "a");

                if (!temp_file){
                    printf("Error opening file name.fileB[%d]\n", i);
                    return;
                }
                fprintf(temp_file, "%d\n", *x);
                // Temp_file closing
                fclose(temp_file);
            }
            else{
                break;
            }  
        }
    }
    // Free dynamically allocated memory for x
    free(x);
    x = NULL;

    // File_a closing
    fclose(file_a);
}