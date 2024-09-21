#include "elf_reader.h"
Elf64_Ehdr read_elf64_header(int fd) {
  Elf64_Ehdr file_header;
  read_bytes_from_file(fd, &file_header, sizeof(file_header));
  check_elf64_header(file_header);
  return file_header;
}
//e_type I don't know.readelf show that I have DYN.
void check_elf64_header(Elf64_Ehdr header) {
  if (header.e_ident[EI_MAG0] != ELFMAG0 ||
      header.e_ident[EI_MAG1] != ELFMAG1 ||
      header.e_ident[EI_MAG2] != ELFMAG2 ||
      header.e_ident[EI_MAG3] != ELFMAG3) {
    exit_and_write(9, "Wrong file header.It's not elf.\n");
  }
  if (header.e_ident[EI_CLASS] != ET_EXEC) {
    exit_and_write(
        9, "Wrong elf architecture.Work only with the 64-bit architecture.\n");
  }
  // ET_DYN or ET_EXEC
  if (header.e_type != ET_DYN) {
    exit_and_write(9, "Wrong elf file type.Work only with executable files.\n");
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

// maybe another way of checking
//  another error code
// will we have name size in bytes???.
void *find_executable_section(const char *section_executable_name,
                              const Elf64_Ehdr *file_header,
                              const Elf64_Shdr *segment_table, int fd) {
  if (file_header->e_shstrndx == SHN_UNDEF) {
    exit_and_write(-1, "Can't find executable section because section name "
                       "string table doesn't exist.\n");
  }
  Elf64_Shdr section_name_string_table = segment_table[file_header->e_shstrndx];
  if (section_name_string_table.sh_type != SHT_STRTAB) {
    exit_and_write(-1, "Can't read sections' names,current section isn't name "
                       "string table.\n");
  }

  for (int i = 0; i < file_header->e_shnum; i++) {
    set_position(fd, section_name_string_table.sh_offset +
                         segment_table[i].sh_name);
    char section_name[32];
    read_bytes_from_file(fd, section_name, 32);
    if (compare_string(section_executable_name, section_name)) {
      return (void *)segment_table[i].sh_addr;
    }
  }
  exit_and_write(-1, "Can't find executable section\n");
}

uintptr_t align_virtual_address(Elf64_Word virtual_address) {
  return (uintptr_t)virtual_address & (uintptr_t) ~(sysconf(_SC_PAGE_SIZE) - 1);
}
void *allocate_memory(void *virtual_address, size_t size) {
  void *result = mmap(virtual_address, size, PROT_READ | PROT_WRITE,
                      MAP_FIXED | MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
  if (result == MAP_FAILED) {
    exit_and_write(errno, "Can't allocate memory for segment\n");
  }
  return result;
}

void set_permission_for_pages(const Elf64_Phdr *header_table,
                              size_t amount_of_entries) {
  for (size_t i = 0; i < amount_of_entries; i++) {
    if (header_table[i].p_type == PT_LOAD) {
      int permissions = 0;
      void *aligned_virtual_address =
          (void *)align_virtual_address(header_table[i].p_vaddr);
      if (header_table[i].p_flags & PF_X) {
        permissions |= PROT_EXEC;
      }
      if (header_table[i].p_flags & PF_W) {
        permissions |= PROT_WRITE;
      }
      if (header_table[i].p_flags & PF_R) {
        permissions |= PROT_READ;
      }
        uintptr_t padding = (uintptr_t)header_table[i].p_vaddr -
                            (uintptr_t)aligned_virtual_address;
      int result = mprotect(aligned_virtual_address, header_table[i].p_memsz+padding,
                            permissions);
      if (result == -1) {
        exit_and_write(errno, "Cant' set permissions for allocated pages\n");
      }
    }
  }
}

void mmap_file_to_virtual_address_space(int fd, const Elf64_Phdr *header_table,
                                        size_t amount_of_entries) {
  for (size_t i = 0; i < amount_of_entries; i++) {
    if (header_table[i].p_type == PT_LOAD) {
      set_position(fd, header_table[i].p_offset);
      void *aligned_virtual_address =
          (void *)align_virtual_address(header_table[i].p_vaddr);
      read_bytes_from_file(fd, aligned_virtual_address,
                           header_table[i].p_filesz);
    }
  }
}

// Maybe wrong implementation.
// Custom segments can overlap due to page aligning.
void create_segments(const Elf64_Phdr *header_table, size_t amount_of_entries,
                     int fd) {
  for (size_t i = 0; i < amount_of_entries; i++) {
    if (header_table[i].p_type == PT_LOAD) {
      void *aligned_virtual_address =
          (void *)align_virtual_address(header_table[i].p_vaddr);
      uintptr_t padding = (uintptr_t)header_table[i].p_vaddr -
                          (uintptr_t)aligned_virtual_address;
          void *address_segment = allocate_memory(
          aligned_virtual_address, header_table[i].p_memsz + padding);
      if ((address_segment) != (aligned_virtual_address)) {
        exit_and_write(-1, "Start segment virtual address isn't the same as "
                           "aligned virtual address");
      }
    }
  }
  mmap_file_to_virtual_address_space(fd, header_table, amount_of_entries);
  set_permission_for_pages(header_table, amount_of_entries);
}

void check_executable_segment(void *entry_point,
                              const Elf64_Shdr *segment_table,
                              Elf64_Phdr *table_headers, size_t table_h,
                              size_t table_s) {
  uintptr_t start_address = (uintptr_t)entry_point;

  for (size_t i = 0; i < table_h; i++) {
    uintptr_t start = table_headers[i].p_vaddr;
    uintptr_t end = start + table_headers[i].p_memsz;

    if (start_address <= start & start_address < end) {
      if (!(table_headers[i].p_flags & PF_X) ||
          !(table_headers[i].p_flags & PF_R)) {
        exit_and_write(13, "Segment doesn't have permissions for executing or "
                           "reading,please try another segment\n");
      }
    }
  }
  for (size_t i = 0; i < table_s; i++) {
    if (segment_table[i].sh_addr == start_address) {
      if (!(segment_table[i].sh_flags & (SHF_EXECINSTR))) {
        exit_and_write(13, "Segment doesn't contain executable "
                           "instructions,please try another segment\n");
      }
    }
  }
}
