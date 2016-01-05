#include "readelf.h"
#include <endian.h>

char sys_table[256][32];
// 193 targets, according to elf.h
char sys_target[193][32];

void read_elf_header(FILE *file, elf_header *header) {
    fseek(file, 0, SEEK_SET);
    fread(header->magic_number, sizeof(uint8_t), 4, file);
    fread(&header->word_size, sizeof(uint8_t), 1, file);
    fread(&header->endianness, sizeof(uint8_t), 1, file);
    fread(&header->version, sizeof(uint8_t), 1, file);
    fread(&header->sys_type, sizeof(uint8_t), 1, file);
    // padding
    fseek(file, 6, SEEK_CUR);
    // size of 'identification' field
    fseek(file, 2, SEEK_CUR);
    fread(&header->file_type, sizeof(Elf32_Half), 1, file);
    fread(&header->sys_target, sizeof(Elf32_Half), 1, file);
    fread(&header->sys_version, sizeof(Elf32_Word), 1, file);
    fread(&header->entry_point, sizeof(Elf32_Addr), 1, file);
    fread(&header->header_table_offset, sizeof(Elf32_Off), 1, file);
    fread(&header->section_table_offset, sizeof(Elf32_Off), 1, file);
    fread(&header->flags, sizeof(Elf32_Word), 1, file);
    fread(&header->header_size, sizeof(Elf32_Half), 1, file);
    fread(&header->header_entry_size, sizeof(Elf32_Half), 1, file);
    fread(&header->header_entry_count, sizeof(Elf32_Half), 1, file);
    fread(&header->section_entry_size, sizeof(Elf32_Half), 1, file);
    fread(&header->section_entry_count, sizeof(Elf32_Half), 1, file);

    if (header->endianness == ELFDATA2MSB) {
        // swap endianness, file is encoded in little endian
        header->file_type = htobe16(header->file_type);
        // swap endianness, file is encoded in little endian
        header->sys_target = htobe16(header->sys_target);
        // swap endianness, file is encoded in little endian
        header->sys_version = htobe32(header->sys_version);
        header->entry_point = htobe32(header->entry_point);
        header->header_table_offset = htobe32(header->header_table_offset);
        header->section_table_offset = htobe32(header->section_table_offset);
        header->flags  = htobe32(header->flags);
        header->header_size = htobe16(header->header_size);
        header->header_entry_size = htobe16(header->header_entry_size);
        header->header_entry_count = htobe16(header->header_entry_count);
        header->section_entry_size = htobe16(header->section_entry_size);
        header->section_entry_count = htobe16(header->section_entry_count);
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
