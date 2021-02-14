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
	int numPages =  ((size + sizeof(struct freeNode) - 1) / sysconf(_SC_PAGESIZE)) + 1;
	size_t correctSize = numPages * sysconf(_SC_PAGESIZE);
	
	pthread_mutex_lock(&lock);
	toReturn = sbrk(0);
	tmp = sbrk(correctSize);
	pthread_mutex_unlock(&lock);
	
	if (tmp == (void *) -1) {
		perror("sbrk error");
		return NULL;
	}
	toReturn->size = correctSize;
	toReturn->next = NULL;

	return toReturn;
}

void insert(Freelist *curr, size_t size) {	
	curr->size = size;
	curr->next = freeList;
	freeList = curr;			
}

void split(Freelist *list, size_t size) {
	int numSegments = list->size / (size + sizeof(struct freeNode));
	uint8_t *ptr = (uint8_t *) list;
	
	for (int i = 0; i < numSegments; i++) {
		insert((Freelist *) (ptr + i * (sizeof(struct freeNode) + size)), size);
	}
}

void *getChunk(size_t size) {
	Freelist *toReturn = NULL, *pred = NULL, *curr = freeList;

	while (curr != NULL) {
		if (curr->size >= size) {
			if (toReturn == NULL || curr->size < toReturn->size)
				toReturn = curr;
		}
		pred = curr;
		curr = curr->next;
	}

	if (toReturn != NULL) {		
		if (pred == freeList)
			freeList = NULL;
		else
			pred->next = NULL;	
	}

	return (void *) toReturn + sizeof(struct freeNode);
}

void *malloc(size_t size) {
	void *toReturn = NULL;

	if (size <= 0)
		return NULL;
	
	if (freeList == NULL) {
		split(allocate(size), size);
	}
	if ((toReturn = getChunk(size)) == NULL) {
		split(allocate(size), size);
		toReturn = getChunk(size);
	}

	return toReturn;
}

void free(void *ptr){
	Freelist *chunk = (Freelist *) (((uint8_t *) ptr) - sizeof(struct freeNode));
	assert(0);
}
