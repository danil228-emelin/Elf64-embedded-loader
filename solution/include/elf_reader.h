

#ifndef ELF64_LOADER_ELF_READER_H
#define ELF64_LOADER_ELF_READER_H
#include "file_helper.h"
#include <elf.h>
#include <sys/mman.h>
Elf64_Ehdr read_elf64_header(int fd);
uintptr_t align_virtual_address(Elf64_Word virtual_address);
void *allocate_memory(void *virtual_address, size_t size);

void set_permission_for_pages(const Elf64_Phdr *header_table,
                              size_t amount_of_entries);
void mmap_file_to_virtual_address_space(int fd, const Elf64_Phdr *header_table,
                                        size_t amount_of_entries);
void check_elf64_header(Elf64_Ehdr header);
void set_position(int fd, Elf64_Off offset);
Elf64_Phdr read_elf64_program_header_table_entry(int fd, Elf64_Half entry_size);
void read_program_header_table(int fd, Elf64_Ehdr *file_header,
                               Elf64_Phdr *table_headers);
Elf64_Shdr read_elf64_segment_table_entry(int fd, Elf64_Half entry_size);
void read_elf64_section_header_table(int fd, Elf64_Ehdr *file_header,
                                     Elf64_Shdr *table_segments);
void *find_executable_section(const char *section_executable_name,
                              const Elf64_Ehdr *file_header,
                              const Elf64_Shdr *segment_table, int fd);
void create_segments(const Elf64_Phdr *header_table, size_t amount_of_entries,int fd);
void check_executable_segment(void *entry_point,
                              const Elf64_Shdr *segment_table,
                              Elf64_Phdr *table_headers, size_t table_h,
                              size_t table_s);
#endif
