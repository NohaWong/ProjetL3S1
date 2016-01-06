#include <stdio.h>
#include "compute_args.h"

int compute_multiple_args (int argc, char **argv) {
    init_systable();
    init_systarget();
    FILE *file = fopen(argv[argc - 1], "rb");
    Elf32_Ehdr header;
    Elf32_Shdr *table_entetes_section = NULL;
    char *table_nom_sections = NULL;
    uint16_t symbols_count = 0;

    // load everything before printing
    read_elf_header(file, &header);
    table_entetes_section = read_elf_section_header(file, &header, &table_nom_sections);table_entetes_section = read_elf_section_header(file, &header, &table_nom_sections);
    Elf32_Sym *symbols = read_symbol_table(file, table_entetes_section, header.e_shnum, &symbols_count);

    int i;
    for (i = 1; i < argc - 1; ++i) {
        if (strcmp(argv[i], "--help") == 0) {
            print_help();
            return EXIT_SUCCESS;
        } else if (argv[i][0] == '-') {
            // check arg value
            size_t len = strlen(argv[i]);
            int j;
            // j = 1, skip '-'
            for (j = 1; j < len; ++j) {
                switch (argv[i][j]) {
                    case 'h':
                    {
                        // print header
                        int value = 0;
                        if (value) {
                            printf("Le programme s'est terminé avec le code %d.\n", value);
                            return value;
                        }
                        print_elf_header(header);
                        break;
                    }
                    case 's':
                    {
                        print_elf_symbol_table(symbols, symbols_count);
                        // print symbol table
                        break;
                    }
                    case 'S':
                    {
                        // affichage de l'entete des sections
                        print_elf_section_header(header, table_entetes_section, table_nom_sections);
                        // print all sections
                        break;
                    }
                    default:
                        // unknown argument
                        break;
                }
            }
        } else {
            break;
        }
    }

    free(symbols);
    free(table_entetes_section);
    free(table_nom_sections);

    return EXIT_SUCCESS;
}

int compute_no_args (const char *filename) {
    // TODO
    return EXIT_SUCCESS;
}

void print_help() {
    // TODO
}
