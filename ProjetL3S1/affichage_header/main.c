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
    header.version = 0;

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

    printf("> Taille d'un word : ");
    if (header.word_size == ELFCLASS32) {
        printf("ELF32\n");
    } else if (header.word_size == ELFCLASS64) {
        printf("ELF64\n");
    } else {
        printf("Taille invalide");
        return ERROR_WRONG_WORD_SIZE;
    }

    printf("> Endianess : ");
    if (header.endianess == ELFDATA2LSB) {
        printf("Little endian\n");
    } else if (header.endianess == ELFDATA2MSB) {
        printf("Big endian\n");
    } else {
        printf("Endianess invalide");
        return ERROR_WRONG_ENDIAN;
    }

    printf("> Version ELF : ");
    if (header.version >= EV_CURRENT) {
        printf("%d (current)\n", header.version);
    } else if (header.version == EV_NONE) {
        printf("%d\n", header.version);
        return ERROR_INVALID_VERSION;
    }

    printf("> OS/ABI : ");
    printf("%s\n", sys_table[header.sys_type]);

    printf("> Type de fichier : ");
    if (header.file_type == ET_NONE) {
        printf("Aucun\n");
    } else if (header.file_type == ET_REL) {
        printf("Repositionnable (REL)\n");
    } else if (header.file_type == ET_EXEC) {
        printf("Exécutable (EXEC)\n");
    } else if (header.file_type == ET_DYN) {
        printf("Dyanmique (partagé)");
    } else if (header.file_type == ET_CORE) {
        printf("Fichier core (CORE)");
    }

    printf("\n");

    return EXIT_SUCCESS;
}
