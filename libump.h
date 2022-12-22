#ifndef NULL 
#define NULL (void *)0
#endif

#define SLOT_TYPE uint64_t
#define CACHE_LINE_SZ 64

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFER_READ_MODE 0
#define BUFFER_WRITE_MODE 1

#define BUFFER_WAITING 0 

struct shared_buffer_t {
  uint64_t __cache_sz; 
  void *__buffer;
};

struct shared_buffer_t *create_shared_buffer(char *name, ssize_t buffer_sz, int mode);

