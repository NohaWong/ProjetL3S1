#ifndef PRINTELF_H
#define PRINTELF_H

#include "readelf.h"

int print_elf_header(Elf32_Ehdr elf);
char* print_elf_section_header(FILE* file, Elf32_Ehdr header, Elf32_Shdr * table_entetes_section);

#endif
