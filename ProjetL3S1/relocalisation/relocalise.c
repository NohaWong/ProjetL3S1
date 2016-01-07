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

/*
void new_section_content (TableRel* table_rel, char* nom_sections, uint8_t** section_content, rel_info* infos, int nb_relocalisations, Elf32_Shdr * section_headers) {

    uint32_t tempo;
    int j,i;
     uint8_t *nomsection;
    for (j=0; j<nb_relocalisations; j++) {
        for (i=0;i<table_rel.nb_elem;i++) {
            if (!strcmp(infos[j].section_name,&nom_sections[relInfo_to_symbole(table_rel.tab[i].r_info)])){
                section = section_content[nom_sections[relInfo_to_symbole(table_rel.tab[i].r_info)]]+table_rel.tab[i].r_offset;

                fread(tempo, sizeof(uint32_t),1, file);
            }
        }
    }
}

*/
