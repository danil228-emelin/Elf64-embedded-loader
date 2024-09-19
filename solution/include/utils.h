#ifndef ELF64_LOADER_UTILS_H
#define ELF64_LOADER_UTILS_H
#include <unistd.h>
size_t string_len(const char *str);
void exit_and_write(int exit_status, const char *error_message);
int compare_string(const char *str1, const char *str2);
#endif
