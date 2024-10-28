#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EXTRAFNUM 10

struct NamesStruct{
    char *fileB[EXTRAFNUM];
    char *fileC[EXTRAFNUM];
};

void divideFileA(struct NamesStruct *name, short fnum);

void createExtraFiles(struct NamesStruct *name, short fnum);

void allocateStructMemory(struct NamesStruct *name, short fnum);

void freeStructMemory(struct NamesStruct *name, short fnum);

int findMinNum(int *arr, int size, int *min_index);

void mergeFiles(char *fname[EXTRAFNUM], char *sname[EXTRAFNUM], int fnum, int series);

int main(){
    struct NamesStruct name;
    allocateStructMemory(&name, EXTRAFNUM);
    createExtraFiles(&name, EXTRAFNUM);
    divideFileA(&name, EXTRAFNUM);
    mergeFiles(name.fileB, name.fileC, EXTRAFNUM, 1);

    freeStructMemory(&name, EXTRAFNUM);
    printf("\nABOBA_final");
    return 0;
}

void allocateStructMemory(struct NamesStruct *name, short fnum){
    int *digits = (int *) calloc(1, sizeof(int)); //memory allocation
    if (digits == NULL){
        printf("Error: memory allocation failed for digits\n");
        return;
    }
    *digits = (int)log10(fnum) + 3 + 45;

    for (int i = 0; i < fnum; i++){
        name->fileB[i] = (char *) calloc(*digits, sizeof(char)); //memory allocation
        if (name->fileB[i] == NULL){
            printf("Error: memory allocation failed for names.fileB[%d]\n", i);
            free(digits); // free memory
            digits = NULL;
            for (int j = 0; j < i; j++){
                free(name->fileB[j]);
                name->fileB[j] = NULL;
            }
            return;
        }
        name->fileC[i] = (char *) calloc(*digits, sizeof(char));
        if (name->fileC[i] == NULL){
            printf("Error: memory allocation failed for names.fileC[%d]\n", i);
            free(digits); // free memory
            digits = NULL;
            for (int j = 0; j < i; j++){
                free(name->fileB[j]); 
                name->fileB[j] = NULL;
                free(name->fileC[j]);
                name->fileC[j] = NULL;
            }
            return;
        }
    }

    free(digits); // free memory
    digits = NULL;
}

void freeStructMemory(struct NamesStruct *name, short fnum){
    for (int i = 0; i < EXTRAFNUM; i++){
        if (i > 0){
            remove(name->fileC[i]);
            remove(name->fileB[i]);
        }
        free(name->fileB[i]); // free memory
        free(name->fileC[i]);

        name->fileB[i] = NULL;
        name->fileC[i] = NULL;
    }
}

void createExtraFiles(struct NamesStruct *name, short fnum){
    FILE *temp_file;

    for (int i = 0; i < fnum; i++){
        sprintf(name->fileB[i], "D:\\Education\\Algorithms\\lab1\\text_files\\B%d.txt", i);
        temp_file = fopen(name->fileB[i], "w"); //open file

        if (temp_file == NULL){
            printf("Error: file %s creating failed\n", name->fileB[i]);
            return;
        }
        printf("File %s was created\n", name->fileB[i]);

        fclose(temp_file); // close file
        temp_file = NULL;

        sprintf(name->fileC[i], "D:\\Education\\Algorithms\\lab1\\text_files\\C%d.txt", i);
        temp_file = fopen(name->fileC[i], "w"); //open file

        if (temp_file == NULL){
            printf("Error: file %s creating failed\n", name->fileC[i]);
            return;
        }
        printf("File %s was created\n", name->fileC[i]);

        fclose(temp_file); // close file
        temp_file = NULL;
    }
}

void divideFileA(struct NamesStruct *name, short fnum){
    FILE *file_a = fopen("D:\\Education\\Algorithms\\lab1\\text_files\\A.txt", "r"); // open file
    if (file_a == NULL){
        printf("Error: file A.txt opening failed\n");
        return;
    }

    int *x = (int *) calloc(1, sizeof(int)); // memory allocation

    FILE *temp_file;
    while (!feof(file_a)){
        for (int i = 0; i < fnum; i++){
            if (fscanf(file_a, "%d", x) != EOF){
                temp_file = fopen(name->fileB[i], "a"); // open file
                if (temp_file == NULL){
                    printf("Error: file name.fileB[%d] opening failed\n", i);
                    free(x);
                    fclose(file_a);
                    return;
                }
                fprintf(temp_file, "%d\n", *x);
                fclose(temp_file); // close file
            }
            else{
                break;
            }  
        }
    }

    free(x); // memory free
    x = NULL;

    fclose(file_a); // close file
}
  
int findMinNum(int *arr, int size, int *min_index){
    long long *min_num = (long long *) calloc(1, sizeof(long long));
    *min_num = LONG_LONG_MAX;
    for (int i = 0; i < size; i++){
        if (*min_num > arr[i] && arr[i] > 0){
            *min_num = arr[i];
            *min_index = i;
        }
    }
    free(min_num);
    if(*min_num == LONG_LONG_MAX){
        return 0;
    }
    return 1;
}

void mergeFiles(char *fname[EXTRAFNUM], char *tname[EXTRAFNUM], int fnum, int series){ // from file, to file
    FILE *ffile[fnum];
    FILE *tfile = NULL;
    
    printf("ABOBUS 1\n");

    int *arr = (int *) calloc(fnum, sizeof(int)); // memory allocation 1 
    if (arr == NULL){
        printf("Error: memory allocation failed for arr\n");
        return;
    }

    for (int i = 0; i < fnum; i++){
        ffile[i] = fopen(fname[i], "r"); // open files f
        if (ffile[i] == NULL){
            printf("Error failed to open file %s", fname[i]);
            return;
        }
    }

    int *endOfFiles = (int *) calloc(1, sizeof(int)); // memory allocation 3
    if (endOfFiles== NULL){
        printf("Error: memory allocation failed for endOfFiles\n");
        return;
    }
    *endOfFiles = fnum;

    int *index = (int *) calloc(1, sizeof(int)); // memory allocation 4
    if (index== NULL){
        printf("Error: memory allocation failed for index\n");
        return;
    }
    *index = 0; 

    tfile = fopen(tname[*index], "a+"); // open file t
    if (tfile == NULL){
        printf("Error: failed to open file %s\n", tname[*index]);
        return;
    }

    int *counter = (int *) calloc(1, sizeof(int)); // memory allocation 5
    if (counter == NULL){
        printf("Error: memory allocation failed for counter\n");
        return;
    }
    *counter = 0;

    int *min_num_index = (int *) calloc(1, sizeof(int)); // memory allocation 6
    if (min_num_index == NULL){
        printf("Error: memory allocation failed for counter\n");
        return;
    }
    do{
        printf("index = %d\n", *index);
        for (int i = 0; i < *endOfFiles; i++){
            if (fscanf(ffile[i], "%d", &arr[i]) != 1){
                printf("Error reading from file: %s\n", fname[i]);
                *endOfFiles = i;
                break;
            }
        }
        if (series == 1){
            for (int i = 0; i < *endOfFiles; i++){
                if (findMinNum(arr, *endOfFiles, min_num_index) == 1){
                    fprintf(tfile, "%d\n", arr[*min_num_index]);
                    arr[*min_num_index] = -1;
                }
            }
        }
        else{
            int arr_size = *endOfFiles;
            int counters[*endOfFiles];
            for (int i = 0; i < *endOfFiles; i++){
                counters[i] = 1;
            }
            while (arr_size > 0){
                if (findMinNum(arr, *endOfFiles, min_num_index) == 1){
                    fprintf(tfile, "%d\n", arr[*min_num_index]);
                    arr[*min_num_index] = -1;
                    arr_size--;
                }
                if (counters[*min_num_index] < series){
                    if (fscanf(ffile[*min_num_index], "%d", &arr[*min_num_index]) != 1){
                        printf("Error reading from file: %s\n", fname[*min_num_index]);
                    }
                    else{
                        arr_size++;
                    }
                }
            }
        }
        
        if (*endOfFiles != fnum){
            break;
        }

        (*index)++;
        if (*index > fnum){
            *index = 0;
        }

        fclose(tfile); // close file t
        tfile = fopen(tname[*index], "a+"); // open file t
    }while (1);

    fclose(tfile); // close file t
    free(counter); // free memory 5
    counter = NULL;
    free(min_num_index); // free memory 6
    min_num_index = NULL;
    for (int i = 0; i < fnum; i++){
        fclose(ffile[i]); // close f files
        ffile[i] = fopen(fname[i], "w"); // clean f files
        fclose(ffile[i]);
    }
    free(arr); // free memory 1
    arr = NULL;

    printf("abobus 2\n");

    free(endOfFiles); // free memory 3
    endOfFiles = NULL;
    series *= fnum;

    printf("index = %d\n", *index);

    fnum = *index;
    free(index); // free memory 4
    index = NULL;
    tfile = fopen(tname[1], "r");
    if (!feof(tfile)){
        fclose(tfile);
        mergeFiles(tname, fname, fnum, series);
    }
}