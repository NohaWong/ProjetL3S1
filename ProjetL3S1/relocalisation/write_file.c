#include write_file.h
#include <endian.h>

void write_file_header (FILE *f, Elf32_Ehdr *header) {

    fseek(file, 0, SEEK_SET);

    if (header->e_ident[EI_DATA] == ELFDATA2mSB) {
        header->e_machine = htole16(header->e_machine);
        header->e_version = htole32(header->e_version);
        header->e_entry = htole32(header->e_entry);
        header->e_phoff = htole32(header->e_phoff);
        header->e_shoff = htole32(header->e_shoff);
        header->e_flags  = htole32(header->e_flags);
        header->e_ehsize = htole16(header->e_ehsize);
        header->e_phentsize = htole16(header->e_phentsize);
        header->e_phnum = htole16(header->e_phnum);
        header->e_shentsize = htole16(header->e_shentsize);
        header->e_shnum = htole16(header->e_shnum);
        header->e_shstrndx = htole16(header->e_shstrndx);
    }
    fwrite(header,Elf32_Ehdr,1,f);
}

