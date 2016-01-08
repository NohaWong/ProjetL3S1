#include "relocalise.h"

void new_section_header(Elf32_Shdr* section_headers, char* nom_sections, rel_info* infos, int nb_relocalisations, Elf32_Ehdr header) {
int i;
//    {'', '.', 't', 'e', 'x', 't', '\0', '.', 'd', 'a'...}
    int j;
    for (j=0; j<nb_relocalisations; j++) {
        for (i=0; i<header.e_shnum;i++) {
            if (!strcmp(infos[j].section_name, &nom_sections[section_headers[i].sh_name])) {
                // On a trouve la section a changer
                section_headers[i].sh_addr = infos[j].section_new_addr;
                section_headers[i].sh_offset += infos[j].section_new_addr;
            }
        }
    }
}



void new_section_content (Ensemble_table_rel table_rel, char* nom_sections, uint8_t** section_content, rel_info* infos, int nb_relocalisations, Elf32_Shdr * section_headers,Elf32_Ehdr *header) {

    uint32_t* tempo= NULL;
    int j,i, section_to_change;
    char *b;
    for (j=0; j<nb_relocalisations; j++) {
        for (i=0;i<table_rel->nb_elem;i++) {
            b = &nom_sections[section_headers[ELF32_M_SYM(table_rel->tab[i].r_info)].sh_name];
            if (!strcmp(infos[j].section_name,b)){
                // get the right section to modify
                section_to_change = table_rel ;
                nomsection = &section_content[section_name_to_number(&nom_sections[relInfo_to_symbole(table_rel->tab[i].r_info)],section_headers,nom_sections,header)][table_rel->tab[i].r_offset];
                memcpy(tempo,nomsection,32);
//                printf("test%d", *tempo);

            }
        }
    }
}


