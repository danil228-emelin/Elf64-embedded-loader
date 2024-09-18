

#ifndef ELF64_LOADER_ELF_READER_H
#define ELF64_LOADER_ELF_READER_H
#include "file_helper.h"
#include <elf.h>
Elf64_Ehdr read_elf64_header(int fd);
void check_elf64_header(Elf64_Ehdr header);
void set_position(int fd, Elf64_Off offset);
Elf64_Phdr read_elf64_program_header_table_entry(int fd, Elf64_Half entry_size);
void read_program_header_table(int fd, Elf64_Ehdr *file_header,
                               Elf64_Phdr *table_headers);
Elf64_Shdr read_elf64_segment_table_entry(int fd, Elf64_Half entry_size);
void read_elf64_section_header_table(int fd, Elf64_Ehdr *file_header,
                                     Elf64_Shdr *table_segments);
Elf64_Shdr find_executable_section(const char *section_executable_name,
                                   const Elf64_Ehdr *file_header,
                                   const Elf64_Shdr *segment_table, int fd);
#endif
