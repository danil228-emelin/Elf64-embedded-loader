

#ifndef ELF64_LOADER_ELF_READER_H
#define ELF64_LOADER_ELF_READER_H
#include <elf.h>
#include "file_helper.h"
Elf64_Ehdr read_file_header(int fd);
void check_elf64_header(Elf64_Ehdr header);
#endif
