#include "relocalise.h"

int *old_sec_to_new_sec;
int old_sym_to_new_sym[5000];

Elf32_Shdr* new_section_header(Elf32_Shdr* section_headers, char** new_sec_name, char* nom_sections, rel_info* infos, int rel_count, Elf32_Ehdr header,Elf32_Ehdr *new_header) {
    int i, j, k = 0;
    int last_size = 0, last_offset = 0;

    Elf32_Shdr *new_sections_header = NULL;
    int sections_count=0;

    // for malloc below
    for(i=0;i<header.e_shnum;i++) {
        // avoid relocs and empty sections
        if (section_headers[i].sh_type != SHT_REL && section_headers[i].sh_type != SHT_NOBITS) {
            sections_count++;
        }
    }
    new_sections_header = malloc(sections_count*(sizeof(Elf32_Shdr)));

    for (i = 0; i < header.e_shnum; ++i) {
        if (section_headers[i].sh_type != SHT_REL && section_headers[i].sh_type != SHT_NOBITS) {

			old_sec_to_new_sec[i] = k;
            memcpy(&new_sections_header[k], &section_headers[i], sizeof(Elf32_Shdr));
            for (j = 0; j < rel_count; ++j) {
                if (strcmp(&nom_sections[new_sections_header[k].sh_name], infos[j].section_name) == 0) {
                    new_sections_header[k].sh_addr = infos[j].section_new_addr;
                }
            }
/*
            if (i == 1) {
                new_sections_header[k].sh_offset = new_sections_header[k].sh_addr + last_size + last_offset + new_sections_header[k].sh_offset;
                last_offset = section_headers[i].sh_offset;
            } else {
                new_sections_header[k].sh_offset = new_sections_header[k].sh_addr + last_size + last_offset;
                last_size = new_sections_header[k].sh_size;
                last_offset = new_sections_header[k].sh_offset;
            }

            printf("AUFÇ7: %x, TAYE: %x, NON SEXION DASSO: %s\n", last_offset, last_size, &nom_sections[new_sections_header[k].sh_name]);
*/
            ++k;
        }
    }

    *new_header = header;
    new_header->e_shstrndx = old_sec_to_new_sec[new_header->e_shstrndx]; // changes the index...
    new_header->e_shnum=sections_count;
    new_header->e_type=ET_EXEC;

    *new_sec_name = new_section_header_name(nom_sections, new_sections_header, *new_header);
    // en théorie ici on doit faire la nouvelle table des symboles
	// donc ici on peut faire change_idx_and_name_symb_table

	// en enfin on peut changer les offsets des sections
	for (i=0; i<new_header->e_shnum; ++i) {

		if (i == 1) {
			new_sections_header[i].sh_offset = new_sections_header[i].sh_addr + last_size + last_offset + new_sections_header[i].sh_offset;
			last_offset = section_headers[i].sh_offset - section_headers[i].sh_addr;
		} else {
			new_sections_header[i].sh_offset = new_sections_header[i].sh_addr + last_size + last_offset;
			last_size = new_sections_header[i].sh_size;
			last_offset = new_sections_header[i].sh_offset;
		}

//		printf("AUFÇ7: %x, TAYE: %x, NON SEXION DASSO: %s\n", last_offset, last_size, &((*new_sec_name)[new_sections_header[i].sh_name]));

	}


    return new_sections_header;
}

void change_idx_and_name_symb_table(Elf32_Sym* symbols, int symb_count, Elf32_Shdr* section_headers) {
	int i;

	for (i=0; i<symb_count; ++i) {
        symbols[i].st_shndx = old_sec_to_new_sec[symbols[i].st_shndx];
        symbols[i].st_name = section_headers[symbols[i].st_shndx].sh_name;
	}
}


uint8_t** new_section_content (	Table_rel_set table_rel,
								char* sections_name,
								uint8_t** section_content,
								rel_info* infos,
								Elf32_Shdr * section_headers,
								/*Elf32_Shdr * new_section_headers,*/
								Elf32_Ehdr header,
								int rel_count,
								Elf32_Sym *symbols,
								Elf32_Sym *new_symbols
								)
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
                     int32_t result_fusion32 = 0;
                     // convert array of 4 bytes into one integer of 32 bits
                     uint32_t P;
                     Elf32_Sym S;
					int symb_idx = old_sym_to_new_sym[((!(ELF32_R_SYM(table_rel.rel_section_list[i].rel_list[j].r_info)==STN_UNDEF)) * ELF32_R_SYM(table_rel.rel_section_list[i].rel_list[j].r_info))];
                     S = new_symbols[symb_idx];
                     //A = result_fusion32;
                     P = section_headers[S.st_shndx].sh_addr + table_rel.rel_section_list[i].rel_list[j].r_offset;
                     switch (ELF32_R_TYPE(table_rel.rel_section_list[i].rel_list[j].r_info)) {
                         case R_ARM_ABS32:
                            result_fusion32 = (to_fuse[0] << 24) | (to_fuse[1] << 16) | (to_fuse[2] << 8) | (to_fuse[3]);
                            result_fusion32 += S.st_value;
                        break;

                         case R_ARM_ABS16:
                            result_fusion32 = (to_fuse[2] << 8) | (to_fuse[3]);
                            if (result_fusion32 & 0x8000) {
                                result_fusion32 |= ~0xFFFF;
                            }
                            result_fusion32 += S.st_value;
                        break;

                        case R_ARM_ABS8:
                            result_fusion32 = (to_fuse[3]);
                            if (result_fusion32 & 0x80) {
                                result_fusion32 |= ~0xFF;
                            }
                            result_fusion32 += S.st_value;
                        break;

                         case R_ARM_JUMP24:
                         case R_ARM_CALL:
                            result_fusion32 = (to_fuse[2] << 16) | (to_fuse[1] << 8) | (to_fuse[0]);
							result_fusion32 <<= 2;
							result_fusion32 += S.st_value;
							result_fusion32 -= P;
                            result_fusion32 &= 0x03FFFFFE;
                            result_fusion32 >>= 2;
                            result_fusion32 = (result_fusion32 & 0x00FFFFFF) | to_fuse[3] << 24;
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

     return section_cpy;
}

Elf32_Sym *new_symbol_table(Elf32_Sym *symb_table, rel_info *info, uint32_t symb_count, uint32_t* new_symb_count, uint32_t rel_count, Elf32_Shdr *sections_header, char* section_name, Elf32_Shdr *new_sections_header){
    uint32_t i, j;

	*new_symb_count = 0;
    // count number of useless symb
    for (i=0; i<symb_count; i++) {
		*new_symb_count += (sections_header[symb_table[i].st_shndx].sh_type!=SHT_NOBITS);
    }

    Elf32_Sym *new_symb_table=malloc(sizeof(Elf32_Sym)*(*new_symb_count));
	j=0;
    for (i=0; i<symb_count; i++) {
		if (sections_header[symb_table[i].st_shndx].sh_type!=SHT_NOBITS) {
			memcpy(&new_symb_table[j], &symb_table[i], sizeof(Elf32_Sym));
			old_sym_to_new_sym[i]=j;
			j++;
		}
    }
	change_idx_and_name_symb_table(new_symb_table, *new_symb_count, new_sections_header);

    for(i=0 ; i<*new_symb_count ; i++){
        new_symb_table[i].st_value += new_sections_header[new_symb_table[i].st_shndx].sh_addr;
    }
    return new_symb_table;
}

/** creates new char* section_name that contains the name of the sections in the EXEC file
*/
char* new_section_header_name(char* section_header_name, Elf32_Shdr *new_sections_header, Elf32_Ehdr new_header) {
    char *new_sec_header_name;
    uint32_t i,size=0,current_pos = 0;

    for (i=0; i<new_header.e_shnum; i++) {
        size += my_strlen(&section_header_name[new_sections_header[i].sh_name])+1;
    }
    new_sec_header_name = malloc(size*sizeof(char));

    for(i=0 ; i<new_header.e_shnum ; i++){
        strcpy(&new_sec_header_name[current_pos], &section_header_name[new_sections_header[i].sh_name]);
        new_sections_header[i].sh_name = current_pos;
        current_pos += my_strlen(&new_sec_header_name[current_pos]);
    }

    new_sections_header[new_header.e_shstrndx].sh_size = size;

    return new_sec_header_name;
}


size_t my_strlen(const char* str) {
    char c = 1;
    size_t size = 0;
    while (c != '\0') {
        c = str[size++];
    }

    return size;
}


