#ifndef PRINTELF_H
#define PRINTELF_H

#include "../commun/readelf.h"

/**
 * Init the systems table. Used to print human-readable informations
 * when printing the header.
 */
void init_systable();

/**
 * Init the targets table. Used to print human-readable informations
 * when printing the header.
 */
void init_systarget();

/**
 * Prints the header of ELF file given to the program.
 *
 * @param elf_header   all informations about ELF file header
 */
void print_elf_header(Elf32_Ehdr elf_header);

/**
 * Prints symbol table of an ELF file
 *
 * @param symbols   symbols array
 */
void print_elf_symbol_table(Elf32_Sym *symbols, uint16_t symbols_count);

/**
 * Prints one section header of an ELF file
 *
 * @param symbols               symbols array
 * @param section_header_table  the table of sections headers
 * @param secname               name of the wanted section
 */
void print_elf_section_header(Elf32_Ehdr header, Elf32_Shdr *section_header_table, char *secname);

/**
 * Prints static relocation table
 *
 * @param relocations           all relocations
 * @param symb_table            the table of all symbols
 * @param secname               name of all sections
 * @param elf                   all informations about ELF file header
 */
void print_elf_rel_tab(Table_rel_set relocations, Elf32_Sym* symb_table, Elf32_Shdr * section_headers, char *secname, Elf32_Ehdr header);

/**
 * Prints a entire section content, with addresses and ASCII
 * equivalent of the hexadecimal dump.
 *
 * @param sec_content           content of all sections
 * @param number                identifier of a section
 * @param symbols               symbols array
 * @param section_header        the table of sections headers
 * @param secname               name of the wanted section
 * @param elf_header            all informations about ELF file header
 */
void print_elf_section_content(uint8_t** sec_content, int number, Elf32_Shdr *section_headers, char *secname, Elf32_Ehdr elf_header);


#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */
#define RESET       "\033[0m"


#endif // PRINT_ELF
