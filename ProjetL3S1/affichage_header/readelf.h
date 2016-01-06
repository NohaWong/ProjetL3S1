#ifndef READELF_H
#define READELF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

enum { ERROR_MAGIC_NUMBERS = 1, ERROR_MISSING_ARG, ERROR_WRONG_WORD_SIZE, ERROR_WRONG_ENDIAN,
       ERROR_INVALID_VERSION };

typedef struct TableRel TableRel;

struct TableRel

{
	int num_section;
	int nb_elem;
    Elf32_Rel * tab;

};


// fonctions lecture header fichier
void read_elf_header(FILE *file, Elf32_Ehdr *header);
void init_systable();
void init_systarget();

// fonctions lecture header sections
Elf32_Shdr *read_elf_section_header(FILE *file, Elf32_Ehdr *header, char** c);
Elf32_Sym *read_symbol_table(FILE *file, Elf32_Shdr *section_headers, Elf32_Half shnum, uint16_t *symbols_count);
TableRel * read_rel_table(FILE *file, Elf32_Shdr *section_headers, Elf32_Half shnum);
Elf32_Rela * read_rela_table();



// prend un nom de section en argument et retourne le numeros associé
int section_name_to_number (char* nom, Elf32_Shdr * section_headers, char* table_noms, Elf32_Ehdr *header);

// lecture du contenu des tables
uint8_t** read_section_content(FILE* file, Elf32_Shdr *section_headers, Elf32_Ehdr *header);
/* renvoie un tableau de contenant l'ensemble des contenus de chaque section */
#endif
