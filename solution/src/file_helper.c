#include "file_helper.h"
#include "utils.h"
int read_file(const char *file_name, int flags) {
  int res;
  if ((res = open(file_name, flags)) <= 0) {
    exit_and_write(2, "Can't open file\n");
  }
  return res;
}
