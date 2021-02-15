#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include "malloc.h"

void *work(void *arg) {
    int size = (int)(uintptr_t) arg;
    int **array;
    
    array = malloc(size * sizeof(int *));
    
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
    
    for (int i = 0; i < size; i++)
        free(array[i]);
    
    free(array);    
}

int main(int argc, char **argv) {
	
	if (argc != 3) {
		fprintf(stderr, "usage: ./mallocTest num_threads array_size\n");
		return -1;
	}

    int size = atoi(argv[2]);
    int nthreads = atoi(argv[1]);
    pthread_t threads[nthreads];
    
    for (int i = 0; i < nthreads; i++)
        pthread_create(&threads[i], NULL, work, (void *)(uintptr_t) size);

    for (int i = 0; i < nthreads; i++) 
		pthread_join(threads[i], NULL);	

    return 0;
}
