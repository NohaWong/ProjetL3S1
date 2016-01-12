#include "relocalise.h"


Elf32_Shdr* new_section_header(Elf32_Shdr* section_headers, char* nom_sections, rel_info* infos, int rel_count, Elf32_Ehdr header,Elf32_Ehdr *new_header) {
    int i,k;

    *new_header = header;

    Elf32_Shdr * new_sections_header = NULL;
    int sections_count=0;
    int j;
    for(i=0;i<header.e_shnum;i++){
        if (section_headers[i].sh_type!=SHT_REL){
            sections_count++;
        }
    }
    new_sections_header = malloc(sections_count*(sizeof(Elf32_Shdr)));
    k=0;


    for (j=0; j<rel_count; j++) {
        k=0;
        for (i=0; i<header.e_shnum;i++) {
            if (section_headers[i].sh_type!=SHT_REL){
                new_sections_header[k]=section_headers[i];
                if (!strcmp(infos[j].section_name, &nom_sections[new_sections_header[k].sh_name])) {
                    // On a trouve la section a changer
                    new_sections_header[k].sh_addr = infos[j].section_new_addr;
                    new_sections_header[k].sh_offset += infos[j].section_new_addr;
                }
                k++;
            }
        }
    }

    new_header->e_shnum=sections_count;
    new_header->e_type=ET_EXEC;
    return new_sections_header;
}



uint8_t** new_section_content (Table_rel_set table_rel, char* sections_name, uint8_t** section_content, rel_info* infos, Elf32_Shdr * section_headers,Elf32_Ehdr header, int rel_count, Elf32_Sym *symbols)
{
    int i, j, k;
    // create copy of each sections
    uint8_t **section_cpy = malloc(sizeof(uint8_t*) * header.e_shnum);
    for (i = 0; i < header.e_shnum; ++i) {
        section_cpy[i] = malloc(section_headers[i].sh_size+1);
        memcpy(section_cpy, section_content, section_headers[i].sh_size);
    }
    /* display test section content (before modif)
    for (j = 0; j < section_headers[1].sh_size; ++j) {
        printf("%02x", section_cpy[1][j]);

        if ((j+1)%4 == 0 && j != 0) {
            printf(" ");
        }
        if ((j+1)%16 == 0 && j != 0) {
            printf("\n");
        }
    }
    printf("\n");
    //*/


    // Each different rel section
    for (i=0; i<table_rel.section_count_rel; i++) {
        // For each rel of the section
        for(j=0; j<table_rel.rel_section_list[i].elem_count; j++) {
            for (k=0; k<rel_count; k++) {
                // Name's row is equal to argument
                if(!strcmp(&sections_name[rel_info_to_symbol(table_rel.rel_section_list[i].rel_list[j].r_info, symbols, section_headers)], infos[k].section_name)) {
                    uint8_t *to_fuse = &section_cpy[section_name_to_number(&sections_name[table_rel.rel_section_list[i].section_name+4], section_headers, sections_name, &header)][table_rel.rel_section_list[i].rel_list[j].r_offset];
                    uint32_t result_fusion32 = 0;
                    // convert array of 4 bytes into one integer of 32 bits
                    result_fusion32 = (to_fuse[0] << 24) | (to_fuse[1] << 16) | (to_fuse[2] << 8) | (to_fuse[3]);
                    switch (ELF32_R_TYPE(table_rel.rel_section_list[i].rel_list[j].r_info)) {
                        case R_ARM_ABS32:
                            result_fusion32 += infos[k].section_new_addr;
                            break;
                        case R_ARM_JUMP24:
                            result_fusion32 += infos[k].section_new_addr;
                            result_fusion32 -= table_rel.rel_section_list[i].rel_list[j].r_offset;
                        case R_ARM_CALL:
                            result_fusion32 += infos[k].section_new_addr;
                            result_fusion32 -= table_rel.rel_section_list[i].rel_list[j].r_offset;
                            break;

                        default:
                            break;
                    }

                    to_fuse[0] = (result_fusion32 >> 24) & 0xFF;
                    to_fuse[1] = (result_fusion32 >> 16) & 0xFF;
                    to_fuse[2] = (result_fusion32 >> 8) & 0xFF;
                    to_fuse[3] = result_fusion32 & 0xFF;
                }
            }
        }
    }

	/* display test section_content (after modif)
    for (j = 0; j < section_headers[1].sh_size; ++j) {
        printf("%02x", section_cpy[1][j]);

        if ((j+1)%4 == 0 && j != 0) {
            printf(" ");
        }
        if ((j+1)%16 == 0 && j != 0) {
            printf("\n");
        }
    }
    //*/
    return section_cpy;
}

Elf32_Sym *new_symbol_table(Elf32_Sym *symb_table, rel_info *info, uint32_t symb_count, uint32_t rel_count, Elf32_Shdr *sections_header, char* section_name ){
    uint32_t i,j;
    Elf32_Sym *new_symb_table=malloc(sizeof(Elf32_Sym)*symb_count);
    memcpy(new_symb_table,symb_table,sizeof(Elf32_Sym)*symb_count);
    for(i=0 ; i<symb_count ; i++){
        for(j=0 ; j<rel_count ; j++){
            /* display test symbol table ???
            printf("%i\n",symb_table[i].st_shndx);
            //*/
            if(symb_table[i].st_shndx != SHN_ABS && !strcmp(info[j].section_name,&section_name[sections_header[symb_table[i].st_shndx].sh_name])){
                new_symb_table[i].st_value += info[j].section_new_addr;
            }
        }

    }
    return new_symb_table;
}





