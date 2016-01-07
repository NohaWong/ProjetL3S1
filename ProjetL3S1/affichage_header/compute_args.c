#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "compute_args.h"

int compute_multiple_args (int argc, char **argv) {
    if (strcmp(argv[1], "--help") == 0) {
        print_help();
        return EXIT_SUCCESS;
    }

    init_systable();
    init_systarget();
    FILE *file = fopen(argv[argc - 1], "rb");
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

    table_entetes_section = read_elf_section_header(file, &header, &table_nom_sections);table_entetes_section = read_elf_section_header(file, &header, &table_nom_sections);
    Elf32_Sym *symbols = read_symbol_table(file, table_entetes_section, header.e_shnum, &symbols_count);
    section_content = read_section_content(file, table_entetes_section, &header);
    TableRel * table_rel= read_rel_table(file, table_entetes_section, header.e_shnum);
    TableRela * table_rela= read_rela_table(file, table_entetes_section, header.e_shnum);

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
                    case 'r':
                    {
                        print_elf_rel_tab(table_rel);
                        // print rel table
                        break;
                    }
                     case 'R':
                    {
                        print_elf_rela_tab(table_rela);
                        // print rel table
                        break;
                    }
                    case 'S':
                    {
                        // affichage de l'entete des sections
                        print_elf_section_header(header, table_entetes_section, table_nom_sections);
                        // print all sections
                        break;
                    }
                    case 'x':
                    {
                        // skip the section name that doesn't exists for the next argument
                        i++;
                        int secnum;
                        if (!is_numeric(argv[i])) {
                            secnum = section_name_to_number(argv[i], table_entetes_section, table_nom_sections, &header);
                            if (secnum == -1) {
                                printf("La section \"%s\" n'existe pas.\n", argv[i]);
                                continue;
                            }
                        } else {
                            secnum = strtol(argv[i], NULL, 10);
                        }

                        print_elf_section_content(section_content, secnum, table_entetes_section, argv[i]);
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
    printf("Manuel utilisateur - our_readelf\n\n");
    printf(BOLDWHITE "NOM\n" RESET);
    printf("\tour_readelf - lit un fichier au format ELF et affiche ses caractéristiques\n\n");
    printf(BOLDWHITE "SYNOPSIS\n");
    printf("\tour_readelf [-hsSrRx]" RESET " FICHIER\n\n");
    printf(BOLDWHITE "DESCRIPTION\n" RESET);
    printf("\tLit un fichier au format ELF et affiche les caractéristiques voulues à l'écran.\n\tSi aucun argument n'est spécifié, un menu apparait à l'écran pour choisir l'opération souhaitée.\n\n");
    printf(BOLDWHITE "\t-h\n" RESET);
    printf("\t\tAffiche l'en-tête (header) du fichier ELF spécifié en entrée.\n\n");
    printf(BOLDWHITE "\t-s\n" RESET);
    printf("\t\tAffiche la table des symboles du fichier ELF spécifié en entrée.\n\n");
    printf(BOLDWHITE "\t-S\n" RESET);
    printf("\t\tAffiche l'en-tête (header) des sections du fichier ELF spécifié en entrée.\n\n");
    printf(BOLDWHITE "\t-x <nombre ou nom>\n" RESET);
    printf("\t\tAffiche le contenu de la section donnée par son numéro ou son nom.\n\n");
    printf(BOLDWHITE "\t--help\n" RESET);
    printf("\t\tAffiche cette aide.\n\n");
    printf(BOLDWHITE "\t-r\n" RESET);
    printf("\t\tAffiche la table de réimplantation statique.\n\n");
    printf(BOLDWHITE "\t-R\n" RESET);
    printf("\t\tAffiche la table de réimplantation dynamique.\n\n");
    printf(BOLDWHITE "    Valeurs de retour :\n" RESET);
    printf("\t0\tLe programme s'est terminé normalement.\n\n");
    printf("\t1\tLes nombres magiques sont erronés.\n\n");
    printf("\t2\tLe nombre d'arguments envoyés au programme est inccorect.\n\n");
    printf("\t3\tLe fichier ELF n'est pas au format 32-bits.\n\n");
    printf("\t4\tL'endianness du fichier n'est pas défini.\n\n");
    printf("\t5\tLa version du fichier ELF n'est pas valide.\n\n");
    printf("\t6\tUne erreur inconnue s'est produite.\n\n");
    printf(BOLDWHITE "AUTEURS\n" RESET);
    printf("\tÉcrit par Alexandre Daubois, Maxence Ginet, Sylvain Marion, Hugo Serem et Noha Wong.\n\n");
}

// Récupéré sur http://rosettacode.org/wiki/Determine_if_a_string_is_numeric#C
int is_numeric (const char * s) {
    if (s == NULL || *s == '\0' || isspace(*s)) {
        return 0;
    }
    char * p;
    strtol(s, &p, 10);
    return *p == '\0';
}
