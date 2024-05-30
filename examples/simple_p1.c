#include <ringbuffer.h>
int main(int argc, char *argv[]) {
  struct shared_buffer_t *buf = create_shared_buffer("simple", 1024, READ_MODE);

  if(buf == NULL) {
    printf("GOT NULL\n");
    return -1;
  }
  printf("FREEING\n");
  free_shared_buffer(buf);

  return 0;
}
