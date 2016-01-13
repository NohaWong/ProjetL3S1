#ifndef RELOCALISE_H_INCLUDED
#define RELOCALISE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "../commun/readelf.h"


/**
 * @struct rel_info
 *
 * holds the information for a relocalisation given to the program via its arguments
 */
typedef struct {
    char* section_name;
    Elf32_Addr section_new_addr;
} rel_info;

/**
 * Changes the sections' headers & file header to that of an EXEC file
 *
 * @param section_headers       The sections' headers of the .o file
 * @param section_name          List of all the sections' names (concatenated)
 * @param infos                 List of the relocalisation needed to be done
 * @param rel_count             Number of elements in previous argument
 * @param header                The .o file header
 * @param new_header            The exec. file header
 *
 * @return the exec. file sections' headers
 */
Elf32_Shdr* new_section_header(Elf32_Shdr* section_headers, char** new_sec_name, char* sections_name, rel_info* infos, int rel_count, Elf32_Ehdr header, Elf32_Ehdr* new_header);

/**
 * Changes the sections' contents to that of an EXEC file
 *
 * @param table_rel             The struct that holds all the reloc. to do
 * @param sections_name         List of all the sections' names (concatenated)
 * @param section_content       Table of all the sections' contents
 * @param infos                 List of the relocalisation needed to be done
 * @param section_headers       The sections' headers of the .o file
 * @param header                The .o file header
 * @param rel_count             Number of elements in argument infos
 * @param symbols				The list of all the symbols from the .o file
 *
 * @return the EXEC file's sections' content
 */
uint8_t **new_section_content (Table_rel_set table_rel, char* sections_name, uint8_t** section_content, rel_info* infos, Elf32_Shdr * section_headers, Elf32_Ehdr header, int rel_count, Elf32_Sym *symbols);

Elf32_Sym *new_symbol_table(Elf32_Sym *symb_table, rel_info *info, uint32_t symb_count, uint32_t* new_symb_count, uint32_t rel_count, Elf32_Shdr *sections_header, char* section_name, Elf32_Shdr *new_sections_header);

char* new_section_header_name(char* section_header_name, Elf32_Shdr *new_sections_header, Elf32_Ehdr new_header);

size_t my_strlen(const char *str);

void change_idx_and_name_symb_table(Elf32_Sym* symbols, int symb_count, Elf32_Shdr* section_headers);

#endif // RELOCALISE_H_INCLUDED
