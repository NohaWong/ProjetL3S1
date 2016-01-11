#include <endian.h>
#include "write_file.h"

void write_file_header (FILE *f, Elf32_Ehdr *header) {

    fseek(f, 0, SEEK_SET);


    if (header->e_ident[EI_DATA] == ELFDATA2MSB) {
        header->e_machine = htobe16(header->e_machine);
        header->e_version = htobe32(header->e_version);
        header->e_entry = htobe32(header->e_entry);
        header->e_phoff = htobe32(header->e_phoff);
        header->e_shoff = htobe32(header->e_shoff);
        header->e_flags  = htobe32(header->e_flags);
        header->e_ehsize = htobe16(header->e_ehsize);
        header->e_phentsize = htobe16(header->e_phentsize);
        header->e_phnum = htobe16(header->e_phnum);
        header->e_shentsize = htobe16(header->e_shentsize);
        header->e_shnum = htobe16(header->e_shnum);
        header->e_shstrndx = htobe16(header->e_shstrndx);
    }
    fwrite(header,sizeof(Elf32_Ehdr),1,f);
}

