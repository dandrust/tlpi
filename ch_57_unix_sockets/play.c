#include <stdio.h>
#include <unistd.h>

#define LINE_MAX 100

int main () {
  char buffer[LINE_MAX];
  ssize_t num_read;


  num_read = read(STDIN_FILENO, buffer, LINE_MAX);

  write(STDOUT_FILENO, buffer, num_read);

  return 0;
}

