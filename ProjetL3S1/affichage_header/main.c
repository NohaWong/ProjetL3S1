#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
void init_systable();

char sys_table[256][32];

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

void print_elf_header(FILE *file, elf_header *header) {
    fseek(file, 0, SEEK_SET);
    fread(header->magic_number, sizeof(int8_t), 4, file);
    fread(&header->word_size, sizeof(int8_t), 1, file);
    fread(&header->endianess, sizeof(int8_t), 1, file);
    fread(&header->version, sizeof(int8_t), 1, file);
    fread(&header->sys_type, sizeof(int8_t), 1, file);
}

void init_systable() {
    strcpy(sys_table[0], "UNIX System V");
    strcpy(sys_table[1], "HP-UX");
    strcpy(sys_table[2], "NetBSD");
    strcpy(sys_table[3], "Linux");
    strcpy(sys_table[6], "Sun Solaris");
    strcpy(sys_table[7], "IBM AIX");
    strcpy(sys_table[8], "SGI Irix");
    strcpy(sys_table[9], "FreeBSD");
    strcpy(sys_table[10], "Compaq TRU64");
    strcpy(sys_table[11], "Novell Modesto");
    strcpy(sys_table[12], "OpenBSD");
    strcpy(sys_table[64], "ARM EABI");
    strcpy(sys_table[97], "ARM");
    strcpy(sys_table[255], "Standalone");
}
