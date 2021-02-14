#include <stdio.h>
#include "malloc.h"

int main(int argc, char **argv) {
    int **array;
    int size = 10;
    printf("ok\n");
    array = malloc(size * sizeof(int *));
     printf("ok\n");
    for (int i = 0; i < size; i++) {
        array[i] = malloc(size * sizeof(int));
    }

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            array[i][j] = i + j;
        }         
    }
    
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", array[i][j]);
        }
        printf("\n");         
    }
    
    free(NULL);

    return 0;
}
