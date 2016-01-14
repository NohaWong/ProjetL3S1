#include "../commun/readelf.h"
#include "../affichage/printelf.h"
#include "relocalise.h"
#include "write_file.h"


#define test 1 //0 pour aucun test,1 pour l'ecriture dans une fichier , (ect a compléter)

extern int *old_sec_to_new_sec;


// APPEL : relocalisation <nom_fichier> <nom_fichier_cible> <nom_section adresse_relocalisee> [nom_section_x adresse_relocalisee_x]

int main(int argc, char **argv) {
    uint32_t i;


    if (argc < 3){
        printf("Pas le bon nombre d'argument\n");

        return 2;
    }
    else{

        FILE *file_source = fopen(argv[1], "rb");
        FILE *file_target = fopen(argv[2], "wb");


        if (file_source == NULL ) {
            printf("Le fichier source n'existe pas.\n");
            return EXIT_FAILURE;
        }

        if (file_target == NULL ){
           printf("Le fichier cible n'existe pas.\n");
           fclose(file_source);
           return EXIT_FAILURE;
        }

        Elf32_Ehdr header, new_header;
        Elf32_Shdr *section_header_table = NULL;
        char *section_header_name = NULL;
        uint32_t symbols_count = 0, new_symb_count = 0;
        uint8_t **section_content;

        // load everything before printing
        read_elf_header(file_source, &header);

     //   memcpy(new_header, &header, sizeof(Elf32_Ehdr));
    //    write_file_header(file_target, &new_header);
		old_sec_to_new_sec = malloc(header.e_shnum*sizeof(int));

        section_header_table = read_elf_section_header(file_source, &header, &section_header_name);
        Elf32_Sym *symbols = read_symbol_table(header,file_source, section_header_table,/* header.e_shnum,*/ &symbols_count);
        section_content = read_section_content(file_source, section_header_table, &header);
        Table_rel_set table_rel= read_rel_table(file_source, section_header_table, header.e_shnum);
        char * new_sec_name;
        //(void)table_rel;




        // get args
        if (!(argc%2)) {
            printf("Nombre d'arguments incorrect. Veuillez veiller à donner une adresse de relocalisation a chaque section que vous souhaitez relocaliser.\n");
            return EXIT_FAILURE;
        }
        uint32_t rel_count = (argc-2)/2;

        rel_info* table_rel_info;
        table_rel_info = malloc(rel_count * sizeof(rel_info));



        for (i=0; i<rel_count; ++i) {
            table_rel_info[i].section_name = argv[(2*i)+3];
            table_rel_info[i].section_new_addr = strtol(argv[((2*i)+1)+3], NULL, 16);

        }


        // create new structs
        new_header = header;
        Elf32_Shdr *new_sections_header = new_section_header(section_header_table, &new_sec_name, section_header_name, table_rel_info, rel_count, header, &new_header);
        Elf32_Sym *new_symb_table = new_symbol_table(symbols, table_rel_info, symbols_count, &new_symb_count, rel_count, section_header_table, new_sec_name, new_sections_header);

        // display tests

        // display header
        printf(BOLDYELLOW "============================= AVANT =============================\n" RESET);
        print_elf_header(header);
        printf(BOLDYELLOW "============================= APRÈS =============================\n" RESET);
        print_elf_header(new_header);


       // display test symb_table (before modif)
        printf(BOLDYELLOW "============================= AVANT =============================\n" RESET);
        print_elf_symbol_table(symbols, symbols_count);
        printf(BOLDYELLOW "============================= APRÈS =============================\n" RESET);
        print_elf_symbol_table(new_symb_table, new_symb_count);

        // display test section header (before modif)
        printf(BOLDYELLOW "============================= AVANT =============================\n" RESET);
        print_elf_section_header(header, section_header_table, section_header_name);
        printf(BOLDYELLOW "============================= APRÈS =============================\n" RESET);
        print_elf_section_header(new_header, new_sections_header, new_sec_name);

        printf(BOLDYELLOW "============================= AVANT =============================\n" RESET);
        print_elf_section_content(section_content, 1, section_header_table, section_header_name, header);
        uint8_t **new_section = new_section_content (table_rel, section_header_name, section_content, table_rel_info, section_header_table, header, rel_count, symbols, new_symb_table);
        printf(BOLDYELLOW "============================= APRÈS =============================\n" RESET);
        print_elf_section_content(new_section, 1, new_sections_header, new_sec_name, new_header);


        free(symbols);
        //free(new_symb_table);
        free(section_header_table);
        free(new_sections_header);
        free(section_header_name);
        free(old_sec_to_new_sec);
        free(new_sec_name);

        for (i = 0; i < header.e_shnum; ++i) {
            free(new_section[i]);
        }
        free(new_section);

        if (test==1){
            write_file_header(file_target, header);
            write_section_header(file_target,section_header_table,header);
            write_symbole_table(file_target,symbols,header,section_header_table,symbols_count);
            write_section_content(file_target,section_header_table,header,section_content);
        }


        fclose(file_source);
        fclose(file_target);

        free(table_rel_info);
        free(section_content);

        return 0;
    }
}


