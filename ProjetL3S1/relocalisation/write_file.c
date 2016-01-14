#include <endian.h>
#include "write_file.h"

void write_file_header (FILE *f, Elf32_Ehdr header) {

    fseek(f, 0, SEEK_SET);


    if (header.e_ident[EI_DATA] == ELFDATA2MSB) {
    // swap endianess to can write in the file in the right format
        header.e_machine = htobe16(header.e_machine);
        header.e_version = htobe32(header.e_version);
        header.e_entry = htobe32(header.e_entry);
        header.e_phoff = htobe32(header.e_phoff);
        header.e_shoff = htobe32(header.e_shoff);
        header.e_flags  = htobe32(header.e_flags);
        header.e_ehsize = htobe16(header.e_ehsize);
        header.e_phentsize = htobe16(header.e_phentsize);
        header.e_phnum = htobe16(header.e_phnum);
        header.e_shentsize = htobe16(header.e_shentsize);
        header.e_shnum = htobe16(header.e_shnum);
        header.e_shstrndx = htobe16(header.e_shstrndx);
    }
    fwrite(&header,sizeof(Elf32_Ehdr),1,f);

    /*    if (header.e_ident[EI_DATA] == ELFDATA2MSB) {
        // swap endianness to can use again the  data
        header.e_machine = be16toh(header.e_machine);
        header.e_version = be32toh(header.e_version);
        header.e_entry = be32toh(header.e_entry);
        header.e_phoff = be32toh(header.e_phoff);
        header.e_shoff = be32toh(header.e_shoff);
        header.e_flags  = be32toh(header.e_flags);
        header.e_ehsize = be16toh(header.e_ehsize);
        header.e_phentsize = be16toh(header.e_phentsize);
        header.e_phnum = be16toh(header.e_phnum);
        header.e_shentsize = be16toh(header.e_shentsize);
        header.e_shnum = be16toh(header.e_shnum);
        header.e_shstrndx = be16toh(header.e_shstrndx);
    }*/

}

void write_section_header (FILE *f, Elf32_Shdr *section_header_table, Elf32_Ehdr header) {
    int i;
    fseek(f, header.e_shoff, SEEK_SET);

       if (header.e_ident[EI_DATA] == ELFDATA2MSB) {
            for (i = 0; i < header.e_shnum; i++) {
                // swap endiannes to can write in the right format in the file
                section_header_table[i].sh_name = htobe32(section_header_table[i].sh_name);
                section_header_table[i].sh_type = htobe32(section_header_table[i].sh_type);
                section_header_table[i].sh_flags = htobe32(section_header_table[i].sh_flags);
                section_header_table[i].sh_addr = htobe32(section_header_table[i].sh_addr);
                section_header_table[i].sh_offset = htobe32(section_header_table[i].sh_offset);
                section_header_table[i].sh_size = htobe32(section_header_table[i].sh_size);
                section_header_table[i].sh_link = htobe32(section_header_table[i].sh_link);
                section_header_table[i].sh_info = htobe32(section_header_table[i].sh_info);
                section_header_table[i].sh_addralign = htobe32(section_header_table[i].sh_addralign);
                section_header_table[i].sh_entsize = htobe32(section_header_table[i].sh_entsize);


            }
}
        fwrite(section_header_table,sizeof(Elf32_Shdr),header.e_shnum,f);

       if (header.e_ident[EI_DATA] == ELFDATA2MSB) {
        for (i = 0; i < header.e_shnum; i++) {
            // swap endiannes to can use again the data
            section_header_table[i].sh_name = be32toh(section_header_table[i].sh_name);
            section_header_table[i].sh_type = be32toh(section_header_table[i].sh_type);
            section_header_table[i].sh_flags = be32toh(section_header_table[i].sh_flags);
            section_header_table[i].sh_addr = be32toh(section_header_table[i].sh_addr);
            section_header_table[i].sh_offset = be32toh(section_header_table[i].sh_offset);
            section_header_table[i].sh_size = be32toh(section_header_table[i].sh_size);
            section_header_table[i].sh_link = be32toh(section_header_table[i].sh_link);
            section_header_table[i].sh_info = be32toh(section_header_table[i].sh_info);
            section_header_table[i].sh_addralign = be32toh(section_header_table[i].sh_addralign);
            section_header_table[i].sh_entsize = be32toh(section_header_table[i].sh_entsize);
            }
        }

}

void write_symbole_table (FILE *f, Elf32_Sym *symb_table,Elf32_Ehdr header,Elf32_Shdr *section_headers_table, uint32_t symbols_count ){

    Elf32_Half symtable_index = 0;
    int i = 0;

    while (section_headers_table[symtable_index].sh_type != SHT_SYMTAB) {
        symtable_index++;
    }
    fseek(f, section_headers_table[symtable_index].sh_offset, SEEK_SET);
    if (header.e_ident[EI_DATA] == ELFDATA2MSB) {
    // switch endianes to can write it in the right format
        for (i = 0; i < symbols_count; ++i) {
            symb_table[i].st_name = htobe32(symb_table[i].st_name);
            symb_table[i].st_value = htobe32(symb_table[i].st_value);
            symb_table[i].st_shndx = htobe16(symb_table[i].st_shndx);
            symb_table[i].st_size = htobe32(symb_table[i].st_size);
        }
    }
    fwrite(symb_table,sizeof(Elf32_Sym),symbols_count,f);

    if (header.e_ident[EI_DATA] == ELFDATA2MSB) {
    // swap endiannes to can use again the data
        for (i = 0; i < symbols_count; ++i) {
            symb_table[i].st_name = be32toh(symb_table[i].st_name);
            symb_table[i].st_value = be32toh(symb_table[i].st_value);
            symb_table[i].st_shndx = be16toh(symb_table[i].st_shndx);
            symb_table[i].st_size = be32toh(symb_table[i].st_size);
        }
    }
}

void write_section_content(FILE *f,Elf32_Shdr *section_header_table, Elf32_Ehdr header, uint8_t **section_content){
    int i;
    for(i=0;i<header.e_shnum;i++){
        if (section_header_table[i].sh_size!=0 || section_header_table[i].sh_type != SHT_NOBITS || section_header_table[i].sh_type != SHT_NULL){
            fseek(f, section_header_table[i].sh_offset, SEEK_SET);
            fwrite(section_content[i],sizeof(uint8_t),section_header_table[i].sh_size,f);
       }
    }

}


