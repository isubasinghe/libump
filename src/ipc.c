#include <ipc.h>


struct ipc_chan_t *create_chan(char *name) {
  return NULL;
}

int ipc_send_msg(struct ipc_chan_t *buf,char *msg, size_t msglen) {
  return 0;
}

inline bool ipc_msg_ready(struct ipc_chan_t *buf) {
  return false;
}

char *ipc_recv_msg(struct ipc_chan_t *buf, size_t *buflen) {
  return NULL;
}

void ipc_free(struct ipc_chan_t *buf) {
}


