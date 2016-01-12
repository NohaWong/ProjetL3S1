#include "../commun/readelf.h"
#include "../affichage/printelf.h"
#include "relocalise.h"
#include "write_file.h"



int main(int argc, char **argv) {
    char type[16],info[16];
    uint32_t i;
// APPEL : relocalisation <nom_fichier> <nom_section adresse_relocalisee> [nom_section_x adresse_relocalisee_x]

    if (argc < 3){
    printf("Pas le bon nombre d'argument\n");

    return 2;
    }
    else{
    init_systable();
    init_systarget();

    FILE *file_source = fopen(argv[1], "rb");
    FILE *file_target = fopen(argv[2], "wb");

    if (file_source == NULL ) {
        printf("Le fichier source n'existe pas.\n");
        return EXIT_FAILURE;
    }

    if (file_target == NULL ){
       printf("Le fichier cible n'existe pas.\n");
       fclose(file_source);
       return EXIT_FAILURE;
    }

    Elf32_Ehdr header;

    Elf32_Shdr *section_header_table = NULL;

    Elf32_Ehdr* new_header= malloc(sizeof(Elf32_Ehdr));


    char *table_nom_sections = NULL;
    uint16_t symbols_count = 0;
    uint8_t **section_content;
    Elf32_Sym *new_symb_table;

    // load everything before printing
    read_elf_header(file_source, &header);

    memcpy(new_header, &header, sizeof(Elf32_Ehdr));
    write_file_header(file_target,new_header);


    // check if magic numbers are correct or not
    if (    header.e_ident[EI_MAG0] != ELFMAG0
        ||  header.e_ident[EI_MAG1] != ELFMAG1
        ||  header.e_ident[EI_MAG2] != ELFMAG2
        ||  header.e_ident[EI_MAG3] != ELFMAG3) {
            printf("Erreur : Les nombres magiques ne sont pas corrects.\n");
            return ERROR_MAGIC_NUMBERS;
    }

    section_header_table = read_elf_section_header(file_source, &header, &table_nom_sections);
    Elf32_Sym *symbols = read_symbol_table(file_source, section_header_table,/* header.e_shnum,*/ &symbols_count);
    section_content = read_section_content(file_source, section_header_table, &header);
    Ensemble_table_rel table_rel= read_rel_table(file_source, section_header_table, header.e_shnum);
//    TableRela * table_rela= read_rela_table(file, section_header_table, header.e_shnum);
    printf("#      Nom         Valeur      Type      Portée    Indice de section\n");
        printf("--------------------------------------------------------------------\n");
        for (i = 0; i < symbols_count; ++i) {
            switch (ELF32_ST_TYPE(symbols[i].st_info)) {
                case STT_NOTYPE:
                default:
                    strcpy(type, "NOTYPE");
                    break;
                case STT_OBJECT:
                    strcpy(type, "OBJECT");
                    break;
                case STT_FUNC:
                    strcpy(type, "FUNC");
                    break;
                case STT_SECTION:
                    strcpy(type, "SECTION");
                    break;
                case STT_FILE:
                    strcpy(type, "FILE");
                    break;
            }

            //bind
            switch(ELF32_ST_BIND(symbols[i].st_info)) {
                case STB_LOCAL:
                    strcpy(info, "LOCAL");
                    break;
                case STB_GLOBAL:
                    strcpy(info, "GLOBAL");
                    break;
                case STB_WEAK:
                    strcpy(info, "WEAK");
                    break;
                case STB_NUM:
                    strcpy(info, "NUM");
                    break;
                default:
                    strcpy(info, "OTHER");
                    break;
            }

            printf("%-7d%#-12x%#-12x%-10s%-10s%-2d", i, symbols[i].st_name, symbols[i].st_value, type, info, symbols[i].st_shndx);
            printf("\n");
        }
        printf("\n");



    uint32_t nb_relocalisation = (argc-2)/2;

    rel_info* table_rel_info;
    table_rel_info = malloc(nb_relocalisation * sizeof(rel_info));

    for (i=0; i<nb_relocalisation; ++i) {
        table_rel_info[i].section_name = argv[(2*i)+3];
        table_rel_info[i].section_new_addr = strtol(argv[((2*i)+1)+3], NULL, 16);
//        printf("%s    ;    %i", (table_rel_info[i].section_name), table_rel_info[i].section_new_addr);
    }


    Elf32_Shdr * new_sections_header = new_section_header(section_header_table, table_nom_sections, table_rel_info, nb_relocalisation, header,new_header);

    for (i=0; i < header.e_shnum; i++) {
        printf("%-6d%-20s%#-12x%#-8x%#-8x(+ %#-8x) %#-8x%#-8x%#-11x%#-8x", i,
                           &(table_nom_sections[section_header_table[i].sh_name]),
                           section_header_table[i].sh_type,
                           section_header_table[i].sh_flags,
                           section_header_table[i].sh_addr,
                           section_header_table[i].sh_offset,
                           section_header_table[i].sh_size,
                           section_header_table[i].sh_link,
                           section_header_table[i].sh_addralign,
                           section_header_table[i].sh_entsize
              );

        printf("\n");
    }
    new_section_content (table_rel,table_nom_sections,section_content,table_rel_info, section_header_table, &header,nb_relocalisation,symbols);
    new_symb_table= new_symbol_table(symbols, table_rel_info, symbols_count, nb_relocalisation, section_header_table, table_nom_sections);


    printf("#      Nom         Valeur      Type      Portée    Indice de section\n");
        printf("--------------------------------------------------------------------\n");
        for (i = 0; i < symbols_count; ++i) {
            switch (ELF32_ST_TYPE(new_symb_table[i].st_info)) {
                case STT_NOTYPE:
                default:
                    strcpy(type, "NOTYPE");
                    break;
                case STT_OBJECT:
                    strcpy(type, "OBJECT");
                    break;
                case STT_FUNC:
                    strcpy(type, "FUNC");
                    break;
                case STT_SECTION:
                    strcpy(type, "SECTION");
                    break;
                case STT_FILE:
                    strcpy(type, "FILE");
                    break;
            }

            //bind
            switch(ELF32_ST_BIND(new_symb_table[i].st_info)) {
                case STB_LOCAL:
                    strcpy(info, "LOCAL");
                    break;
                case STB_GLOBAL:
                    strcpy(info, "GLOBAL");
                    break;
                case STB_WEAK:
                    strcpy(info, "WEAK");
                    break;
                case STB_NUM:
                    strcpy(info, "NUM");
                    break;
                default:
                    strcpy(info, "OTHER");
                    break;
            }

            printf("%-7d%#-12x%#-12x%-10s%-10s%-2d", i, new_symb_table[i].st_name, new_symb_table[i].st_value, type, info, new_symb_table[i].st_shndx);
            printf("\n");
        }
        printf("\n");

    free(new_symb_table);


    for (i=0; i < header.e_shnum; i++) {
        printf("%-6d%-20s%#-12x%#-8x%#-8x(+ %#-8x) %#-8x%#-8x%#-11x%#-8x", i,
                           &(table_nom_sections[new_sections_header[i].sh_name]),
                           new_sections_header[i].sh_type,
                           new_sections_header[i].sh_flags,
                           new_sections_header[i].sh_addr,
                           new_sections_header[i].sh_offset,
                           new_sections_header[i].sh_size,
                           new_sections_header[i].sh_link,
                           new_sections_header[i].sh_addralign,
                           new_sections_header[i].sh_entsize
              );

        printf("\n");
    }

    //write_file_header(file_target,new_header);

    new_section_content (table_rel,table_nom_sections,section_content,table_rel_info, section_header_table, &header,nb_relocalisation,symbols);




    free(symbols);
    free(section_header_table);
    free(table_nom_sections);

    fclose(file_source);
    fclose(file_target);//    free(table_rela);

    free(new_sections_header);
    free(new_header);
//    free(table_rela);
//    free(table_rel);
//    free(table_rel_info);
//    free(section_content);

    return 0;
}

}


