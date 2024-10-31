#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

#define PATH_TO_A "D:\\Education\\Algorithms\\lab1\\text_files\\A.txt"
#define PATH_TO_FOLDER "D:\\Education\\Algorithms\\lab1\\text_files\\"

#define GB 1 << 30
#define MB 1 << 20
#define KB 1 << 10
#define B 1

#define EXTRAFNUM 100
#define EXTRARAM 10*MB

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
void swap(int* a, int* b);
int partition(int arr[], int low, int high);
void quickSort(int arr[], int low, int high);
void divideFile(char file_from[], int fnum, char* file_names[], int series);
int mergeSeries(int fnum, FILE** ffiles, FILE* tfile, int series);
void mergeFiles(int fnum, char* ffile_names[], char* tfile_names[], int* series);
void externalSorting(int fnum, int ram);

int main(){
    initNamesStruct(EXTRAFNUM);

    clock_t start, end;
    start = clock();
    externalSorting(EXTRAFNUM, EXTRARAM);  
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

void swap(int* a, int* b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int* arr, int low, int high){
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++){
        if (arr[j] < pivot){
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(int* arr, int low, int high){
    if (low < high){
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void divideFile(char file_from[], int fnum, char* file_names[], int series){
    FILE* file_a = fopen(file_from, "r");

    int* arr = (int*) calloc(series, sizeof(int));
    int index = 0; 
    FILE** temp_files = openFiles(fnum, "a", file_names);
    while (!feof(file_a)){
        for (int i = 0; i < series; i++){
            if (fscanf(file_a, "%d", &arr[i]) != EOF){
                global_a_size++;
            }else{
                series = i;
                break;
            }
        }
        quickSort(arr, 0, series-1);
        for (int i = 0; i < series; i++){
            fprintf(temp_files[index], "%d\n", arr[i]);
        }
        index++;
        index = (index >= fnum)? 0 : index;
    }
    free(arr);  
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

void externalSorting(int fnum, int ram){
    createExtraFiles(fnum);
    int series = (int)(ram / 3 + 1);
    printf("series=%d\n", series);
    divideFile(PATH_TO_A, fnum, name.b, series);
    
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
    