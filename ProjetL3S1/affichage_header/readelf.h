#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

typedef struct elf_header elf_header;
struct elf_header {
    int8_t magic_number[4];
    Elf32_Half word_size;
    Elf32_Half endianess;
    Elf32_Word version;
    Elf32_Half sys_type;
    Elf32_Half file_type;
};

enum { ERROR_MAGIC_NUMBERS = 1, ERROR_MISSING_ARG, ERROR_WRONG_WORD_SIZE, ERROR_WRONG_ENDIAN,
       ERROR_INVALID_VERSION };

void print_elf_header(FILE *file, elf_header *header);
void init_systable();

#endif
