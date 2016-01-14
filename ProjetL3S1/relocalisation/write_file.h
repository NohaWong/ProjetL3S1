#ifndef WRITE_ELF_H
#define WRITE_ELF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>


void write_file_header (FILE *f, Elf32_Ehdr header);

void write_section_header (FILE *f, Elf32_Shdr *section_header_table,Elf32_Ehdr header);

void write_symbole_table (FILE *f, Elf32_Sym*symb_table,Elf32_Ehdr header,Elf32_Shdr *section_headers_table, uint32_t symbols_count );

void write_section_content(FILE *f,Elf32_Shdr *section_header_table, Elf32_Ehdr header, uint8_t **section_content);

#endif
