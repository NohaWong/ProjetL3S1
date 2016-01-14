#include "compute_args.h"

int main(int argc, char **argv) {
    main_flags flags = { 0, 0, 0, 0, NULL, 0, 0 };
    int argv_idx_filename = 0;
    int return_value = compute_multiple_args(argc, argv, &flags, &argv_idx_filename);

    if (return_value != EXIT_SUCCESS) {
        return return_value;
    }

    init_systable();
    init_systarget();

    FILE *file = fopen(argv[argv_idx_filename], "rb");
    if (file == NULL) {
        printf("Le fichier n'existe pas.\n");
        return EXIT_FAILURE;
    }

    Elf32_Ehdr header;
    Elf32_Shdr *sections_headers_table = NULL;
    char *sections_name_table = NULL;
    uint32_t symbols_count = 0;
    uint8_t **section_content;

    handle_errors(read_elf_header(file, &header));
    sections_headers_table = read_elf_section_header(file, &header, &sections_name_table);
    Elf32_Sym *symbols = read_symbol_table(header,file, sections_headers_table, &symbols_count);
    section_content = read_section_content(file, sections_headers_table, &header);
    Table_rel_set table_rel= read_rel_table(file, sections_headers_table, header.e_shnum);

    if (flags.hflag == 1) {
        // print header
        print_elf_header(header);
    }
    if (flags.sflag == 1) {
        // print symbols
        print_elf_symbol_table(symbols, symbols_count);
    }
    if (flags.Sflag == 1) {
        // print sections
        print_elf_section_header(header, sections_headers_table, sections_name_table);
    }
    if (flags.xflag == 1) {
        // print hex-dump of a section
        int xflag_argint = 0;
        if (flags.is_xflagarg_int == 0) {
            xflag_argint = strtol(flags.xflag_arg, NULL, 10);
        } else {
            xflag_argint = section_name_to_number(flags.xflag_arg, sections_headers_table, sections_name_table, &header);
        }
        print_elf_section_content(section_content, xflag_argint, sections_headers_table, flags.xflag_arg, header);
    }
    if (flags.rflag == 1) {
        print_elf_rel_tab(table_rel, symbols, sections_headers_table, sections_name_table, header);
    }

    free(symbols);
    free(sections_headers_table);
    free(sections_name_table);

    return return_value;
}
