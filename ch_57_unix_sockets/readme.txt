To compile:
gcc client.c chat_socket.c -o ./bin/client
gcc server.c chat_socket.c -o ./bin/server

To clear out old sockets:
rm sock/* 
