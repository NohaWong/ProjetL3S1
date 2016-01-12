#ifndef READELF_H
#define READELF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>


/**
 * @enum
 * This enum contains all known errors that can happens during the program execution
 */
enum { ERROR_MAGIC_NUMBERS = 1, ERROR_MISSING_ARG, ERROR_WRONG_WORD_SIZE, ERROR_WRONG_ENDIAN,
       ERROR_INVALID_VERSION, ERROR_NO_FILE_SPECIFIED };

/**
 * Read an ELF file and store it in a struct.
 *
 * @param file      the file to read, *already opened in "rb"*
 * @param header    the structure to stock header informations
 * @return an integer that describes the error (see enum type)
 */
int read_elf_header(FILE *file, Elf32_Ehdr *header);

/**
 * handle the errors that may occur when reading an ELF file
 *
 * @param error_id  from the enum type - the error that is sent by read_elf_header
 */
void handle_errors (int error_id);

/**
 * Reads a section content and returns it.
 *
 * @param file              the file to read, *already opened in "rb"*
 * @param section_header    all sections headers
 * @param header            the structure to stock header informations
 * @return A double pointer, with the content of a whole section inside
 */
uint8_t** read_section_content(FILE* file, Elf32_Shdr *section_headers, Elf32_Ehdr *header);

/**
 * Reads a sections' headers
 * @param file              the file to read, *already opened in "rb"*
 * @param header            the structure to stock header informations
 * @param
 * @return A pointer to a structure that holds all sections' headers
 */
Elf32_Shdr *read_elf_section_header(FILE *file, Elf32_Ehdr *header, char** c);

/**
 * Reads the symbol table and returns it.
 *
 * @param file              the file to read, *already opened in "rb"*
 * @param section_header    all sections headers
 * @param symbols_count     symbols count. The result is directly written in this variable
 * @return The structure that holds the symbol table
 */
Elf32_Sym *read_symbol_table( Elf32_Ehdr header,FILE *file, Elf32_Shdr *section_headers, uint16_t *symbols_count);

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
 * @struct Table_rel_set
 * Holds the lists of static relocations and dynamic relocations
 */
typedef struct {
    int section_count_rel;
    int section_count_rela;
    Table_rel_section *rel_section_list;
    Table_rela_section *rela_section_list;
} Table_rel_set;


/**
 * Reads the static relocation table
 *
 * @param file              the file to read, *already opened in "rb"*
 * @param section_header    all sections headers
 * @param shnum             number of sections
 * @return The structure (Table_rel_set) that holds the static relocations table.
 */
Table_rel_set read_rel_table(FILE *file, Elf32_Shdr *section_headers, Elf32_Half shnum);

/**
 * Convert a section name to its identifier
 *
 * @param name              the name to convert
 * @param section_header    all sections headers
 * @param names_table       the table with all names
 * @param header            the structure to stock header informations
 * @return The structure (Table_rel_set) that holds the static relocations table.
 */
int section_name_to_number (char* name, Elf32_Shdr * section_headers, char* names_table, Elf32_Ehdr *header);

/**
 * @todo documentation
 */
Elf32_Word rel_info_to_symbol (Elf32_Word info, Elf32_Sym *symb_table, Elf32_Shdr *section_headers);

#endif // READ_ELF_H
