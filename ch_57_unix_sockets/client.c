#include "chat_socket.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>

#define BUFFER_SIZE 100

int main() {
  int connection_socket;
  ssize_t num_read, num_bytes;
  socklen_t length;
  char read_buffer[BUFFER_SIZE], write_buffer[BUFFER_SIZE];
  struct sockaddr_un server_address, local_address;

  connection_socket = get_socket();

  build_address(&server_address, SRV_SOCK_PATH);
  build_address(&local_address, "");
  snprintf(local_address.sun_path, sizeof(local_address.sun_path), CLIENT_SOCK_PATH_FORMAT, (long) getpid());

  bind_socket(connection_socket, &local_address);

  // Enter the room
  if (sendto(connection_socket, ENTRANCE_ANNOUNCEMENT, sizeof(ENTRANCE_ANNOUNCEMENT), 0, (struct sockaddr *) &server_address, sizeof(struct sockaddr_un)) != sizeof(ENTRANCE_ANNOUNCEMENT) ) {
    printf("sendto");
    exit(1);
  }

  for (;;) {
    printf("%s\n", "top");
    length = sizeof(struct sockaddr_un);

    memset(read_buffer, 0, BUFFER_SIZE);

    // This (and `read` below) block and that's not cool.
    // Using `poll` would probably work better: https://beej.us/guide/bgnet/html/#poll
    num_bytes = recvfrom(connection_socket, read_buffer, BUFFER_SIZE, 0, (struct sockaddr *) &server_address, &length);
    fail_unless(num_bytes, "recvfrom");

    printf("%s", read_buffer);

    num_read = read(STDIN_FILENO, write_buffer, BUFFER_SIZE);
    if (num_read > 0) {
      printf("Debug: Read from stdin: %s\n", write_buffer);
      int write_result = sendto(connection_socket, write_buffer, num_read, 0, (struct sockaddr *) &server_address, length);
      printf("Debug: Wrote %d of %zd bytes\n", write_result, num_read);
    }
  }
}