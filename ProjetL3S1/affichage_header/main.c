#include "readelf.h"

extern char sys_table[256][32];

int main(int argc, char **argv) {
    printf("--- Affichage du header ELF ---\n\n");

    if (argc != 2) {
        printf("Utilisation : affichage_header fichier\n");
        return ERROR_MISSING_ARG;
    }

    init_systable();
    FILE *file = fopen(argv[1], "rb");
    elf_header header;

    print_elf_header(file, &header);

    if (    header.magic_number[0] != ELFMAG0
        ||  header.magic_number[1] != ELFMAG1
        ||  header.magic_number[2] != ELFMAG2
        ||  header.magic_number[3] != ELFMAG3) {
            printf("Erreur : Les nombres magiques ne sont pas corrects.\n");
            return ERROR_MAGIC_NUMBERS;
    }

    int i = 0;
    printf("> Magique : ");
    for (i = 0; i < 4; ++i) {
        printf("%x ", header.magic_number[i]);
    }
    printf("\n");

    printf("> Type de fichier : ");
    if (header.word_size == 1) {
        printf("ELF32\n");
    } else if (header.word_size == 2) {
        printf("ELF64\n");
    } else {
        printf("Taille invalide");
        return ERROR_WRONG_WORD_SIZE;
    }

    printf("> Endianess : ");
    if (header.endianess == 1) {
        printf("Little endian\n");
    } else if (header.endianess == 2) {
        printf("Big endian\n");
    } else {
        printf("Endianess invalide");
        return ERROR_WRONG_ENDIAN;
    }

    printf("> Version ELF : ");
    if (header.version == EV_CURRENT) {
        printf("%d\n", EV_CURRENT);
    } else if (header.version == EV_NONE) {
        printf("%d\n", EV_NONE);
        return ERROR_INVALID_VERSION;
    }

    printf("> OS/ABI : ");
    printf("%s\n", sys_table[header.sys_type]);

    printf("\n");

    return EXIT_SUCCESS;
}
