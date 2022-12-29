#ifndef IPC_H
#define IPC_H 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdc-predef.h>
#include <ringbuffer.h>

struct ipc_chan_t {
  char *name;
  struct shared_buffer_t *write_buf;
  struct shared_buffer_t *read_buf;
};

struct ipc_chan_t create_chan(char *name);

int ipc_send_msg(struct ipc_chan_t *buf,char *msg, size_t msglen);

inline bool ipc_msg_ready(struct ipc_chan_t *buf);
char *ipc_recv_msg(struct ipc_chan_t *buf, size_t *buflen);

void ipc_free(struct ipc_chan_t *buf);


#endif // IPC_H
