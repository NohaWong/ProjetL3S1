#ifndef PRINTELF_H
#define PRINTELF_H

#include "readelf.h"

int print_elf_header(elf_header elf);
void print_elf_section_header(elf_header header, Elf32_Shdr * table_entetes_section);

#endif
