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
    printf("  Version : %#x\n", header.e_version);
    printf("  Point d'entrée : %#x\n", header.e_entry);
    printf("  Flags : %#x\n", header.e_flags);
    printf("-- Décalages des tables\n");
    printf("  Décalage de la table d'en-tête du programme : %#x\n", header.e_phoff);
    printf("  Décalage de la table de sections : %#x\n", header.e_shoff);
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


char* print_elf_section_header(FILE* file, Elf32_Ehdr header, Elf32_Shdr * table_entetes_section) {
    uint8_t i;
    Elf32_Addr addr_debut_nom_section = table_entetes_section[header.e_shstrndx].sh_addr + table_entetes_section[header.e_shstrndx].sh_offset;
    uint64_t pos_curs = ftell(file);
    char* c;

    // recuperation de la table des noms
    fseek(file, addr_debut_nom_section, SEEK_SET);
    c = malloc(sizeof(char) * table_entetes_section[header.e_shstrndx].sh_size);
    if (c == NULL) {
        return 0;
    }
    fread(c, sizeof(char), table_entetes_section[header.e_shstrndx].sh_size, file);

    printf("<SECTIONS DU FICHIER>\n");
    printf("#     Nom                 Type        Flags   Adresse              Taille  Lien    Alignement Entsize \n");
    printf("------------------------------------------------------------------------------------------------------\n");
    for (i=0; i < header.e_shnum; i++) {
        printf("%-6d%-20s%#-12x%#-8x%#-8x(+ %#-8x) %#-8x%#-8x%#-11x%#-8x", i,
                           &(c[table_entetes_section[i].sh_name]),
                           table_entetes_section[i].sh_type,
                           table_entetes_section[i].sh_flags,
                           table_entetes_section[i].sh_addr,
                           table_entetes_section[i].sh_offset,
                           table_entetes_section[i].sh_size,
                           table_entetes_section[i].sh_link,
                           table_entetes_section[i].sh_addralign,
                           table_entetes_section[i].sh_entsize
              );

        printf("\n");
    }
    printf("\n");

    fseek(file, pos_curs, SEEK_SET);
    return c;
}

void print_elf_symbol_table(Elf32_Sym *symbols, Elf32_Half shnum) {
    int i = 0;
    char type[16];
    char info[16];
    //char info[16];
    printf("<TABLE DES SYMBOLES>\n");
    printf("#      Nom    Valeur Type      Portée  \n");
    printf("---------------------------------------\n");
    for (i = 0; i < shnum; ++i) {
        switch (ELF32_ST_TYPE(symbols[i].st_info)) {
            case STT_NOTYPE:
            default:
                strcpy(type, "NOTYPE");
                break;
            case STT_OBJECT:
                strcpy(type, "OBJECT");
                break;
            case STT_FUNC:
                strcpy(type, "FUNC");
                break;
            case STT_SECTION:
                strcpy(type, "SECTION");
                break;
            case STT_FILE:
                strcpy(type, "FILE");
                break;
        }

        //bind
        switch(ELF32_ST_BIND(symbols[i].st_info))
        {
            case STB_LOCAL:
                strcpy(info, "LOCAL");
                break;
            case STB_GLOBAL:
                strcpy(info, "GLOBAL");
                break;
            case STB_WEAK:
                strcpy(info, "WEAK");
                break;
            case STB_NUM:
                strcpy(info, "NUM");
                break;
            default:
                strcpy(info, "OTHER");
                break;
        }

        printf("%-7d%#-7x%#-7x%-10s%-8s", i, symbols[i].st_name, symbols[i].st_value, type, info);
        printf("\n");
    }
}
