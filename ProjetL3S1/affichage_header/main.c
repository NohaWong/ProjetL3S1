#include <stdio.h>
#include <stdlib.h>
#include <elf.h>

typedef struct elf_header elf_header;
struct elf_header {
    int8_t magic_number[4];
    Elf32_Half word_size;
    Elf32_Half endianess;
    Elf32_Half file_type;
    Elf32_Half version;
    Elf32_Half sys_type;
};

enum { ERROR_MAGIC_NUMBERS = 1, ERROR_MISSING_ARG, ERROR_WRONG_WORD_SIZE, ERROR_WRONG_ENDIAN,
       ERROR_INVALID_VERSION };

void print_elf_header(FILE *file, elf_header *header);

char sys_table[][32] = {
    "UNIX System V",
    "HP-UX",
    "NetBSD",
    "Linux",
    "Sun Solaris",
    "IBM AIX",
    "SGI Irix",
    "FreeBSD",
    "Compaq TRU64",
    "Novell Modesto",
    "OpenBSD",
    "ARM EABI",
    "ARM",
    "Standalone"
};

int main(int argc, char **argv) {
    printf("--- Affichage du header ELF ---\n\n");

    if (argc != 2) {
        printf("Utilisation : affichage_header fichier\n");
        return ERROR_MISSING_ARG;
    }

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
    switch (header.sys_type) {
        case 0:
            printf("%s", sys_table[0]);
            break;
        case 1:
            printf("%s", sys_table[1]);
            break;
        case 2:
            printf("%s", sys_table[2]);
            break;
        case 3:
            printf("%s", sys_table[3]);
            break;
        case 6:
            printf("%s", sys_table[4]);
            break;
        case 7:
            printf("%s", sys_table[5]);
            break;
        case 8:
            printf("%s", sys_table[6]);
            break;
        case 9:
            printf("%s", sys_table[7]);
            break;
        case 10:
            printf("%s", sys_table[8]);
            break;
        case 11:
            printf("%s", sys_table[9]);
            break;
        case 12:
            printf("%s", sys_table[10]);
            break;
        case 64:
            printf("%s", sys_table[11]);
            break;
        case 97:
            printf("%s", sys_table[12]);
            break;
        case 255:
            printf("%s", sys_table[13]);
            break;
    }

    printf("\n");

    return EXIT_SUCCESS;
}

void print_elf_header(FILE *file, elf_header *header) {
    fseek(file, 0, SEEK_SET);
    fread(header->magic_number, sizeof(int8_t), 4, file);
    fread(&header->word_size, sizeof(int8_t), 1, file);
    fread(&header->endianess, sizeof(int8_t), 1, file);
    fread(&header->version, sizeof(int8_t), 1, file);
    fread(&header->sys_type, sizeof(int8_t), 1, file);
}
