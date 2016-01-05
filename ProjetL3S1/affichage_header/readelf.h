#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

typedef struct elf_header elf_header;
struct elf_header {
    uint8_t magic_number[4];
    uint8_t word_size;
    uint8_t endianness;
    uint8_t version;
    uint8_t sys_type;
    Elf32_Half sys_target;
    Elf32_Half file_type;
    Elf32_Word sys_version;
    Elf32_Addr entry_point;
    Elf32_Word flags;
    Elf32_Half header_size;
};

enum { ERROR_MAGIC_NUMBERS = 1, ERROR_MISSING_ARG, ERROR_WRONG_WORD_SIZE, ERROR_WRONG_ENDIAN,
       ERROR_INVALID_VERSION };

void print_elf_header(FILE *file, elf_header *header);
void init_systable();
void init_systarget();

#endif
