#include "elf_reader.h"

Elf64_Ehdr read_elf64_header(int fd) {
  Elf64_Ehdr file_header;
  read_bytes_from_file(fd, &file_header, sizeof(file_header));
  check_elf64_header(file_header);
  return file_header;
}

void check_elf64_header(Elf64_Ehdr header) {
  if (header.e_ident[EI_MAG0] != ELFMAG0 ||
      header.e_ident[EI_MAG1] != ELFMAG1 ||
      header.e_ident[EI_MAG2] != ELFMAG2 ||
      header.e_ident[EI_MAG3] != ELFMAG3) {
    exit_and_write(9, "Wrong file header.It's not elf.\n");
  }
  if (header.e_ident[EI_CLASS] != ELFCLASS64) {
    exit_and_write(
        9, "Wrong elf architecture.Work only with the 64-bit architecture.\n");
  }
  if (header.e_type != ET_EXEC) {
    exit_and_write(9, "Wrong elf file.Work only with executable files.\n");
  }
  if (header.e_ident[EI_VERSION] != EV_CURRENT) {
    exit_and_write(9, "Wrong elf file.Problem with version.\n");
  }
}
// Possible Elf64_Half -> size_t

void set_position(int fd, Elf64_Off offset) {
  off_t file_position = 0;
  if ((file_position = lseek(fd, offset, SEEK_SET)) == -1) {
    exit_and_write(errno, "Can't set carriage\n");
  }
}

Elf64_Phdr read_elf64_program_header_table_entry(int fd,
                                                 Elf64_Half entry_size) {
  Elf64_Phdr entry;
  read_bytes_from_file(fd, &entry, entry_size);
  return entry;
}

void read_program_header_table(int fd, Elf64_Ehdr *file_header,
                               Elf64_Phdr *table_headers) {
  set_position(fd, file_header->e_phoff);
  for (int i = 0; i < file_header->e_phnum; i++) {
    table_headers[i] =
        read_elf64_program_header_table_entry(fd, file_header->e_phentsize);
  }
}

Elf64_Shdr read_elf64_segment_table_entry(int fd, Elf64_Half entry_size) {
  Elf64_Shdr segment;
  read_bytes_from_file(fd, &segment, entry_size);
  return segment;
}
void read_elf64_section_header_table(int fd, Elf64_Ehdr *file_header,
                                     Elf64_Shdr *table_segments) {
  set_position(fd, file_header->e_shoff);
  for (int i = 0; i < file_header->e_shnum; i++) {
    table_segments[i] =
        read_elf64_segment_table_entry(fd, file_header->e_shentsize);
  }
}
