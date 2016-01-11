#include "../commun/readelf.h"
#include "../affichage/printelf.h"
#include "relocalise.h"
#include "write_file.h"

int main(int argc, char **argv) {

// APPEL : relocalisation <nom_fichier> <nom_section adresse_relocalisee> [nom_section_x adresse_relocalisee_x]

    if (argc != 3){
    printf("Pas le bon d'argument\n");

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
    char *table_nom_sections = NULL;
    uint16_t symbols_count = 0;
   // uint8_t **section_content;

    // load everything before printing
    read_elf_header(file_source, &header);
    write_file_header(file_target,&header);

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

/*    section_content = read_section_content(file_source, table_entetes_section, &header);
    // Ensemble_table_rel table_rel= read_rel_table(file_source, table_entetes_section, header.e_shnum);
//    TableRela * table_rela= read_rela_table(file, table_entetes_section, header.e_shnum);

    uint32_t nb_relocalisation = (argc-1)/2, i;

    rel_info* table_rel_info;
    table_rel_info = malloc(nb_relocalisation * sizeof(rel_info));

    for (i=0; i<nb_relocalisation; ++i) {
        table_rel_info[i].section_name = argv[2*(i+1)];
        table_rel_info[i].section_new_addr = strtol(argv[2*(i+1)+1], NULL, 16);
//        printf("%s    ;    %i", (table_rel_info[i].section_name), table_rel_info[i].section_new_addr);
    }


    new_section_header(table_entetes_section, table_nom_sections, table_rel_info, nb_relocalisation, header);
    for (i=0; i < header.e_shnum; i++) {
        printf("%-6d%-20s%#-12x%#-8x%#-8x(+ %#-8x) %#-8x%#-8x%#-11x%#-8x", i,
                           &(table_nom_sections[table_entetes_section[i].sh_name]),
                           table_entetes_section[i].sh_type,
                           table_entetes_section[i].sh_flags,
                           table_entetes_section[i].sh_addr,
                           table_entetes_section[i].sh_offset,
                           table_entetes_section[i].sh_size,
                           table_entetes_section[i].sh_link,
                           table_entetes_section[i].sh_addralign,
                           table_entetes_section[i].sh_entsize
              );

        printf("\n");
    }

    new_section_content (table_rel,table_nom_sections,section_content,table_rel_info, table_entetes_section, &header,nb_relocalisation,symbols);

    */

    free(symbols);
    free(section_header_table);
    free(table_nom_sections);
    fclose(file_source);
    fclose(file_target);//    free(table_rela);
//    free(table_rel);
//    free(table_rel_info);
//    free(section_content);

    return 0;
}

}
