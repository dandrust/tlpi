#ifndef CHAT_SOCKET_H
#define CHAT_SOCKET_H

#include <sys/socket.h>
#include <sys/un.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define SRV_SOCK_PATH "./sock/chat_server"
#define CLIENT_SOCK_PATH_FORMAT "./sock/client-%ld"
#define ENTRANCE_ANNOUNCEMENT "$CTRL 1\0"

void fail(char *msg);

ssize_t fail_unless(ssize_t ret_val, char *msg);

int get_socket();

void bind_socket(int socket, struct sockaddr_un *address);

void build_address(struct sockaddr_un *address, const char *path);

void listen_for_connections(int socket);

int accept_connection(int socket);

void close_socket(int socket);

void make_connection(int socket, struct sockaddr_un *address);

#endif