#include "utils.h"
#include "stdio.h"

size_t string_len(const char *str) {
  size_t len = 0;
  while ((*str) != 0) {
    str += 1;
    len += 1;
  }
  return len;
}
void exit_and_write(int exit_status, const char *error_message) {
  write(STDERR_FILENO, error_message, string_len(error_message));
  _exit(exit_status);
}
int compare_string(const char *str1, const char *str2) {
  if (string_len(str1) != string_len(str2)) {
    return 0;
  }
  size_t len = string_len(str2);
  for (size_t i = 0; i < len; i++) {
    if (str1[i] != str2[i]) {
      return 0;
    }
  }
  return 1;
}
