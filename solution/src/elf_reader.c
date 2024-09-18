#include "elf_reader.h"

Elf64_Ehdr read_file_header(int fd){
    Elf64_Ehdr file_header;
    read_bytes_from_file(fd,&file_header,sizeof(file_header));
    check_elf64_header(file_header);
    return file_header;
}

void check_elf64_header(Elf64_Ehdr header){
    if (header.e_ident[EI_MAG0]!=ELFMAG0 ||header.e_ident[EI_MAG1]!=ELFMAG1||header.e_ident[EI_MAG2]!=ELFMAG2||header.e_ident[EI_MAG3]!=ELFMAG3){
        exit_and_write(9,"Wrong file header.It's not elf.\n");
    }
    if (header.e_ident[EI_CLASS]!=ELFCLASS64){
        exit_and_write(9,"Wrong elf architecture.Work only with the 64-bit architecture.\n");
    }
    if (header.e_type!=ET_EXEC){
        exit_and_write(9,"Wrong elf file.Work only with executable files.\n");
    }
}

