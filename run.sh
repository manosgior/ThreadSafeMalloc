#!/bin/sh

myMalloc=`pwd`/libmalloc.so

make
cd ../ClientServerHashTable && make

LD_PRELOAD=$myMalloc ./server 500 &
LD_PRELOAD=$myMalloc ./client &
LD_PRELOAD=$myMalloc ./client &
LD_PRELOAD=$myMalloc ./client &
LD_PRELOAD=$myMalloc ./client &
LD_PRELOAD=$myMalloc ./client &
