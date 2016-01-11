#include "relocalise.h"

Elf32_Shdr* new_section_header(Elf32_Shdr* section_headers, char* nom_sections, rel_info* infos, int nb_relocalisations, Elf32_Ehdr header,Elf32_Ehdr *new_header) {
int i,k;
//    {'', '.', 't', 'e', 'x', 't', '\0', '.', 'd', 'a'...}
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


    for (j=0; j<nb_relocalisations; j++) {
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



void new_section_content (Ensemble_table_rel table_rel, char* nom_sections, uint8_t** section_content, rel_info* infos, Elf32_Shdr * section_headers,Elf32_Ehdr *header,int nb_relocalisation,Elf32_Sym *symbols) {

    //uint32_t* tempo= NULL;
    int j,i,k,section_to_change;
    //char *b;
    // Each different relocalization's table
    for (j=0; j<table_rel.section_count_rel; j++) {
        // For each argument name 
        for(k=0;k<nb_relocalisation;k++){
            // If the relocalization table's name is equal to the argument 
            if(!strcmp(&nom_sections[table_rel.rel_section_list[j].section_name+4],infos[k].section_name)){
                //For each row in relocalization's table 
                for (i=0;i<table_rel.rel_section_list[j].elem_count;i++) {
                    // Name's row is equal to argument  
                    
                    if(!strcmp(&nom_sections[rel_info_to_symbol(table_rel.rel_section_list[j].rel_list[i].r_info,symbols,section_headers)],infos[k].section_name)) {
                        section_to_change = section_name_to_number (&nom_sections[table_rel.rel_section_list[j].section_name],section_headers,nom_sections,header);
                        //memcpy(tempo,table_rel.,4);
                        printf("section to change : %i  \n",section_to_change );

                    }
                }
                
            }
        }
        

        
    }


}





