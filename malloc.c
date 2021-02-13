#include <unistd.h>
#include <pthread.h>
#include "malloc.h"

#define PAGE_SIZE 4096

typedef struct freeNode {
	size_t size;
	struct freeNode *next;	
} Freelist;

__thread Freelist *list64 = NULL;
__thread Freelist *list256 = NULL;
__thread Freelist *list512 = NULL;
__thread Freelist *list1024 = NULL;
__thread Freelist *list4096 = NULL;


Freelist *getList(size_t size) {
	if (size <= 64 - sizeof(struct freeNode))
		return list64;
	else if (size <= 256 - sizeof(struct freeNode))
		return list256;
	else if (size <= 512 - sizeof(struct freeNode))
		return list512;
	else if (size <= 1024 - sizeof(struct freeNode))
		return list1024;
	else
		return list4096;
}

void split(Freelist *list, size_t size) {

}

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void mallocInit() {
	
}

void *malloc(size_t size) {

}

void free(void *ptr){

}
