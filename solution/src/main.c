
#include "elf_reader.h"
int main(int argc, char **argv) {
  int fd = open_file(argv[1], O_RDONLY);
  Elf64_Ehdr file_header=read_file_header(fd);
  return 0;
}
