#include "../commun/readelf.h"
#include "relocalise.h"

int main(int argc, char **argv) {

// APPEL : relocalisation <nom_fichier> <nom_section adresse_relocalisee> [nom_section_x adresse_relocalisee_x]

    init_systable();
    init_systarget();
    FILE *file = fopen(argv[1], "rb");

    if (file == NULL) {
        printf("Le fichier n'existe pas.\n");
        return EXIT_FAILURE;
    }

    Elf32_Ehdr header;
    Elf32_Shdr *table_entetes_section = NULL;
    char *table_nom_sections = NULL;
    uint16_t symbols_count = 0;
    uint8_t **section_content;

    // load everything before printing
    read_elf_header(file, &header);

    // check if magic numbers are correct or not
    if (    header.e_ident[EI_MAG0] != ELFMAG0
        ||  header.e_ident[EI_MAG1] != ELFMAG1
        ||  header.e_ident[EI_MAG2] != ELFMAG2
        ||  header.e_ident[EI_MAG3] != ELFMAG3) {
            printf("Erreur : Les nombres magiques ne sont pas corrects.\n");
            return ERROR_MAGIC_NUMBERS;
    }

    table_entetes_section = read_elf_section_header(file, &header, &table_nom_sections);
    Elf32_Sym *symbols = read_symbol_table(file, table_entetes_section,/* header.e_shnum,*/ &symbols_count);
    section_content = read_section_content(file, table_entetes_section, &header);
    Ensemble_table_rel table_rel= read_rel_table(file, table_entetes_section, header.e_shnum);
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
    free(symbols);
    free(table_entetes_section);
    free(table_nom_sections);
//    free(table_rela);
//    free(table_rel);
//    free(table_rel_info);
//    free(section_content);

    return 0;


}
