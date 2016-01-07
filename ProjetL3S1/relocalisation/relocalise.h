#ifndef RELOCALISE_H_INCLUDED
#define RELOCALISE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include "readelf.h"

typedef struct {
    char* section_name;
    Elf32_Addr section_new_addr;
} rel_info;

// prend une chaine de car. et retourne un entier >= 0. retourne -1 si la chaine n'est pas un entier

void new_section_header(Elf32_Shdr* section_headers, char* nom_sections, rel_info * infos, int nb_relocalisations, Elf32_Ehdr header);

#endif // RELOCALISE_H_INCLUDED