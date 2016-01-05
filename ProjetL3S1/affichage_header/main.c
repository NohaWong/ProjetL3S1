#include "readelf.h"

extern char sys_table[256][32];
extern char sys_target[193][32];

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

    print_elf_header(file, &header);

    if (    header.magic_number[EI_MAG0] != ELFMAG0
        ||  header.magic_number[EI_MAG1] != ELFMAG1
        ||  header.magic_number[EI_MAG2] != ELFMAG2
        ||  header.magic_number[EI_MAG3] != ELFMAG3) {
            printf("Erreur : Les nombres magiques ne sont pas corrects.\n");
            return ERROR_MAGIC_NUMBERS;
    }

    int i = 0;
    printf("  Magique : ");
    for (i = 0; i < 4; ++i) {
        printf("%x ", header.magic_number[i]);
    }
    printf("\n");

    printf("  Taille d'un word : ");
    if (header.word_size == ELFCLASS32) {
        printf("ELF32\n");
    } else if (header.word_size == ELFCLASS64) {
        printf("ELF64\n");
        return ERROR_WRONG_WORD_SIZE;
    } else {
        printf("Taille invalide");
        return ERROR_WRONG_WORD_SIZE;
    }

    printf("  Endianness : ");
    if (header.endianness == ELFDATA2LSB) {
        printf("Little endian\n");
    } else if (header.endianness == ELFDATA2MSB) {
        printf("Big endian\n");
    } else {
        printf("Endianess invalide");
        return ERROR_WRONG_ENDIAN;
    }

    printf("  Version ELF : ");
    if (header.version >= EV_CURRENT) {
        printf("%d (current)\n", header.version);
    } else if (header.version == EV_NONE) {
        printf("%d\n", header.version);
        return ERROR_INVALID_VERSION;
    }

    printf("  OS/ABI : ");
    printf("%s\n", sys_table[header.sys_type]);

    printf("  Type de fichier : ");
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

    printf("  Machine cible : %s\n", sys_target[header.sys_target]);
    printf("  Version : 0x%x\n", header.sys_version);
    printf("  Point d'entrée : 0x%x\n", header.entry_point);
    printf("  Flags : 0x%x\n", header.flags);
    printf("  Taille de l'en-tête ELF : %d octets\n", header.header_size);

    printf("\n");

    return EXIT_SUCCESS;
}
