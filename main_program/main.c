#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define EXTRAFNUM 4

struct NamesStruct{
    char *fileB[EXTRAFNUM];
    char *fileC[EXTRAFNUM];
};

void divideFileA(struct NamesStruct *name, short fnum);

void createExtraBFiles(struct NamesStruct *name, short fnum);

void allocateStructMemory(struct NamesStruct *name, short fnum);

void freeStructMemory(struct NamesStruct *name, short fnum);

void mergeFilesB(struct NamesStruct *name, short fnum);

void mergeFilesC(struct NamesStruct *name, short fnum);

int main(){
    struct NamesStruct name;
    allocateStructMemory(&name, EXTRAFNUM);
    createExtraBFiles(&name, EXTRAFNUM);
    divideFileA(&name, EXTRAFNUM);
    mergeFilesB(&name, EXTRAFNUM);
    mergeFilesC(&name, EXTRAFNUM);

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
    *digits = (int)log10(fnum) + 2;

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
    // free(name->fileB[3]);
    // free(name->fileC[1]);
    for (int i = 0; i < EXTRAFNUM; i++){
        remove(name->fileB[i]);
        remove(name->fileC[i]);

        free(name->fileB[i]); // free memory
        free(name->fileC[i]);

        name->fileB[i] = NULL;
        name->fileC[i] = NULL;
    }
}

void createExtraBFiles(struct NamesStruct *name, short fnum){
    FILE *temp_file;

    for (int i = 0; i < fnum; i++){
        sprintf(name->fileB[i], "B%d.txt", i);
        temp_file = fopen(name->fileB[i], "w"); //open file

        if (temp_file == NULL){
            printf("Error: file %s creating failed\n", name->fileB[i]);
            return;
        }
        printf("File %s was created\n", name->fileB[i]);

        fclose(temp_file); // close file
        temp_file = NULL;

        sprintf(name->fileC[i], "C%d.txt", i);
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

void swap(int *a, int *b){
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int arr[], int low, int high){
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quicksort(int arr[], int low, int high){
    if (low < high){
        int pi = partition(arr, low, high);
        quicksort(arr, low, pi - 1);
        quicksort(arr, pi + 1, high);
    }
}

void mergeFilesB(struct NamesStruct *name, short fnum){
    FILE *fileB[fnum];
    FILE *fileC = NULL;

    int *arr = (int *) calloc(fnum, sizeof(int)); // memory allocation
    if (arr == NULL){
        printf("Error: memory allocation failed for arr\n");
        return;
    }

    for (int i = 0; i < fnum; i++){
        fileB[i] = fopen(name->fileB[i], "r"); // open files b
    }

    int *index = (int *) calloc(1, sizeof(int)); // memory allocation
    *index = 0; 

    int *endOfFiles = (int *) calloc(1, sizeof(int)); // memory allocation
    *endOfFiles = fnum;
    while (!feof(fileB[0])){
        for (int i = 0; i < fnum; i++){
            if (fileB[i] == NULL) {
                printf("Failed to open file: %s\n", name->fileB[i]);
                free(index);
                free(arr);
                for (int j = 0; j < i; j++){
                    fclose(fileB[j]);
                }
                return;
            }
            if (fscanf(fileB[i], "%d", &arr[i]) != 1){
                printf("Error reading from file: %s\n", name->fileB[i]);
                *endOfFiles = i;
                break;
            }
        }

        quicksort(arr, 0, *endOfFiles - 1);
        // for (int i = 0; i < *endOfFiles; i++){
        //     printf("%d ", arr[i]);
        // }
        // printf("\n");

        fileC = fopen(name->fileC[*index], "a+"); // open file
        if (fileC == NULL){
            printf("Failed to open file for reading: %s\n", name->fileC[*index]);
            free(index);
            free(arr);
            for (int i = 0; i < fnum; i++){
                fclose(fileB[i]);
            }
            return;
        }
        // printf("%d\n", *index);

        int *temp_num = (int *) calloc(1, sizeof(int));  // memory allocation
        FILE *temp_file = fopen("tempFile", "w"); // open file
        fseek(fileC, 0, SEEK_SET);
        fscanf(fileC, "%d", temp_num);
        if (feof(fileC)){
            for (int i = 0; i < *endOfFiles; i++){
                fprintf(fileC, "%d\n", arr[i]);
            }
        }
        else{
            for (int i = 0; i < *endOfFiles; i++){
                while (!feof(fileC)){
                    if (*temp_num <= arr[i]){
                        fprintf(temp_file, "%d\n", *temp_num);
                        fscanf(fileC, "%d", temp_num);
                    }
                    else{
                        fprintf(temp_file, "%d\n", arr[i]);
                        break;
                    }
                }
                if (feof(fileC)){
                    fprintf(temp_file, "%d\n", arr[i]);
                }
            }    

            while (!feof(fileC)){
                fprintf(temp_file, "%d\n", *temp_num);
                fscanf(fileC, "%d", temp_num);
            }
            fclose(fileC);
            fclose(temp_file);

            temp_file = fopen("tempFile", "r");
            fileC = fopen(name->fileC[*index], "w");
            while (fscanf(temp_file, "%d", temp_num) == 1){
                fprintf(fileC, "%d\n", *temp_num);
            }
        }
        free(temp_num); // free memory
        temp_num = NULL;
        fclose(temp_file); // close file
        remove("tempFile");
        fclose(fileC); // close file

        (*index)++;
        if (*index >= fnum){
            *index = 0;
        }

        if (*endOfFiles != fnum){
            break;
        }
    }

    for (int i = 0; i < fnum; i++){
        fclose(fileB[i]);
    }

    free(endOfFiles);
    free(arr); // free memory
    arr = NULL;
    free(index);
    index = NULL;
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
}

void mergeFilesC(struct NamesStruct *name, short fnum){
    FILE *fileC[fnum];
    FILE *fileB = fopen("D:\\Education\\Algorithms\\lab1\\text_files\\NewBFile.txt", "w");

    for (int i = 0; i < fnum; i++){
        fileC[i] = fopen(name->fileC[i], "r"); // open files c
    }
    int *arr = (int *) calloc(fnum, sizeof(int)); // memory allocation

    int *endOfFiles = (int *) calloc(1, sizeof(int)); // memory allocation
    *endOfFiles = fnum;
    int *arr_size = (int *) calloc(1, sizeof(int)); // memory allocation
    *arr_size = fnum;

    for (int i = 0; i < fnum; i++){
        if (fscanf(fileC[i], "%d", &arr[i]) != 1){
            arr[i] = -1;
            printf("Error reading from file: %s\n", name->fileC[i]);
            *endOfFiles = i;
            *arr_size = i;
            break;
        }
    }

    // for (int i = 0; i < *endOfFiles; i++){
    //     printf("%d ", arr[i]);
    // }
    // printf("\n");

    int *min_index = (int *) calloc(1, sizeof(int));
    while (*arr_size > 0){
        findMinNum(arr, *endOfFiles, min_index);
        fprintf(fileB, "%d\n", arr[*min_index]);
        if (fscanf(fileC[*min_index], "%d", &arr[*min_index]) != 1){
            arr[*min_index] = -1;
            printf("Error reading from file: %s\n", name->fileC[*min_index]);
            (*arr_size)--;
        }
    }

    free(min_index);
    free(arr_size);
    free(endOfFiles);
    free(arr);
    for (int i = 0; i < fnum; i++){
        fclose(fileC[i]); // close files c
    }
    fclose(fileB);
}