#ifndef PRINTELF_H
#define PRINTELF_H

#include "../commun/readelf.h"


enum { ERROR_MAGIC_NUMBERS = 1, ERROR_MISSING_ARG, ERROR_WRONG_WORD_SIZE, ERROR_WRONG_ENDIAN,
       ERROR_INVALID_VERSION, ERROR_NO_FILE_SPECIFIED };

/*
 *	Print the informations about the file format (Magic number, Endian...)
 */
int print_elf_header(Elf32_Ehdr elf);

void print_elf_symbol_table(Elf32_Sym *symbols, uint16_t symbols_count);

void print_elf_section_header(Elf32_Ehdr header, Elf32_Shdr * table_entetes_section, char *c);


void print_elf_rel_tab(Ensemble_table_rel relocations, Elf32_Sym* symb_table, Elf32_Shdr * section_headers, char *secname, Elf32_Ehdr header);


void print_elf_section_header(Elf32_Ehdr header, Elf32_Shdr * table_entetes_section, char *secname);

void print_elf_section_content(uint8_t** secContent, int number, Elf32_Shdr *section_headers, char *secname, Elf32_Ehdr elf_header);

#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */
#define RESET       "\033[0m"


#endif
