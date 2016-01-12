#ifndef RELOCALISE_H_INCLUDED
#define RELOCALISE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "../commun/readelf.h"


/**
 * @struct rel_info
 * holds the information for a relocalisation given to the program via its arguments
 * section_name     char* : the name of the section (ex : .text, .data)
 * section_new_addr Elf32_Addr : the new address at which the section needs to be
 */
typedef struct {
    char* section_name;
    Elf32_Addr section_new_addr;
} rel_info;

/**
 * Change the sections' headers & file header to that of an EXEC file
 *
 * @param section_headers       Elf32_Shdr* : the sections' headers of the .o file
 * @param section_name          char* : list of all the sections' names (concatenated)
 * @param infos                 rel_info : list of the relocalisation needed to be done
 * @param rel_count             int : number of elements in previous argument
 * @param header                Elf32_Ehdr : the .o file header
 * @param new_header            Elf32_Ehdr* : the exec. file header
 *
 * @return the exec. file sections' headers
 */
Elf32_Shdr* new_section_header(Elf32_Shdr* section_headers, char* sections_name, rel_info* infos, int rel_count, Elf32_Ehdr header, Elf32_Ehdr* new_header);


uint8_t **new_section_content (Table_rel_set table_rel, char* sections_name, uint8_t** section_content, rel_info* infos, Elf32_Shdr * section_headers, Elf32_Ehdr header, int rel_count, Elf32_Sym *symbols);

Elf32_Sym *new_symbol_table(Elf32_Sym *symb_table, rel_info *info, uint32_t symb_count, uint32_t rel_count, Elf32_Shdr *sections_header, char* section_name );

char* new_section_header_name(char* section_header_name, Elf32_Shdr *new_sections_header, Elf32_Ehdr new_header);

size_t my_strlen(const char *str);

#endif // RELOCALISE_H_INCLUDED
