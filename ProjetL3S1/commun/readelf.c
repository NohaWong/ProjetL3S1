#include "readelf.h"
#include <endian.h>

// -------------- lecture header fichier -------------- //

char sys_table[256][32];
// 193 targets, according to elf.h
char sys_target[193][32];

void read_elf_header(FILE *file, Elf32_Ehdr *header) {
    fseek(file, 0, SEEK_SET);
    fread(header, sizeof(Elf32_Ehdr), 1, file);

    if (header->e_ident[EI_DATA] == ELFDATA2MSB) {
        // swap endianness, file is encoded in little endian
        header->e_machine = htobe16(header->e_machine);
        header->e_version = htobe32(header->e_version);
        header->e_entry = htobe32(header->e_entry);
        header->e_phoff = htobe32(header->e_phoff);
        header->e_shoff = htobe32(header->e_shoff);
        header->e_flags  = htobe32(header->e_flags);
        header->e_ehsize = htobe16(header->e_ehsize);
        header->e_phentsize = htobe16(header->e_phentsize);
        header->e_phnum = htobe16(header->e_phnum);
        header->e_shentsize = htobe16(header->e_shentsize);
        header->e_shnum = htobe16(header->e_shnum);
        header->e_shstrndx = htobe16(header->e_shstrndx);
    }
}

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

// -------------- lecture section header -------------- //

Elf32_Shdr *read_elf_section_header(FILE *file, Elf32_Ehdr *header, char **c) {
    uint32_t i;
    Elf32_Shdr *table_entetes_section;

    table_entetes_section = (Elf32_Shdr*) malloc(sizeof(Elf32_Shdr) * (header->e_shnum));
    fseek(file, header->e_shoff, SEEK_SET);

    fread(table_entetes_section, sizeof(Elf32_Shdr), header->e_shnum, file);

    if (header->e_ident[EI_DATA] == ELFDATA2MSB) {
        for (i = 0; i < header->e_shnum; i++) {
            // changer en little endian
            table_entetes_section[i].sh_name = htobe32(table_entetes_section[i].sh_name);
            table_entetes_section[i].sh_type = htobe32(table_entetes_section[i].sh_type);
            table_entetes_section[i].sh_flags = htobe32(table_entetes_section[i].sh_flags);
            table_entetes_section[i].sh_addr = htobe32(table_entetes_section[i].sh_addr);
            table_entetes_section[i].sh_offset = htobe32(table_entetes_section[i].sh_offset);
            table_entetes_section[i].sh_size = htobe32(table_entetes_section[i].sh_size);
            table_entetes_section[i].sh_link = htobe32(table_entetes_section[i].sh_link);
            table_entetes_section[i].sh_info = htobe32(table_entetes_section[i].sh_info);
            table_entetes_section[i].sh_addralign = htobe32(table_entetes_section[i].sh_addralign);
            table_entetes_section[i].sh_entsize = htobe32(table_entetes_section[i].sh_entsize);
        }
    }
    // recuperation de la table des noms
    *c = malloc(sizeof(char) * table_entetes_section[header->e_shstrndx].sh_size);
    if (c == NULL) {
        return 0;
    }
    i = ftell(file);
    fseek(file, table_entetes_section[header->e_shstrndx].sh_offset, SEEK_SET);
    fread(*c, sizeof(char), table_entetes_section[header->e_shstrndx].sh_size, file);
    fseek(file, 0, i);

    return table_entetes_section;
}

// -------------- lecture section content -------------- //

Elf32_Sym *read_symbol_table(FILE *file, Elf32_Shdr *section_headers, uint16_t *symbols_count) {
    Elf32_Half symtable_index = 0;
    int i = 0;

    // get section index of the symbole table
    while (section_headers[symtable_index].sh_type != SHT_SYMTAB) {
        symtable_index++;
    }

    *symbols_count = section_headers[symtable_index].sh_size / sizeof(Elf32_Sym);

    Elf32_Sym *symbols = malloc(sizeof(Elf32_Sym) * *symbols_count);
    fseek(file, section_headers[symtable_index].sh_offset, SEEK_SET);
    fread(symbols, sizeof(Elf32_Sym), *symbols_count, file);

    for (i = 0; i < *symbols_count; ++i) {
        symbols[i].st_name = htobe32(symbols[i].st_name);
        symbols[i].st_value = htobe32(symbols[i].st_value);
        symbols[i].st_shndx = htobe16(symbols[i].st_shndx);
        symbols[i].st_size = htobe32(symbols[i].st_size);
    }

    return symbols;
}

Ensemble_table_rel read_rel_table(FILE *file, Elf32_Shdr *section_headers, Elf32_Half shnum){

    int i=0;
    Ensemble_table_rel relocations;

    relocations.section_count_rel=0;
    relocations.section_count_rela=0;

    for (i=0; i< shnum;i++){
        if (section_headers[i].sh_type == SHT_REL ) {
            //compteur += section_headers[i].sh_size/sizeof(Elf32_Rel);
            relocations.section_count_rel++;
        }else if (section_headers[i].sh_type == SHT_RELA){
            relocations.section_count_rela++;
        }
    }

    relocations.rel_section_list=malloc(sizeof(Table_rel_section)*relocations.section_count_rel);
    relocations.rela_section_list=malloc(sizeof(Table_rela_section)*relocations.section_count_rela);

    int k=0;//nb de section Rel deja traité.
    int l=0;//nb de section Rela deja traité
    for (i=0; i< shnum;i++){ //on parcour toute les sections

        if (section_headers[i].sh_type == SHT_REL) { // la section regardé est une relocation
            int j=0;
            fseek(file, section_headers[i].sh_offset, SEEK_SET);
            int section_relocation_count_rel = section_headers[i].sh_size/sizeof(Elf32_Rel);
            relocations.rel_section_list[k].rel_list = malloc(sizeof(Elf32_Rel)*section_relocation_count_rel);
            relocations.rel_section_list[k].section_name = section_headers[i].sh_name;
            for(j=0;j<section_relocation_count_rel;j++){ //on lit tout les elements de la section i
                    fread(&relocations.rel_section_list[k].rel_list[j], sizeof(Elf32_Rel),1, file);
            }
            k++;
        }else if(section_headers[i].sh_type == SHT_RELA){
            int j=0;
            fseek(file, section_headers[i].sh_offset, SEEK_SET);
            int section_relocation_count_rela = section_headers[i].sh_size/sizeof(Elf32_Rela);
            relocations.rela_section_list[l].rel_list = malloc(sizeof(Elf32_Rela)*section_relocation_count_rela);
            relocations.rela_section_list[l].section_name = section_headers[i].sh_name;

            for(j=0;j<section_relocation_count_rela;j++){ //on lit tout les elements de la section i
                    fread(&relocations.rela_section_list[l].rel_list[j], sizeof(Elf32_Rela),1, file);
            }
            l++;
        }
    }
    return relocations;
}


int section_name_to_number (char* nom, Elf32_Shdr * section_headers, char* table_noms, Elf32_Ehdr *header) {
    int i;
    for (i=0; i < header->e_shnum ; i++) {
        if (!(strcmp(nom, &table_noms[section_headers[i].sh_name]))) {
            return i;
        }
    }
    return -1;

}


Elf32_Word relInfo_to_symbole (Elf32_Word info) {
    Elf32_Word tempo = ELF32_R_SYM(info);
    return (!(tempo==STN_UNDEF)) * tempo;
}

uint8_t ** read_section_content(FILE* file, Elf32_Shdr *section_headers, Elf32_Ehdr *header) {

    Elf32_Half nbSections = header->e_shnum;
    uint8_t i;
    uint8_t **resultat = malloc(sizeof(uint8_t*) * nbSections);

    uint32_t pos_curs = ftell(file);

    for (i=0;i<nbSections;i++) {

        // verification que la section contient bien quelque chose
        if (section_headers[i].sh_size==0 ||
            section_headers[i].sh_type==SHT_NOBITS ||
            section_headers[i].sh_type==SHT_NULL) { // ne contient rien
            resultat[i] = NULL;
        }
        else { // contient qqc
            resultat[i] = malloc(sizeof(char) * section_headers[i].sh_size);
            if (resultat[i] == NULL) {
                return NULL;
            }
            fseek(file, section_headers[i].sh_offset, section_headers[i].sh_addr);
            fread(resultat[i], sizeof(char), section_headers[i].sh_size, file);
        }
    }
    fseek(file, 0, pos_curs);

    return resultat;
}

