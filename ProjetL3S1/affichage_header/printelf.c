#include "printelf.h"

extern char sys_table[256][32];
extern char sys_target[193][32];

int print_elf_header(Elf32_Ehdr header)
{
    if (    header.e_ident[EI_MAG0] != ELFMAG0
        ||  header.e_ident[EI_MAG1] != ELFMAG1
        ||  header.e_ident[EI_MAG2] != ELFMAG2
        ||  header.e_ident[EI_MAG3] != ELFMAG3) {
            printf("Erreur : Les nombres magiques ne sont pas corrects.\n");
            return ERROR_MAGIC_NUMBERS;
    }

    int i = 0;
    printf("  Magique : ");
    for (i = 0; i < 4; ++i) {
        printf("%x ", header.e_ident[i]);
    }
    printf("\n");

    printf("  Taille d'un word : ");
    if (header.e_ident[EI_CLASS] == ELFCLASS32) {
        printf("ELF32\n");
    } else if (header.e_ident[EI_CLASS] == ELFCLASS64) {
        printf("ELF64\n");
        return ERROR_WRONG_WORD_SIZE;
    } else {
        printf("Taille invalide %d\n", header.e_ident[EI_CLASS]);
        return ERROR_WRONG_WORD_SIZE;
    }

    printf("  Endianness : ");
    if (header.e_ident[EI_DATA] == ELFDATA2LSB) {
        printf("Little endian\n");
    } else if (header.e_ident[EI_DATA] == ELFDATA2MSB) {
        printf("Big endian\n");
    } else {
        printf("Endianess invalide");
        return ERROR_WRONG_ENDIAN;
    }

    printf("  Version ELF : ");
    if (header.e_ident[EI_VERSION] >= EV_CURRENT) {
        printf("%d (current)\n", header.e_ident[EI_VERSION]);
    } else if (header.e_ident[EI_VERSION] == EV_NONE) {
        printf("%d\n", header.e_ident[EI_VERSION]);
        return ERROR_INVALID_VERSION;
    }

    printf("  OS/ABI : ");
    printf("%s\n", sys_table[header.e_ident[EI_OSABI]]);

    printf("  Type de fichier : ");
    if (header.e_type == ET_NONE) {
        printf("Aucun\n");
    } else if (header.e_type == ET_REL) {
        printf("Repositionnable (REL)\n");
    } else if (header.e_type == ET_EXEC) {
        printf("Exécutable (EXEC)\n");
    } else if (header.e_type == ET_DYN) {
        printf("Dyanmique (partagé)");
    } else if (header.e_type == ET_CORE) {
        printf("Fichier core (CORE)");
    }

    printf("  Machine cible : %s\n", sys_target[header.e_machine]);
    printf("  Version : 0x%x\n", header.e_version);
    printf("  Point d'entrée : 0x%x\n", header.e_entry);
    printf("  Flags : 0x%x\n", header.e_flags);
    printf("-- Décalages des tables\n");
    printf("  Décalage de la table d'en-tête du programme : 0x%x\n", header.e_phoff);
    printf("  Décalage de la table de sections : 0x%x\n", header.e_shoff);
    printf("-- Informations sur l'en-tête\n");
    printf("  Taille de l'en-tête ELF : %d octets\n", header.e_ehsize);
    printf("  Taille d'une entrée de l'en-tête du programme : %d octets\n", header.e_phentsize);
    printf("  Nombre d'entrées de l'en-tête du programme : %d\n", header.e_phnum);
    printf("-- Informations sur les sections\n");
    printf("  Taille d'une entrée de la table de sections : %d octets\n", header.e_shentsize);
    printf("  Nombre d'entrées de la table de sections : %d\n", header.e_shnum);

    printf("\n");

    return EXIT_SUCCESS;
}


void print_elf_section_header(Elf32_Ehdr header, Elf32_Shdr * table_entetes_section) {
    uint8_t i;
    for (i=0; i < header.e_shnum; i++) {
        printf("Nom : 0x%x\n", table_entetes_section[i].sh_name);
        printf("Type : 0x%x\n", table_entetes_section[i].sh_type);
        printf("Flag : 0x%x\n", table_entetes_section[i].sh_flags);
        printf("Adresse : 0x%x\n", table_entetes_section[i].sh_addr);
        printf("Decalage : 0x%x\n", table_entetes_section[i].sh_offset);
        printf("Taille : 0x%x\n", table_entetes_section[i].sh_size);
        printf("Lien : 0x%x\n", table_entetes_section[i].sh_link);
        printf("Alignement : 0x%x\n", table_entetes_section[i].sh_addralign);
        printf("Entsize : 0x%x\n", table_entetes_section[i].sh_entsize);
        printf("\n\n");
    }
}
