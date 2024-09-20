#include <inttypes.h>
#include <stdio.h>
#include <unistd.h>

char *get_pointer(void) { return (char *)NULL; }
void *align_address(void *ptr) {
  void *result = get_pointer() +
                 ((uintptr_t)ptr & (uintptr_t) ~(sysconf(_SC_PAGE_SIZE) - 1));

  return result;
}

char *get_zero_pointer(void) { return (char *)NULL; }

int main(int argc, char *argv[]) {
  uintptr_t ad = 0;
  printf("numeric representation-%lu\n", (uintptr_t)align_address((void *)ad));
}
