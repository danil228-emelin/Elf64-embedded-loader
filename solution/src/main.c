#include "file_helper.h"
int main(int argc, char **argv) {
  int ds = read_file(argv[1], O_RDONLY);
  printf("%d", ds);
  return 0;
}
