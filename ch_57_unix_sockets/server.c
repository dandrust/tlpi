#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include "chat_socket.h"

#define BUF_SIZE 100

int main() {
  int listening_socket, connection_socket;
  ssize_t num_read;
  char buf[BUF_SIZE];
  socklen_t length;
  ssize_t num_bytes;

  struct sockaddr_un *clients[10];
  int num_clients = 0;

  char *room_name = "My Chat Room";

  struct sockaddr_un local_address, client_address;

  listening_socket = get_socket();

  build_address(&local_address, SRV_SOCK_PATH);

  bind_socket(listening_socket, &local_address);

  for (;;) {
    length = sizeof(struct sockaddr_un);

    memset(buf, 0, BUF_SIZE);

    num_bytes = recvfrom(listening_socket, buf, BUF_SIZE, 0, (struct sockaddr *) &client_address, &length);
    fail_unless(num_bytes, "recvfrom");

    printf("Server received %ld bytes from %s: %s\n", (long)num_bytes, client_address.sun_path, buf);
    
    if (strcmp(ENTRANCE_ANNOUNCEMENT, buf) == 0) {
      struct sockaddr_un *new_client_address = (struct sockaddr_un *) malloc(sizeof(struct sockaddr_un));
      memcpy(new_client_address, &client_address, sizeof(struct sockaddr_un));
      
      clients[num_clients] = new_client_address;
      int num_clients_was = num_clients++;

      printf("  > num_clients was %d, now it's %d\n", num_clients_was, num_clients);
      
      char message[BUF_SIZE];
      memset(message, 0, BUF_SIZE);
      snprintf(message, BUF_SIZE, "%s has entered %s\n", client_address.sun_path, room_name);
      printf("  > Preparig to broadcast: %s\n", message);

      for (int i=0; i<num_clients; i++) {
        printf("  > Sending entrance announcement to [%i] %s\n", i, clients[i]->sun_path);
        if (sendto(listening_socket, message, sizeof(message), 0, (struct sockaddr *)clients[i], sizeof(struct sockaddr_un)) != sizeof(message) ) {
          printf("sendto: Could not broadcast to %s\n", clients[i]->sun_path);
          exit(1);
        }
      }
    }

  }

  // listen_for_connections(listening_socket);

  // for (;;) {
  //   connection_socket = accept_connection(listening_socket);

  //   while ((num_read = fail_unless(
  //               read(connection_socket, buf, BUF_SIZE),
  //               "read")) > 0) {
  //     if (write(STDOUT_FILENO, buf, num_read) != num_read) {
  //       printf("partial/failed write");
  //       exit(EXIT_FAILURE);
  //     }
  //   }

  //   close_socket(connection_socket);
  // }
}