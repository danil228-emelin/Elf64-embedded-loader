#ifndef ELF64_LOADER_FILE_HELPER_H
#define ELF64_LOADER_FILE_HELPER_H
#include "stdio.h"
#include <fcntl.h>
int read_file(const char *file_name, int flags);
#endif
