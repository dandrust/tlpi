#include "chat_socket.h"

#define BACKLOG 10

void fail(char *msg) {
  perror(msg);
  exit(EXIT_FAILURE);
}

ssize_t fail_unless(ssize_t ret_val, char *msg) {
  if (ret_val == -1)
    fail(msg);
  return ret_val;
}

int get_socket() {
  int socket_fd = socket(AF_UNIX, SOCK_DGRAM, 0);
  fail_unless(socket_fd, "socket");
  return socket_fd;
}

void bind_socket(int socket, struct sockaddr_un *address) {
  int bind_result = bind(socket, (struct sockaddr *)address, sizeof(struct sockaddr_un));
  fail_unless(bind_result, "bind");
}

void build_address(struct sockaddr_un *address, const char *path) {
  // Clear address struct
  memset(address, 0, sizeof(struct sockaddr_un));
  // Set unix socket family
  address->sun_family = AF_UNIX;
  // Copy socket name
  strncpy(address->sun_path, path, sizeof(address->sun_path) - 1);
}

void listen_for_connections(int socket) {
  int accept_result = listen(socket, BACKLOG);
  fail_unless(accept_result, "listen");
}

int accept_connection(int socket) {
  socklen_t peer_address_size;
  struct sockaddr_un peer_address;
  int accept_result = accept(socket, (struct sockaddr *)&peer_address, &peer_address_size);
  fail_unless(accept_result, "accept");
  return accept_result;
}

void close_socket(int socket) {
  int close_result = close(socket);
  fail_unless(close_result, "close");
}

void make_connection(int socket, struct sockaddr_un *address) {
  int connect_result = connect(socket, (struct sockaddr* )address, sizeof(struct sockaddr_un));
  fail_unless(connect_result, "connect");

}