#include "elf_reader.h"

int main(int argc, char **argv) {
  if (argc != 3) {
    exit_and_write(22, "./elf64-loader <source-elf64-file> <section-name>\n");
  }
  int fd = open_file(argv[1], O_RDONLY);
  Elf64_Ehdr file_header = read_elf64_header(fd);
  Elf64_Phdr header_table[file_header.e_phnum];
  read_program_header_table(fd, &file_header, header_table);
  Elf64_Shdr segment_table[file_header.e_shnum];
  read_elf64_section_header_table(fd, &file_header, segment_table);
  Elf64_Shdr executable_segment =
      find_executable_section(argv[2], &file_header, segment_table, fd);
  return 0;
}
