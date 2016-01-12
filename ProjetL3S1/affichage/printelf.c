#include "printelf.h"

extern char sys_table[256][32];
extern char sys_target[193][32];

/**
 * Init the systems table. Used to print human-readable informations
 * when printing the header.
 */
void init_systable() {
    strcpy(sys_table[ELFOSABI_SYSV], "UNIX System V");
    strcpy(sys_table[ELFOSABI_HPUX], "HP-UX");
    strcpy(sys_table[ELFOSABI_NETBSD], "NetBSD");
    strcpy(sys_table[ELFOSABI_LINUX], "Linux");
    strcpy(sys_table[ELFOSABI_SOLARIS], "Sun Solaris");
    strcpy(sys_table[ELFOSABI_AIX], "IBM AIX");
    strcpy(sys_table[ELFOSABI_IRIX], "SGI Irix");
    strcpy(sys_table[ELFOSABI_FREEBSD], "FreeBSD");
    strcpy(sys_table[ELFOSABI_TRU64], "Compaq TRU64");
    strcpy(sys_table[ELFOSABI_MODESTO], "Novell Modesto");
    strcpy(sys_table[ELFOSABI_OPENBSD], "OpenBSD");
    strcpy(sys_table[ELFOSABI_ARM_AEABI], "ARM EABI");
    strcpy(sys_table[ELFOSABI_ARM], "ARM");
    strcpy(sys_table[ELFOSABI_STANDALONE], "Standalone");
}

/**
 * Init the targets table. Used to print human-readable informations
 * when printing the header.
 */
void init_systarget() {
    strcpy(sys_target[EM_NONE], "Aucune");
    strcpy(sys_target[EM_SPARC], "SPARC");
    strcpy(sys_target[EM_386], "Intel 80386");
    strcpy(sys_target[EM_68K], "Motorola 68000");
    strcpy(sys_target[EM_860], "Intel i860");
    strcpy(sys_target[EM_MIPS], "MIPS I");
    strcpy(sys_target[EM_960], "Intel i960");
    strcpy(sys_target[EM_PPC], "PowerPC");
    strcpy(sys_target[EM_ARM], "ARM");
    strcpy(sys_target[EM_IA_64], "Intel IA64");
    strcpy(sys_target[EM_X86_64], "x64");
}

/**
 * Prints the header of ELF file given to the program.
 *
 * @param elf   Elf32_Ehdr, all informations about ELF file header
 */
void print_elf_header(Elf32_Ehdr elf_header) {
    printf(BOLDWHITE "<EN-TÊTE ELF>\n" RESET);

    int i = 0;
    printf("  Nombre magique : ");
    for (i = 0; i < 4; ++i) {
        printf("%x ", elf_header.e_ident[i]);
    }
    printf("\n");

    printf("  Taille d'un mot : ");
    if (elf_header.e_ident[EI_CLASS] == ELFCLASS32) {
        printf("ELF32\n");
    } else if (elf_header.e_ident[EI_CLASS] == ELFCLASS64) {
        printf("ELF64\n");
        printf("Les fichiers 64-bits ne sont pas supportés.\n");
    } else {
        printf("Mauvaise taille : %d\n", elf_header.e_ident[EI_CLASS]);
    }

    printf("  Endianness : ");
    if (elf_header.e_ident[EI_DATA] == ELFDATA2LSB) {
        printf("Little endian\n");
    } else if (elf_header.e_ident[EI_DATA] == ELFDATA2MSB) {
        printf("Big endian\n");
    } else {
        printf("Endianess invalide");
    }

    printf("  ELF Version : ");
    if (elf_header.e_ident[EI_VERSION] >= EV_CURRENT) {
        printf("%d (current)\n", elf_header.e_ident[EI_VERSION]);
    } else if (elf_header.e_ident[EI_VERSION] == EV_NONE) {
        printf("Version invalide : %d\n", elf_header.e_ident[EI_VERSION]);
    }

    printf("  OS/ABI : ");
    printf("%s\n", sys_table[elf_header.e_ident[EI_OSABI]]);

    printf("  Type de fichier : ");
    if (elf_header.e_type == ET_NONE) {
        printf("Aucun\n");
    } else if (elf_header.e_type == ET_REL) {
        printf("Relocalizable (REL)\n");
    } else if (elf_header.e_type == ET_EXEC) {
        printf("Executable (EXEC)\n");
    } else if (elf_header.e_type == ET_DYN) {
        printf("Dynamic (partagé)\n");
    } else if (elf_header.e_type == ET_CORE) {
        printf("Core file (CORE)\n");
    } else {
        printf("Type inconnu\n");
    }

    printf("  Machine cible : %s\n", sys_target[elf_header.e_machine]);
    printf("  Version : %#x\n", elf_header.e_version);
    printf("  Point d'entrée : %#x\n", elf_header.e_entry);
    printf("  Flags : %#x\n", elf_header.e_flags);
    printf("-- Décalage des tables\n");
    printf("  Décalage de la table des en-têtes : %#x\n", elf_header.e_phoff);
    printf("  Décalage de la table des sections : %#x\n", elf_header.e_shoff);
    printf("-- Informations de l'en-tête\n");
    printf("  Taille de l'en-tête ELF : %d octets\n", elf_header.e_ehsize);
    printf("  Taille d'une entrée de la table des en-têtes : %d octets\n", elf_header.e_phentsize);
    printf("  Nombre d'entrées dans l'en-tête : %d\n", elf_header.e_phnum);
    printf("-- Informations de sections\n");
    printf("  Taille d'une entrée de la table des sections: %d octets\n", elf_header.e_shentsize);
    printf("  Nombre d'entrées dans la table des sections : %d\n", elf_header.e_shnum);

    printf("\n");
}

/**
 * Prints one section header of an ELF file
 *
 * @param symbols               Elf32_Sym*, symbols array
 * @param section_header_table  Elf32_Shdr*, the table of sections headers
 * @param secname               char*, name of the wanted section
 */
void print_elf_section_header(Elf32_Ehdr header, Elf32_Shdr *section_header_table, char *secname) {
    uint8_t i;
    printf(BOLDWHITE "<TABLE DES SECTIONS>\n" RESET);
    printf("#     Nom                 Type        Flags   Adresse              Taille  Link    Alignement Entsize \n");
    printf("------------------------------------------------------------------------------------------------------\n");

    for (i=0; i < header.e_shnum; i++) {
        printf("%-6d%-20s%#-12x%#-8x%#-8x(+ %#-8x) %#-8x%#-8x%#-11x%#-8x", i,
                           &(secname[section_header_table[i].sh_name]),
                           section_header_table[i].sh_type,
                           section_header_table[i].sh_flags,
                           section_header_table[i].sh_addr,
                           section_header_table[i].sh_offset,
                           section_header_table[i].sh_size,
                           section_header_table[i].sh_link,
                           section_header_table[i].sh_addralign,
                           section_header_table[i].sh_entsize
              );

        printf("\n");
    }

    printf("\n");
}

/**
 * Prints symbol table of an ELF file
 *
 * @param symbols   Elf32_Sym*, symbols array
 */
void print_elf_symbol_table(Elf32_Sym *symbols, uint16_t symbols_count) {
    int i = 0;
    char type[16];
    char info[16];
    //char info[16];
    printf(BOLDWHITE "<TABLE DES SYMBOLES>\n" RESET);
    printf("#      Nom         Valeur      Type      Portée    Indice de section\n");
    printf("--------------------------------------------------------------------\n");
    for (i = 0; i < symbols_count; ++i) {
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
        switch(ELF32_ST_BIND(symbols[i].st_info)) {
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
        char idx[15];
        (symbols[i].st_shndx == SHN_ABS) ? sprintf(idx, "ABS") : sprintf(idx, "%d", symbols[i].st_shndx);
        printf("%-7d%#-12x%#-12x%-10s%-10s%-5s", i, symbols[i].st_name, symbols[i].st_value, type, info, idx);
        printf("\n");
    }
    printf("\n");
}


void print_elf_section_content(uint8_t** sec_content, int number, Elf32_Shdr *section_headers, char *secname, Elf32_Ehdr elf_header) {
    printf(BOLDWHITE "<CONTENU DE LA SECTION %s>\n" RESET, secname);

    // prevent the case where a user enter a identifier grater than the section table size
    if (elf_header.e_shnum <= number || number < 0) {
        printf("La section n'existe pas.\n");
        return;
    }

    // print the address of the current line
    printf("[0x%08x]\t", 0x0);

    uint32_t i, j;
    for (i = 0; i < (section_headers[number].sh_size); i++) {
        if (!(i%4) && i != 0) {
            printf(" ");
        }
        if ((!(i%16) && i != 0)) {
            printf("\t| ");
            for (j = i - 16; j < i; ++j) {
                // if character is a blank character, print a '.'
                if (sec_content[number][j] < 0x20) {
                    printf(".");
                } else {
                    printf("%c", sec_content[number][j]);
                }
            }
            printf("\n[0x%08x]\t", i);
        }
        printf("%02x", sec_content[number][i]);
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
        if (sec_content[number][i] < 0x20) {
            printf(".");
        } else {
            printf("%c", sec_content[number][i]);
        }
    }

    printf("\n\n");
}

/**
 * Prints static relocation table
 *
 * @param relocations           Ensemble_table_rel, all relocations
 * @param symb_table            Elf32_Sym*, the table of all symbols
 * @param secname               char*, name of all sections
 * @param elf                   Elf32_Ehdr, all informations about ELF file header
 */
void print_elf_rel_tab(Ensemble_table_rel relocations, Elf32_Sym* symb_table, Elf32_Shdr * section_headers, char *secname, Elf32_Ehdr header){
    printf(BOLDWHITE "<TABLE DE RELOCATION STATIQUE>\n" RESET);

    if (relocations.section_count_rel == 0) {
        printf("Aucune entrée.\n");
        return;
    }

    int i = 0;
    for(i = 0; i < relocations.section_count_rel; i++) {
        int j;
        printf("Table de relocations de la section %s à l'adresse de décalage %#x contient %d relocations\n",
                    &secname[relocations.rel_section_list[i].section_name],
                    section_headers[section_name_to_number(&secname[relocations.rel_section_list[i].section_name], section_headers, secname, &header)].sh_offset,
                    relocations.rel_section_list[i].elem_count
                );
        printf("Décalage    Informations   Type      Valeur      Nom du symbole     \n");
        printf("--------------------------------------------------------------------\n");
        for (j=0; j<relocations.rel_section_list[i].elem_count; j++) {
            printf("%#-12x%#-15x%#-10x%#-12x%-19s\n",
                    relocations.rel_section_list[i].rel_list[j].r_offset,
                    relocations.rel_section_list[i].rel_list[j].r_info,
                    ELF32_R_TYPE(relocations.rel_section_list[i].rel_list[j].r_info),
                    relocations.rel_section_list[i].rel_list[j].r_info,
                    &secname[rel_info_to_symbol(relocations.rel_section_list[i].rel_list[j].r_info, symb_table, section_headers)]
                );
        }
        printf("\n");
    }
}

