#include "readelf.h"
#include <endian.h>

// -------------- lecture header fichier -------------- //

char sys_table[256][32];
// 193 targets, according to elf.h
char sys_target[193][32];

/**
 * Read an ELF file and store it in a struct.
 *
 * @param file      FILE*, the file to read, *already opened in "rb"*
 * @param header    ELF32_Ehdr*, the structure to stock header informations
 * @return an integer that describes the error (see enum type)
 */
int read_elf_header(FILE *file, Elf32_Ehdr *header) {
    fseek(file, 0, SEEK_SET);
    fread(header, sizeof(Elf32_Ehdr), 1, file);

    if (header->e_ident[EI_DATA] == ELFDATA2MSB) {
        // swap endianness, file is encoded in little endian
        header->e_machine = be16toh(header->e_machine);
        header->e_version = be32toh(header->e_version);
        header->e_entry = be32toh(header->e_entry);
        header->e_phoff = be32toh(header->e_phoff);
        header->e_shoff = be32toh(header->e_shoff);
        header->e_flags  = be32toh(header->e_flags);
        header->e_ehsize = be16toh(header->e_ehsize);
        header->e_phentsize = be16toh(header->e_phentsize);
        header->e_phnum = be16toh(header->e_phnum);
        header->e_shentsize = be16toh(header->e_shentsize);
        header->e_shnum = be16toh(header->e_shnum);
        header->e_shstrndx = be16toh(header->e_shstrndx);
    }

    if (    header->e_ident[EI_MAG0] != ELFMAG0
        ||  header->e_ident[EI_MAG1] != ELFMAG1
        ||  header->e_ident[EI_MAG2] != ELFMAG2
        ||  header->e_ident[EI_MAG3] != ELFMAG3) {
        return ERROR_MAGIC_NUMBERS;
    }

    if (header->e_ident[EI_CLASS] != ELFCLASS32) {
        return ERROR_WRONG_WORD_SIZE;
    }

    if (header->e_ident[EI_DATA] != ELFDATA2LSB && header->e_ident[EI_DATA] != ELFDATA2MSB) {
        return ERROR_WRONG_ENDIAN;
    }

    if (header->e_ident[EI_VERSION] == EV_NONE) {
        return ERROR_INVALID_VERSION;
    }
    return EXIT_SUCCESS;
}


void handle_errors (int error_id) {
    if (error_id) {
        printf("Le programme s'est terminé avec l'erreur : ");
        switch (error_id) {
            case ERROR_MAGIC_NUMBERS:
                printf("mauvais nombres magiques.");
            break;

            case ERROR_WRONG_ENDIAN:
                printf("endian corrompu.");
            break;

            case ERROR_WRONG_WORD_SIZE:
                printf("mauvaises tailles de mots (64 non supporté).");
            break;

            case ERROR_INVALID_VERSION:
                printf("version invalide.");
            break;

            default:
                printf("inconnue");
            break;
        }
        printf("\n");
        exit(error_id);
    }
}


/**
 * Reads a sections' headers
 * @param file              FILE*, the file to read, *already opened in "rb"*
 * @param header            ELF32_Ehdr*, the structure to stock header informations
 * @param
 * @return A pointer to a structure that holds all sections' headers
 */
Elf32_Shdr *read_elf_section_header(FILE *file, Elf32_Ehdr *header, char **c) {
    uint32_t i;
    Elf32_Shdr *table_entetes_section;

    table_entetes_section = (Elf32_Shdr*) malloc(sizeof(Elf32_Shdr) * (header->e_shnum));
    fseek(file, header->e_shoff, SEEK_SET);

    fread(table_entetes_section, sizeof(Elf32_Shdr), header->e_shnum, file);

    if (header->e_ident[EI_DATA] == ELFDATA2MSB) {
        for (i = 0; i < header->e_shnum; i++) {
            // swap endiannes
            table_entetes_section[i].sh_name = be32toh(table_entetes_section[i].sh_name);
            table_entetes_section[i].sh_type = be32toh(table_entetes_section[i].sh_type);
            table_entetes_section[i].sh_flags = be32toh(table_entetes_section[i].sh_flags);
            table_entetes_section[i].sh_addr = be32toh(table_entetes_section[i].sh_addr);
            table_entetes_section[i].sh_offset = be32toh(table_entetes_section[i].sh_offset);
            table_entetes_section[i].sh_size = be32toh(table_entetes_section[i].sh_size);
            table_entetes_section[i].sh_link = be32toh(table_entetes_section[i].sh_link);
            table_entetes_section[i].sh_info = be32toh(table_entetes_section[i].sh_info);
            table_entetes_section[i].sh_addralign = be32toh(table_entetes_section[i].sh_addralign);
            table_entetes_section[i].sh_entsize = be32toh(table_entetes_section[i].sh_entsize);
        }
    }
    // Get name's table
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

/**
 * Reads the symbol table and returns it.
 *
 * @param file              FILE*, the file to read, *already opened in "rb"*
 * @param section_header    ELF32_Shdr*, all sections headers
 * @param symbols_count     uint16_t*, symbols count. The result is directly written in this variable
 * @return The structure that holds the symbol table
 */
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

    // swap endiannes
    for (i = 0; i < *symbols_count; ++i) {
        symbols[i].st_name = be32toh(symbols[i].st_name);
        symbols[i].st_value = be32toh(symbols[i].st_value);
        symbols[i].st_shndx = be16toh(symbols[i].st_shndx);
        symbols[i].st_size = be32toh(symbols[i].st_size);
    }

    return symbols;
}

/**
 * Reads the static relocation table
 *
 * @param file              FILE*, the file to read, *already opened in "rb"*
 * @param section_header    ELF32_Shdr*, all sections headers
 * @param shnum             Elf32_Half, number of sections
 * @return The structure (Ensemble_table_rel) that holds the static relocations table.
 */
Ensemble_table_rel read_rel_table(FILE *file, Elf32_Shdr *section_headers, Elf32_Half shnum){

    int i=0;
    Ensemble_table_rel relocations;

    relocations.section_count_rel=0;
    relocations.section_count_rela=0;

    for (i=0; i< shnum;i++){
        if (section_headers[i].sh_type == SHT_REL ) {
            relocations.section_count_rel++;
        } else if (section_headers[i].sh_type == SHT_RELA){
            relocations.section_count_rela++;
        }
    }

    relocations.rel_section_list=malloc(sizeof(Table_rel_section)*relocations.section_count_rel);
    relocations.rela_section_list=malloc(sizeof(Table_rela_section)*relocations.section_count_rela);

    int k=0;//nb de section Rel deja traité.
    int l=0;//nb de section Rela deja traité
    for (i=0; i< shnum;i++){ //on parcourt toute les sections

        if (section_headers[i].sh_type == SHT_REL) { // found a rel section

            int j=0;

            // set its name
            relocations.rel_section_list[k].section_name = section_headers[i].sh_name;

            // set the number of relocation inside it
            relocations.rel_section_list[k].elem_count = section_headers[i].sh_size/sizeof(Elf32_Rel);

            relocations.rel_section_list[k].rel_list = malloc(sizeof(Elf32_Rel)*relocations.rel_section_list[k].elem_count);

            // move to where we need to read in the file
            fseek(file, section_headers[i].sh_offset, SEEK_SET);
            // start reading
            for(j=0;j<relocations.rel_section_list[k].elem_count;j++){ //Read every elem of the section
                    fread(&relocations.rel_section_list[k].rel_list[j], sizeof(Elf32_Rel),1, file);
                    relocations.rel_section_list[k].rel_list[j].r_info = be32toh(relocations.rel_section_list[k].rel_list[j].r_info);
                    relocations.rel_section_list[k].rel_list[j].r_offset = be32toh(relocations.rel_section_list[k].rel_list[j].r_offset);
            }
            k++;
        }else if(section_headers[i].sh_type == SHT_RELA){
            int j=0;

            // set its name
            relocations.rela_section_list[l].section_name = section_headers[i].sh_name;

            // set the number of relocation inside it
            relocations.rela_section_list[l].elem_count = section_headers[i].sh_size/sizeof(Elf32_Rela);

            relocations.rela_section_list[l].rel_list = malloc(sizeof(Elf32_Rela) * relocations.rela_section_list[l].elem_count);

            // move to where we need to read in the file
            fseek(file, section_headers[i].sh_offset, SEEK_SET);
            // start reading
            for(j=0; j<relocations.rela_section_list[l].elem_count; j++){ // read its elements
                    fread(&relocations.rela_section_list[l].rel_list[j], sizeof(Elf32_Rela),1, file);
                    relocations.rela_section_list[l].rel_list[j].r_info = be32toh(relocations.rela_section_list[l].rel_list[j].r_info);
                    relocations.rela_section_list[l].rel_list[j].r_offset = be32toh(relocations.rela_section_list[l].rel_list[j].r_offset);
                    relocations.rela_section_list[l].rel_list[j].r_addend = be32toh(relocations.rela_section_list[l].rel_list[j].r_addend);
            }
            l++;
        }
    }
    return relocations;
}

/**
 * Convert a section name to its identifier
 *
 * @param name              char*, the name to convert
 * @param section_header    ELF32_Shdr*, all sections headers
 * @param names_table       char*, the table with all names
 * @param header            ELF32_Ehdr*, the structure to stock header informations
 * @return The structure (Ensemble_table_rel) that holds the static relocations table.
 */
int section_name_to_number (char* name, Elf32_Shdr * section_headers, char* names_table, Elf32_Ehdr *header) {
    int i;
    for (i=0; i < header->e_shnum ; i++) {
        if (!(strcmp(name, &names_table[section_headers[i].sh_name]))) {
            return i;
        }
    }
    return -1;

}

/**
 * @todo documentation
 */
Elf32_Word rel_info_to_symbol (Elf32_Word info, Elf32_Sym * symb_table, Elf32_Shdr* section_headers) {
    return section_headers[symb_table[(!(ELF32_R_SYM(info)==STN_UNDEF)) * ELF32_R_SYM(info)].st_shndx].sh_name;
}

/**
 * Reads a section content and returns it.
 *
 * @param file              FILE*, the file to read, *already opened in "rb"*
 * @param section_header    ELF32_Shdr*, all sections headers
 * @param header            ELF32_Ehdr*, the structure to stock header informations
 * @return A double pointer array, with the content of a whole section inside
 */
uint8_t **read_section_content(FILE* file, Elf32_Shdr *section_headers, Elf32_Ehdr *header) {

    Elf32_Half sectionNumber = header->e_shnum;
    uint8_t i;
    uint8_t **res = malloc(sizeof(uint8_t*) * sectionNumber);

    uint32_t pos_curs = ftell(file);

    for (i=0;i<sectionNumber;i++) {

        // Is empty ?
        if (section_headers[i].sh_size==0 ||
            section_headers[i].sh_type==SHT_NOBITS ||
            section_headers[i].sh_type==SHT_NULL) {
            res[i] = NULL;
        }
        else { // Not empty
            res[i] = malloc(sizeof(char) * section_headers[i].sh_size);
            if (res[i] == NULL) {
                return NULL;
            }
            // Change position of write pointer.
            fseek(file, section_headers[i].sh_offset, section_headers[i].sh_addr);
            fread(res[i], sizeof(char), section_headers[i].sh_size, file);
        }
    }
    fseek(file, 0, pos_curs);

    return res;
}

