#include "readelf.h"
#include "printelf.h"

int main(int argc, char **argv) {
    printf("En-tête ELF:\n");

        int return_value = EXIT_SUCCESS;

    if (argc < 2) {
        print_help();
        return ERROR_MISSING_ARG;
    }

    if (argc >= 3) {
        return_value = compute_multiple_args(argc, argv);
    }
    if (argc != 2) {
        printf("Utilisation : affichage_header fichier\n");
        return ERROR_MISSING_ARG;
    }

    init_systable();
    init_systarget();
    FILE *file = fopen(argv[1], "rb");
    Elf32_Ehdr header;
    Elf32_Shdr *table_entetes_section = NULL;
    char *table_nom_sections = NULL;
//    int16_t i;

    read_elf_header(file, &header);
    int value = 0;//print_elf_header(header);
    if (value) {
        printf("Le programme s'est terminé avec le code %d.\n", value);
        return value;
    }
    print_elf_header(header);

    // lecture de l'entete des sections
    table_entetes_section = read_elf_section_header(file, &header, &table_nom_sections);
    // affichage de l'entete des sections
    print_elf_section_header(header, table_entetes_section, table_nom_sections);
    // table des symboles
    uint16_t symbols_count = 0;
    Elf32_Sym *symbols = read_symbol_table(file, table_entetes_section, header.e_shnum, &symbols_count);
    print_elf_symbol_table(symbols, symbols_count);

    TableRel * tab = read_rel_table(file,table_entetes_section,header.e_shnum);
    print_elf_rel_tab(tab);

    free(table_entetes_section);
    free(symbols);
    free(table_nom_sections);

    return EXIT_SUCCESS;
}
