#include "readelf.h"
#include "printelf.h"

int main(int argc, char **argv) {
    printf("En-tête ELF:\n");

    if (argc != 2) {
        printf("Utilisation : affichage_header fichier\n");
        return ERROR_MISSING_ARG;
    }

    init_systable();
    init_systarget();
    FILE *file = fopen(argv[1], "rb");
    Elf32_Ehdr header;
    Elf32_Shdr *table_entetes_section;


    read_elf_header(file, &header);
    int value = 0;//print_elf_header(header);
    if (value) {
        printf("Le programme s'est terminé avec le code %d.\n", value);
        return value;
    }
    print_elf_header(header);

    // lecture de l'entete des sections
    table_entetes_section = read_elf_section_header(file, &header);


    // affichage de l'entete des sections
    print_elf_section_header(header, table_entetes_section);
    free(table_entetes_section);

    return EXIT_SUCCESS;
}
