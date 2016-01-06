#ifndef READELF_H
#define READELF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

enum { ERROR_MAGIC_NUMBERS = 1, ERROR_MISSING_ARG, ERROR_WRONG_WORD_SIZE, ERROR_WRONG_ENDIAN,
       ERROR_INVALID_VERSION };


// fonctions lecture header fichier
void read_elf_header(FILE *file, Elf32_Ehdr *header);
void init_systable();
void init_systarget();

// fonctions lecture header sections
Elf32_Shdr *read_elf_section_header(FILE *file, Elf32_Ehdr *header, char** c);
Elf32_Sym *read_symbol_table(FILE *file, Elf32_Shdr *section_headers, Elf32_Half shnum, uint16_t *symbols_count);

#endif
