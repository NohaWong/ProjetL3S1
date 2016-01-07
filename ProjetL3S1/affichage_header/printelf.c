#include "printelf.h"

extern char sys_table[256][32];
extern char sys_target[193][32];

int print_elf_header(Elf32_Ehdr header)
{
    printf(BOLDWHITE "<EN-TÊTE ELF>\n" RESET);

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
        printf("Les fichiers 64-bits ne sont pas supportés.\n");
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
        printf("La version n'est pas valide : %d\n", header.e_ident[EI_VERSION]);
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
    } else {
        printf("Type inconnu");
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


void print_elf_section_header(Elf32_Ehdr header, Elf32_Shdr * table_entetes_section, char *secname) {
    uint8_t i;
    printf(BOLDWHITE "<SECTIONS DU FICHIER>\n" RESET);
    printf("#     Nom                 Type        Flags   Adresse              Taille  Lien    Alignement Entsize \n");
    printf("------------------------------------------------------------------------------------------------------\n");

    for (i=0; i < header.e_shnum; i++) {
        printf("%-6d%-20s%#-12x%#-8x%#-8x(+ %#-8x) %#-8x%#-8x%#-11x%#-8x", i,
                           &(secname[table_entetes_section[i].sh_name]),
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
}



void print_elf_symbol_table(Elf32_Sym *symbols, Elf32_Half shnum) {
    int i = 0;
    char type[16];
    char info[16];
    //char info[16];
    printf(BOLDWHITE "<TABLE DES SYMBOLES>\n" RESET);
    printf("#      Nom         Valeur      Type      Portée  \n");
    printf("-------------------------------------------------\n");
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

        printf("%-7d%#-12x%#-12x%-10s%-8s", i, symbols[i].st_name, symbols[i].st_value, type, info);
        printf("\n");
    }
    printf("\n");
}



void print_elf_section_content(uint8_t** secContent, int number, Elf32_Shdr *section_headers, char *secname) {
    printf(BOLDWHITE "<CONTENU DE LA SECTION %s>" RESET, secname);

    // print the address of the current line
    printf("\n[0x%08x]\t", 0x0);

    uint32_t i, j;
    for (i = 0; i < (section_headers[number].sh_size); i++) {
        if (!(i%4) && i != 0) {
            printf(" ");
        }
        if ((!(i%16) && i != 0)) {
            printf("\t| ");
            for (j = i - 16; j < i; ++j) {
                // if character is a blank character, print a '.'
                if (secContent[number][j] < 0x20) {
                    printf(".");
                } else {
                    printf("%c", secContent[number][j]);
                }
            }
            printf("\n[0x%08x]\t", i);
        }

        printf("%02x", secContent[number][i]);
    }

    for (j = i; j%16 != 0; ++j) {
        printf("  ");
        if (j%4 == 0) {
            printf(" ");
        }
    }

    printf("\t| ");

    uint32_t initial_printfor = 0, condition_printfort = 0;

    if ((section_headers[number].sh_size%16) != 0) {
        initial_printfor = j - 16;
        condition_printfort = (j - 16) + section_headers[number].sh_size%16;
    } else {
        // case when there is only one line of 16 bytes
        initial_printfor = i - 16;
        condition_printfort = i;
    }

    for (i = initial_printfor; i < condition_printfort; ++i) {
        // if character is a blank character, print a '.'
        if (secContent[number][i] < 0x20) {
            printf(".");
        } else {
            printf("%c", secContent[number][i]);
        }
    }

    printf("\n");
}


void print_elf_rel_tab(TableRel *tab, Elf32_Shdr * table_entetes_section, char *secname){
    printf(BOLDWHITE "<TABLE DE RÉIMPLANTATION STATIQUE>\n" RESET);

    if (tab->nb_elem == 0) {
        printf("Aucun élément.\n");
        return;
    }

    printf("Décalage    Information   Type      Valeur      Nom symbole     \n");
    printf("----------------------------------------------------------------\n");
    int i = 0;
    for(i = 0; i < tab->nb_elem;  i++){
        printf("%#-12x%#-14x%#-10x%#-12x%-13s\n",tab->tab[i].r_offset,
                                            tab->tab[i].r_info,
                                            ELF32_R_TYPE(tab->tab[i].r_info),
                                            ELF32_R_SYM(tab->tab[i].r_info) == STN_UNDEF ?
                                                ELF32_R_SYM(tab->tab[i].r_info) : 0,
                                            &(secname[table_entetes_section[ELF32_M_SYM(tab->tab[i].r_info)].sh_name]
                                        ));
    }

    printf("\n");
}


void print_elf_rela_tab(TableRela *tab){
    printf(BOLDWHITE "<TABLE DE RÉIMPLANTATION DYNAMIQUE>\n" RESET);

    if (tab->nb_elem == 0) {
        printf("Aucun élément.\n");
        return;
    }

    printf("#       Décalage    Information   Fin addresse    \n");
    printf("--------------------------------------------------\n");
    int i = 0;
    for(i = 0; i < tab->nb_elem;  i++){
        printf("%-8d%#-12x%#-14x%#-16x\n", i, tab->tab[i].r_offset, tab->tab[i].r_info, tab->tab[i].r_addend);
    }

    printf("\n");
}
