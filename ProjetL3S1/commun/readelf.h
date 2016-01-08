#ifndef READELF_H
#define READELF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

/**
 * Read an ELF file and store it in a struct.
 *
 * @param file      FILE*, the file to read, *already opened in "rb"*
 * @param header    Elf32_Ehdr*, the structure to stock header informations
 */
void read_elf_header(FILE *file, Elf32_Ehdr *header);

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
 * Reads a section content and returns it.
 *
 * @param file              FILE*, the file to read, *already opened in "rb"*
 * @param section_header    ELF32_Shdr*, all sections headers
 * @param header            ELF32_Ehdr*, the structure to stock header informations
 * @return A double pointer, with the content of a whole section inside
 */
uint8_t** read_section_content(FILE* file, Elf32_Shdr *section_headers, Elf32_Ehdr *header);

/**
 * Reads a sections' headers
 * @param file              FILE*, the file to read, *already opened in "rb"*
 * @param header            ELF32_Ehdr*, the structure to stock header informations
 * @param
 * @return A pointer to a structure that holds all sections' headers
 */
Elf32_Shdr *read_elf_section_header(FILE *file, Elf32_Ehdr *header, char** c);

/**
 * Reads the symbol table and returns it.
 *
 * @param file              FILE*, the file to read, *already opened in "rb"*
 * @param section_header    ELF32_Shdr*, all sections headers
 * @param symbols_count     uint16_t*, symbols count. The result is directly written in this variable
 * @return The structure that holds the symbol table
 */
Elf32_Sym *read_symbol_table(FILE *file, Elf32_Shdr *section_headers, uint16_t *symbols_count);

/**
 * @struct Table_rel_section
 * Holds a list of static relocations tables
 */
typedef struct
{
	Elf32_Word section_name;
	int elem_count;
    Elf32_Rel * rel_list;
} Table_rel_section  ;

/**
 * @struct Table_rela_section
 * Holds a list of dynamic relocations tables
 */
typedef struct
{
    Elf32_Word section_name;
    int elem_count;
    Elf32_Rela * rel_list;
} Table_rela_section;

/**
 * @struct Ensemble_table_rel
 * Holds the lists of static relocations and dynamic relocations
 */
typedef struct {
    int section_count_rel;
    int section_count_rela;
    Table_rel_section *rel_section_list;
    Table_rela_section *rela_section_list;
} Ensemble_table_rel;


/**
 * Reads the static relocation table
 *
 * @param file              FILE*, the file to read, *already opened in "rb"*
 * @param section_header    ELF32_Shdr*, all sections headers
 * @param shnum             Elf32_Half, number of sections
 * @return The structure (Ensemble_table_rel) that holds the static relocations table.
 */
Ensemble_table_rel read_rel_table(FILE *file, Elf32_Shdr *section_headers, Elf32_Half shnum);

/**
 * Convert a section name to its identifier
 *
 * @param name              char*, the name to convert
 * @param section_header    ELF32_Shdr*, all sections headers
 * @param names_table       char*, the table with all names
 * @param header            ELF32_Ehdr*, the structure to stock header informations
 * @return The structure (Ensemble_table_rel) that holds the static relocations table.
 */
int section_name_to_number (char* name, Elf32_Shdr * section_headers, char* names_table, Elf32_Ehdr *header);

/**
 * @todo documentation
 */
Elf32_Word rel_info_to_symbol (Elf32_Word info, Elf32_Sym *symb_table, Elf32_Shdr *section_headers);

#endif // READ_ELF_H
