#include "file_helper.h"
#include "utils.h"
int open_file(const char *file_name, int flags) {
  int res;
  if ((res = open(file_name, flags)) <= 0) {
    exit_and_write(2, "Can't open file\n");
  }
  return res;
}
int close_file(int id) {
  int res = 0;
  if ((res = close(id)) == -1) {
    exit_and_write(errno, "Can't close file\n");
  }
  return res;
}

size_t read_bytes_from_file(int fd, void *buf, size_t count) {
  ssize_t res = 0;
  if ((res = read(fd, buf, count)) == -1) {
    exit_and_write(errno, "Can't read bytes from file\n");
  }
  return res;
}
