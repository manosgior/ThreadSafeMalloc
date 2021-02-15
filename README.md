# ThreadSafeMalloc

The allocation policy is trivial:

Each thread has its own (threadlocal) free list.
Since the sbrk syscall is not thread-safe, it is protected with a simple lock.

Malloc's algorithm:
	1. If the given size <= 0 return NULL
	2. Get a chunk from the free list (best-fist policy)
	3. If no chunk was returned, we need to allocate space (lock, call sbrk(size), unlock and return the pointer)

Free's algorithm:
	1. If the given pointer is NULL, return (do nothing)
	2. Get the pointer's chunk info ((uint8_t *) ptr - sizeof(meta_info_struct))
	3. Add the chunk to the free list

Obviously there is huge room for improvement:
-Multiple free lists or even better a hash table of free lists (hashed with allocation size)
-Check other allocation policies (first fit, next fit, buddy blocks, slab allocation)


