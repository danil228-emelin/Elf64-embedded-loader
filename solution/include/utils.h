#ifndef ELF64_LOADER_UTILS_H
#define ELF64_LOADER_UTILS_H
#include <stdlib.h>
#include <stdnoreturn.h>
#include <unistd.h>
int string_len(const char *error_message) {
  int len = 0;
  while ((*error_message) != 0) {
    error_message += 1;
    len += 1;
  }
  return len;
}
void exit_and_write(int exit_status, const char *error_message) {
  write(STDERR_FILENO, error_message, string_len(error_message));
  _exit(exit_status);
}
#endif
