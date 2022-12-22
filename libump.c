#include <stdio.h>
#include <stdlib.h>
#include <libump.h>

static inline ssize_t get_next_index(struct shared_buffer_t *buf) {
  return 0;
}

unsigned int gcd(unsigned int u, unsigned int v) {
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

inline unsigned int lcm(unsigned int u, unsigned int v) {
  if(u > v) {
    return (u/gcd(u,v))*v;
  }else {
    return (v/gcd(u,v))*u;
  }
}

struct shared_buffer_t *create_shared_buffer(char *name, ssize_t buffer_sz, int mode) {
  long cache_sz = sysconf(_SC_LEVEL1_DCACHE_SIZE);
  if(cache_sz < 0 || cache_sz != CACHE_LINE_SZ) {
    return NULL;
  }

  int fd_shm = shm_open(name, O_RDWR, S_IRUSR | S_IREAD | S_IWRITE);
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
    shm_unlink(name);
  }
 
  struct shared_buffer_t *buf = calloc(1, sizeof(struct shared_buffer_t));
  buf->__cache_sz = cache_sz;
  buf->__buffer = internal_buffer;
}

