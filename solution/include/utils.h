#ifndef ELF64_LOADER_UTILS_H
#define ELF64_LOADER_UTILS_H
#include <stdlib.h>
#include <stdnoreturn.h>
#include <unistd.h>
int string_len(const char *error_message);
void exit_and_write(int exit_status, const char *error_message);
#endif
