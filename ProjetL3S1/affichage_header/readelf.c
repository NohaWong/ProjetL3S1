#include "readelf.h"

char sys_table[256][32];
char sys_target[64][32];

void print_elf_header(FILE *file, elf_header *header) {
    fseek(file, 0, SEEK_SET);
    fread(header->magic_number, sizeof(int8_t), 4, file);
    fread(&header->word_size, sizeof(int8_t), 1, file);
    fread(&header->endianess, sizeof(int8_t), 1, file);
    fread(&header->version, sizeof(int16_t), 1, file);
    fread(&header->sys_type, sizeof(int8_t), 1, file);
    // padding
    fseek(file, 6, SEEK_CUR);
    // size of 'identification' field
    fseek(file, 2, SEEK_CUR);
    fread(&header->file_type, sizeof(Elf32_Half), 1, file);
    fread(&header->sys_target, sizeof(Elf32_Half), 1, file);
}

void init_systable() {
    strcpy(sys_table[0], "UNIX System V");
    strcpy(sys_table[1], "HP-UX");
    strcpy(sys_table[2], "NetBSD");
    strcpy(sys_table[3], "Linux");
    strcpy(sys_table[6], "Sun Solaris");
    strcpy(sys_table[7], "IBM AIX");
    strcpy(sys_table[8], "SGI Irix");
    strcpy(sys_table[9], "FreeBSD");
    strcpy(sys_table[10], "Compaq TRU64");
    strcpy(sys_table[11], "Novell Modesto");
    strcpy(sys_table[12], "OpenBSD");
    strcpy(sys_table[64], "ARM EABI");
    strcpy(sys_table[97], "ARM");
    strcpy(sys_table[255], "Standalone");
}

void init_systarget() {
    strcpy(sys_target[0], "Aucune");
    strcpy(sys_target[2], "SPARC");
    strcpy(sys_target[3], "Intel 80386");
    strcpy(sys_target[4], "Motorola 68000");
    strcpy(sys_target[7], "Intel i860");
    strcpy(sys_target[8], "MIPS I");
    strcpy(sys_target[19], "Intel i960");
    strcpy(sys_target[20], "PowerPC");
    strcpy(sys_target[40], "ARM");
    strcpy(sys_target[50], "Intel IA64");
    strcpy(sys_target[62], "x64");
}
