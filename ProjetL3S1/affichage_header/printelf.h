#ifndef PRINTELF_H
#define PRINTELF_H

#include "readelf.h"

int print_elf_header(Elf32_Ehdr elf);
void print_elf_symbol_table(Elf32_Sym *symbols, Elf32_Half shnum);

void print_elf_section_header(Elf32_Ehdr header, Elf32_Shdr * table_entetes_section, char *c);
void print_elf_rel_tab(TableRel * tab);

void print_elf_section_header(Elf32_Ehdr header, Elf32_Shdr * table_entetes_section, char *secname);
void print_elf_section_content(uint8_t** secContent, int number, Elf32_Shdr *section_headers);

#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */
#define RESET       "\033[0m"


#endif
