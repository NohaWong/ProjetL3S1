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
    elf_header header;

    read_elf_header(file, &header);
    int value = print_elf_header(header);

    printf("Le programme s'est terminé avec le code %d.\n", value);

    return EXIT_SUCCESS;
}
