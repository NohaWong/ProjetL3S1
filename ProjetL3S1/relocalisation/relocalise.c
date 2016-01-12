#include "relocalise.h"

void new_section_header(Elf32_Shdr* section_headers, char* sections_name, rel_info* infos, int nb_relocalisations, Elf32_Ehdr header)
{
    int i;
//    {'', '.', 't', 'e', 'x', 't', '\0', '.', 'd', 'a'...}
    int j;
    for (j=0; j<nb_relocalisations; j++) {
        for (i=0; i<header.e_shnum;i++) {
            if (!strcmp(infos[j].section_name, &sections_name[section_headers[i].sh_name])) {
                // Find the section to change
                section_headers[i].sh_addr = infos[j].section_new_addr;
                section_headers[i].sh_offset += infos[j].section_new_addr;
            }
        }
    }
}



uint8_t** new_section_content (Ensemble_table_rel table_rel, char* sections_name, uint8_t** section_content, rel_info* infos, Elf32_Shdr * section_headers,Elf32_Ehdr header, int rel_count, Elf32_Sym *symbols)
{
    int i, j, k;
    // create copy of each sections
    uint8_t **section_cpy = malloc(sizeof(uint8_t*) * header.e_shnum);
    for (i = 0; i < header.e_shnum; ++i) {
        section_cpy[i] = malloc(section_headers[i].sh_size+1);
        memcpy(section_cpy, section_content, section_headers[i].sh_size);
    }

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

    return section_cpy;
}


