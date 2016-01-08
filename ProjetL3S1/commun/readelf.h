#ifndef READELF_H
#define READELF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

enum { ERROR_MAGIC_NUMBERS = 1, ERROR_MISSING_ARG, ERROR_WRONG_WORD_SIZE, ERROR_WRONG_ENDIAN,
       ERROR_INVALID_VERSION, ERROR_NO_FILE_SPECIFIED };



// fonctions lecture header fichier
void read_elf_header(FILE *file, Elf32_Ehdr *header);
void init_systable();
void init_systarget();

// read table content
uint8_t** read_section_content(FILE* file, Elf32_Shdr *section_headers, Elf32_Ehdr *header);

// read section headers
Elf32_Shdr *read_elf_section_header(FILE *file, Elf32_Ehdr *header, char** c);

// read symbol table
Elf32_Sym *read_symbol_table(FILE *file, Elf32_Shdr *section_headers, uint16_t *symbols_count);

// read relocation tables
// contains the static relocations of 1 section
typedef struct
{
	int section_name;
	int elem_count;
    Elf32_Rel * rel_list;
} Table_rel_section  ;

// contains the dynamic relocations of 1 section
typedef struct
{
	int section_name;
	int elem_count;
    Elf32_RelA * rel_list;
} Table_relA_section  ;

// contains all the static relocations
typedef struct {
    int section_count_rel;
    int section_count_relA;
    Table_rel_section *rel_section_list;
    Table_relA_section *relA_section_list;
} Ensemble_table_rel;


// the reading functions
Ensemble_table_rel read_rel_table(FILE *file, Elf32_Shdr *section_headers, Elf32_Half shnum);
//TableRela * read_rela_table(FILE *file, Elf32_Shdr *section_headers, Elf32_Half shnum);

// find the section number, being given its name
int section_name_to_number (char* nom, Elf32_Shdr * section_headers, char* table_noms, Elf32_Ehdr *header);

/* renvoie un tableau de contenant l'ensemble des contenus de chaque section */
#endif
