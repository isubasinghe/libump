#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

#include <ringbuffer.h>

#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

static inline ssize_t get_next_index(struct shared_buffer_t *buf) {
  return 0;
}

static unsigned int gcd(unsigned int u, unsigned int v) {
  int shift;
  if (u == 0)
    return v;
  if (v == 0)
    return u;
  shift = __builtin_ctz(u | v);
  u >>= __builtin_ctz(u);
  do {
    unsigned m;
    v >>= __builtin_ctz(v);
    v -= u;
    m = (int)v >> 31;
    u += v & m;
    v = (v + m) ^ m;
  } while (v != 0);
  return u << shift;
}

static inline unsigned int lcm(unsigned int u, unsigned int v) {
  if(u > v) {
    return (u/gcd(u,v))*v;
  }else {
    return (v/gcd(u,v))*u;
  }
}

struct shared_buffer_t *create_shared_buffer(char *name, ssize_t buffer_sz, enum shared_buffer_mode mode) {
  long cache_sz = sysconf(_SC_LEVEL1_DCACHE_SIZE);
  if(cache_sz < 0 || cache_sz != CACHE_LINE_SZ) {
    return NULL;
  }
  char *name_buffer = calloc(strlen(name)+MAX(strlen(BUFFER_READ_STRING),strlen(BUFFER_WRITE_STRING)), sizeof(char));
  switch (mode) {
    case BUFFER_WRITE_MODE:
      sprintf(name_buffer, "%s%s", name, BUFFER_WRITE_STRING);
      break;
    case BUFFER_READ_MODE:
      sprintf(name_buffer, "%s%s", name, BUFFER_READ_STRING);
      break;
    default:
      return NULL;
  }

  int fd_shm = shm_open(name_buffer, O_RDWR, S_IRUSR | S_IREAD | S_IWRITE);
  if(fd_shm < 0) {
    return NULL;
  }


  unsigned int page_sz = getpagesize();
  int remer = lcm(page_sz, sizeof(SLOT_TYPE));
  
  // a multple of page size and slot size
  unsigned int rem = buffer_sz%remer; 
  buffer_sz += rem;

  if(ftruncate(fd_shm, buffer_sz) < 0) {
    return NULL;
  }

  void *internal_buffer = mmap(NULL, buffer_sz, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, fd_shm, 0);
  if(internal_buffer == NULL) {
    shm_unlink(name_buffer);
  }
 
  struct shared_buffer_t *buf = calloc(1, sizeof(struct shared_buffer_t));
  buf->__cache_sz = cache_sz;
  buf->__buffer = internal_buffer;
  buf->read_cursor = 0;
  buf->write_cursor = 0;
  buf->name = name_buffer;
  buf->mode = mode;
  memset(buf->read_buf, 0, CACHE_LINE_SZ);
}

inline char *try_read_line(struct shared_buffer_t *buf) {
  assert(buf->mode == READ_MODE);
  return NULL;
}

inline size_t try_write_line(struct shared_buffer_t *buf, char *data) {
  assert(buf->mode == WRITE_MODE);
  return 0;
}

char *try_read_msg(struct shared_buffer_t *buf, size_t *datalen) {
  return NULL;
}

size_t try_write_msg(struct shared_buffer_t *buf, char *data, size_t datalen) {
  return 0;
}



void free_shared_buffer(struct shared_buffer_t *buf) {
  free(buf->name);
  free(buf);
}




