#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define PATH_TO_A "D:\\Education\\Algorithms\\lab1\\text_files\\A.txt"
#define PATH_TO_FOLDER "D:\\Education\\Algorithms\\lab1\\text_files\\"

#define EXTRAFNUM 100

struct Names{
    char* b[EXTRAFNUM];
    char* c[EXTRAFNUM];
}name;

int global_a_size = 0;

void initNamesStruct(int fnum);
void freeNamesStruct(int fnum);
void createExtraFiles(int fnum);
FILE** openFiles(int fnum, char mode[], char* file_names[]);
void closeFiles(int fnum, FILE** files);
void divideFileA(int fnum, char* file_names[]);
int mergeSeries(int fnum, FILE** ffiles, FILE* tfile, int series);
void mergeFiles(int fnum, char* ffile_names[], char* tfile_names[], int* series);
void externalSorting(int fnum);

int main(){
    initNamesStruct(EXTRAFNUM);

    clock_t start, end;
    start = clock();
    externalSorting(EXTRAFNUM);  
    end = clock();
    printf("Time used: %f\n", (double)(end - start) / CLOCKS_PER_SEC);

    freeNamesStruct(EXTRAFNUM);
    return 0;
}

void initNamesStruct(int fnum){
    int digits;
    digits = (int)log10(fnum) + 6 + strlen(PATH_TO_FOLDER);
    for (int i = 0; i < fnum; i++){
        name.b[i] = (char*) calloc(digits, sizeof(char));
        name.c[i] = (char*) calloc(digits, sizeof(char));
    }
}

void freeNamesStruct(int fnum){
    for (int i = 0; i < fnum; i++){
        free(name.b[i]);
        free(name.c[i]);
    }
}

void createExtraFiles(int fnum){
    FILE* new_file;
    for (int i = 0; i < fnum; i++){
        sprintf(name.b[i], "%sB%d.txt", PATH_TO_FOLDER, i);
        new_file = fopen(name.b[i], "w");
        fclose(new_file);

        sprintf(name.c[i], "%sC%d.txt", PATH_TO_FOLDER, i);
        new_file = fopen(name.c[i], "w");
        fclose(new_file);
    }
}

void deleteExtraFiles(int fnum, int sequence){
    for (int i = 0; i < fnum; i++){
        if (i > 0){
            remove(name.b[i]);
            remove(name.c[i]);
        }
        else{
            if (sequence == 0){
                remove(name.c[i]);
            }
            else{
                remove(name.b[i]);
            }
        }
    }
}

FILE** openFiles(int fnum, char mode[], char* file_names[]){
    FILE** temp_files = (FILE**) malloc(fnum * sizeof(FILE*));
    for (int i = 0; i < fnum; i++){
        temp_files[i] = fopen(file_names[i], mode);
    }
    return temp_files;
}

void closeFiles(int fnum, FILE** files) {
    for (int i = 0; i < fnum; i++) {
        if (files[i] != NULL) {
            fclose(files[i]);
        }
    }
    free(files);
}

void divideFileA(int fnum, char* file_names[]){
    FILE* file_a = fopen(PATH_TO_A, "r");
    int temp_num;
    FILE** temp_files = openFiles(fnum, "a", file_names);
    while (!feof(file_a)){
        for (int i = 0; i < fnum; i++){
            global_a_size++;
            if (fscanf(file_a, "%d", &temp_num) != EOF){
                fprintf(temp_files[i], "%d\n", temp_num);
            }else{
                break;
            }
        }
    }
    fclose(file_a);
    closeFiles(fnum, temp_files);
}

int findMinNum(int* arr, int size, int *min_index){
    long long min_num = LONG_LONG_MAX;
    for (int i = 0; i < size; i++){
        if (min_num > arr[i] && arr[i] > 0){
            min_num = arr[i];
            *min_index = i;
        }
    }
    if (min_num == LONG_LONG_MAX){
        return 0;
    }
    return 1;
}

int mergeSeries(int fnum, FILE** ffiles, FILE* tfile, int series){
    int* series_check = (int*) calloc(fnum, sizeof(int));
    int* arr = (int*) calloc(fnum, sizeof(int));
    int end_of_files = fnum;
    for (int i = 0; i < end_of_files; i++){
        if (fscanf(ffiles[i], "%d", &arr[i]) != 1){
            end_of_files = i;
            break;
        }
        series_check[i] += 1;
    }

    int min_index = 0;
    int array_size = end_of_files;
    while (array_size > 0){
        if (findMinNum(arr, end_of_files, &min_index) == 1){
            fprintf(tfile, "%d\n", arr[min_index]);
            arr[min_index] = -1;
            array_size--;
            if (series_check[min_index] < series){  
                if (fscanf(ffiles[min_index], "%d", &arr[min_index]) != EOF){
                    series_check[min_index]++;
                    array_size++;
                }
            }
        }
    }

    free(series_check);
    free(arr);    

    return (end_of_files = fnum)? 1 : 0;
}

void mergeFiles(int fnum, char* ffile_names[], char* tfile_names[], int* series){
    FILE** ffiles = openFiles(fnum, "r", ffile_names);
    FILE** tfiles = openFiles(fnum, "w", tfile_names);

    int index = 0;
    while (!feof(ffiles[0])){
        if (mergeSeries(fnum, ffiles, tfiles[index], *series) == 1){
            index++;
            if (index >= fnum){
                index = 0;
            }
        }
        else{
            break;
        }
    }

    closeFiles(fnum, ffiles);
    closeFiles(fnum, tfiles);
    *series *= fnum;
}

void externalSorting(int fnum){
    createExtraFiles(fnum);
    divideFileA(fnum, name.b);
    int series = 1;
    int sequence = 0;
    while (series < global_a_size){
        if (sequence == 0){
            mergeFiles(fnum, name.b, name.c, &series);
            sequence = 1;
        }
        else{
            mergeFiles(fnum, name.c, name.b, &series);
            sequence = 0;
        }
    }   
    deleteExtraFiles(fnum, sequence);
}
    