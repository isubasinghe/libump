
#ifndef NULL 
#define NULL (void *)0
#endif

#define SLOT_TYPE uint64_t
#define CACHE_LINE_SZ 32768

#define MAX_OPTIMAL_LOOP 1000

#define LOOP_SIZE 512


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#define BUFFER_READ_MODE 0
#define BUFFER_WRITE_MODE 1

#define BUFFER_READ_STRING "/read"
#define BUFFER_WRITE_STRING "/write"

#define BUFFER_WAITING 0 

#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)

enum shared_buffer_mode {
  READ_MODE, 
  WRITE_MODE,
};

enum buffer_state {
  WAITING = 0,
  READY = 1,
  CONTINUE =2,
};

typedef struct {
  unsigned char buffer_state: 32;
  uint32_t length: 32;
} read_buffer_data;

typedef union {
  read_buffer_data read_data;
  uint64_t raw_data;
} buffer_data;

struct shared_buffer_t {
  uint64_t __cache_sz; 
  buffer_data *__buffer;
  char *name;
  ssize_t read_cursor;
  ssize_t write_cursor;
  // buffer to hold read items
  char read_buf[CACHE_LINE_SZ];
  enum shared_buffer_mode mode;
};

struct shared_buffer_t *create_shared_buffer(char *name, ssize_t buffer_sz, enum shared_buffer_mode mode);

char *try_read_line(struct shared_buffer_t *buf);

size_t try_write_line(struct shared_buffer_t *buf, char *data);

char *try_read_msg(struct shared_buffer_t *buf, size_t *datalen);
size_t try_write_msg(struct shared_buffer_t *buf, char *data, size_t datalen);


void free_shared_buffer(struct shared_buffer_t *buf);

