#ifndef WRITE_ELF_H
#define WRITE_ELF_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>


void write_file_header (FILE *f, Elf32_Ehdr *header);

void write_section_header (FILE *f, Elf32_Shdr * section_header_table,Elf32_Ehdr *header);


#endif
