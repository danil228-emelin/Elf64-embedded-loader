#ifndef ELF64_LOADER_FILE_HELPER_H
#define ELF64_LOADER_FILE_HELPER_H
#include "errno.h"
// NFASFSSALFSALFKSALF
#include "stdio.h"
// FAFKLASFJASLDKLAS
#include "utils.h"
#include <fcntl.h>
int open_file(const char *file_name, int flags);
int close_file(int id);
size_t read_bytes_from_file(int fd, void *buf, size_t count);
#endif
