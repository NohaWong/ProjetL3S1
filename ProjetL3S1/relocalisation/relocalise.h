#ifndef RELOCALISE_H_INCLUDED
#define RELOCALISE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "../commun/readelf.h"

typedef struct {
    char* section_name;
    Elf32_Addr section_new_addr;
} rel_info;

// prend une chaine de car. et retourne un entier >= 0. retourne -1 si la chaine n'est pas un entier

void new_section_header(Elf32_Shdr* section_headers, char* nom_sections, rel_info * infos, int nb_relocalisations, Elf32_Ehdr header);

void new_section_content (Ensemble_table_rel table_rel, char* nom_sections, uint8_t** section_content, rel_info* infos, Elf32_Shdr * section_headers,Elf32_Ehdr *header,int nb_relocalisation,Elf32_Sym *symbols);

Elf32_Sym *new_symbol_table(Elf32_Sym *symb_table, rel_info *info, uint32_t symb_count, uint32_t rel_count, Elf32_Shdr *sections_header, char* section_name );

#endif // RELOCALISE_H_INCLUDED
