

#include "readelf.h"
#include <endian.h>

// -------------- lecture header fichier -------------- //

char sys_table[256][32];
// 193 targets, according to elf.h
char sys_target[193][32];

void read_elf_header(FILE *file, Elf32_Ehdr *header) {
    fseek(file, 0, SEEK_SET);
    fread(header, sizeof(Elf32_Ehdr), 1, file);

    if (header->e_ident[EI_DATA] == ELFDATA2MSB) {
        // swap endianness, file is encoded in little endian
        header->e_machine = htobe16(header->e_machine);
        // swap endianness, file is encoded in little endian
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
    }
}

void init_systable() {
    strcpy(sys_table[ELFOSABI_SYSV], "UNIX System V");
    strcpy(sys_table[ELFOSABI_HPUX], "HP-UX");
    strcpy(sys_table[ELFOSABI_NETBSD], "NetBSD");
    strcpy(sys_table[ELFOSABI_LINUX], "Linux");
    strcpy(sys_table[ELFOSABI_SOLARIS], "Sun Solaris");
    strcpy(sys_table[ELFOSABI_AIX], "IBM AIX");
    strcpy(sys_table[ELFOSABI_IRIX], "SGI Irix");
    strcpy(sys_table[ELFOSABI_FREEBSD], "FreeBSD");
    strcpy(sys_table[ELFOSABI_TRU64], "Compaq TRU64");
    strcpy(sys_table[ELFOSABI_MODESTO], "Novell Modesto");
    strcpy(sys_table[ELFOSABI_OPENBSD], "OpenBSD");
    strcpy(sys_table[ELFOSABI_ARM_AEABI], "ARM EABI");
    strcpy(sys_table[ELFOSABI_ARM], "ARM");
    strcpy(sys_table[ELFOSABI_STANDALONE], "Standalone");
}

void init_systarget() {
    strcpy(sys_target[EM_NONE], "Aucune");
    strcpy(sys_target[EM_SPARC], "SPARC");
    strcpy(sys_target[EM_386], "Intel 80386");
    strcpy(sys_target[EM_68K], "Motorola 68000");
    strcpy(sys_target[EM_860], "Intel i860");
    strcpy(sys_target[EM_MIPS], "MIPS I");
    strcpy(sys_target[EM_960], "Intel i960");
    strcpy(sys_target[EM_PPC], "PowerPC");
    strcpy(sys_target[EM_ARM], "ARM");
    strcpy(sys_target[EM_IA_64], "Intel IA64");
    strcpy(sys_target[EM_X86_64], "x64");
}

// -------------- lecture section header -------------- //

Elf32_Shdr *read_elf_section_header(FILE *file, Elf32_Ehdr *header) {
    uint32_t i;
    Elf32_Shdr * table_entetes_section;

    table_entetes_section = (Elf32_Shdr*) malloc(sizeof(Elf32_Shdr) * (header->e_shnum));
    fseek(file, header->e_shoff, SEEK_SET);

    fread(table_entetes_section, sizeof(Elf32_Shdr), header->e_shnum, file);

    if (header->e_ident[EI_DATA] == ELFDATA2MSB) {
        for (i = 0; i < header->e_shnum; i++) {
        // changer en little endian
            table_entetes_section[i].sh_name = htobe32(table_entetes_section[i].sh_name);
            table_entetes_section[i].sh_type = htobe32(table_entetes_section[i].sh_type);
            table_entetes_section[i].sh_flags = htobe32(table_entetes_section[i].sh_flags);
            table_entetes_section[i].sh_addr = htobe32(table_entetes_section[i].sh_addr);
            table_entetes_section[i].sh_offset = htobe32(table_entetes_section[i].sh_offset);
            table_entetes_section[i].sh_size = htobe32(table_entetes_section[i].sh_size);
            table_entetes_section[i].sh_link = htobe32(table_entetes_section[i].sh_link);
            table_entetes_section[i].sh_info = htobe32(table_entetes_section[i].sh_info);
            table_entetes_section[i].sh_addralign = htobe32(table_entetes_section[i].sh_addralign);
            table_entetes_section[i].sh_entsize = htobe32(table_entetes_section[i].sh_entsize);
        }
    }
    return table_entetes_section;
}

// -------------- lecture section content -------------- //
//
