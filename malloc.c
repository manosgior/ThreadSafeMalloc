#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include "malloc.h"

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

typedef struct freeNode {
	size_t size;
	struct freeNode *next;	
} Freelist;

__thread Freelist *freeList = NULL;

Freelist *allocate(size_t size) {
	Freelist *toReturn;
	void *tmp;	
	
	pthread_mutex_lock(&lock);
	toReturn = sbrk(0);
	tmp = sbrk(size + sizeof(struct freeNode));
	pthread_mutex_unlock(&lock);
	
	if (tmp == (void *) -1) {
		perror("sbrk error");
		return NULL;
	}
	toReturn->size = size;
	toReturn->next = NULL;

	return toReturn;
}

void *getChunk(size_t size) {
	Freelist *toReturn = NULL, *toReturnPred = NULL, *pred = NULL, *curr = freeList;

	while (curr != NULL) {
		if (curr->size >= size) {
			if (toReturn == NULL || curr->size < toReturn->size) {
				toReturnPred = pred;
				toReturn = curr;
			}
		}
		pred = curr;		
		curr = curr->next;
	}
	if (toReturn != NULL) {
		if (toReturnPred == NULL)
			freeList = NULL;
		else
			toReturnPred->next = toReturn->next;
	}

	return (void *) toReturn;
}

void *malloc(size_t size) {
	void *toReturn = NULL;

	if (size <= 0)
		return NULL;
	
	toReturn = getChunk(size);

	if (toReturn == NULL) {
		toReturn = (void *) (allocate(size) + 1);
	}

	return toReturn;
}

void free(void *ptr){
	Freelist *chunk;

	if (ptr == NULL)
		return;	

	chunk = (Freelist *) (((uint8_t *) ptr) - sizeof(struct freeNode));
	
	chunk->next = freeList;
	freeList = chunk;
}
