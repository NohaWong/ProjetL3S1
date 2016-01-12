#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <getopt.h>
#include "compute_args.h"


/**
 * This function retrieve all arguments given in command line by the @a main
 * function.
 *
 * @param argc     integer, arguments count
 * @param argv     char**, arguments value represented by strings
 * @return Return a code defined is the error enumeration in @a printfelf.h
 *
 */
int compute_multiple_args (int argc, char **argv, main_flags *flags, int *argv_idx_filename) {
    int opt;

	struct option longopts[] = {
		{ "header", no_argument, NULL, 'h' },
		{ "symbols", no_argument, NULL, 's' },
		{ "sections", no_argument, NULL, 'S' },
		{ "help", no_argument, NULL, 'H' },
		{ "hex-dump", required_argument, NULL, 'x' },
		{ "relocs", no_argument, NULL, 'r' },
		{ "all", no_argument, NULL, 'a' },
		{ NULL, 0, NULL, 0 }
	};

	while ((opt = getopt_long(argc, argv, "ahsSx:rH", longopts, NULL)) != -1) {
		switch(opt) {
		case 'h':
			flags->hflag = 1;
			break;
		case 'S':
			flags->Sflag = 1;
			break;
		case 'H':
            print_help();
			return ERROR_MISSING_ARG;
		case 's':
			flags->sflag = 1;
			break;
        case 'x':
            flags->xflag = 1;
            flags->xflag_arg = optarg;
            if (is_numeric(flags->xflag_arg)) {
                flags->is_xflagarg_int = 0;
            } else {
                flags->is_xflagarg_int = 1;
            }
            break;
        case 'r':
            flags->rflag = 1;
            break;
        case 'a':
            flags->hflag = flags->rflag = flags->Sflag = flags->sflag = 1;
            break;
		default:
			fprintf(stderr, "Unrecognized option %c\n", opt);
			exit(1);
		}
	}

	// missing file arg
	if (argc == optind) {
        print_help();
        return ERROR_MISSING_ARG;
	}

	*argv_idx_filename = optind;

    return EXIT_SUCCESS;
}

/**
 * @todo
 */
int compute_no_args (const char *filename) {
    // TODO
    return EXIT_SUCCESS;
}

/**
 * This function prints help on screen.
 */
void print_help() {
    printf("Manuel utilisateur - our_readelf\n\n");
    printf(BOLDWHITE "NOM\n" RESET);
    printf("\tour_readelf - lit un fichier au format ELF et affiche ses caractéristiques\n\n");
    printf(BOLDWHITE "SYNOPSIS\n");
    printf("\tour_readelf [-hsSrx]" RESET " [<nom ou nombre> si l'option x est spécifiée] FICHIER\n\n");
    printf(BOLDWHITE "DESCRIPTION\n" RESET);
    printf("\tLit un fichier au format ELF et affiche les caractéristiques voulues à l'écran.\n\tSi aucun argument n'est spécifié, un menu apparait à l'écran pour choisir l'opération souhaitée.\n\n");
    printf(BOLDWHITE "\t-a\n" RESET);
    printf("\t\tÉquivalent à " BOLDWHITE "-hSsr" RESET ", " BOLDWHITE "-h-S-s-r" RESET " ou " BOLDWHITE "-h" RESET ", " BOLDWHITE "-S" RESET ", " BOLDWHITE "-s" RESET ", et " BOLDWHITE "-r" RESET ".\n\n");
    printf(BOLDWHITE "\t-h\n" RESET);
    printf("\t\tAffiche l'en-tête (header) du fichier ELF spécifié en entrée.\n\n");
    printf(BOLDWHITE "\t-s\n" RESET);
    printf("\t\tAffiche la table des symboles du fichier ELF spécifié en entrée.\n\n");
    printf(BOLDWHITE "\t-S\n" RESET);
    printf("\t\tAffiche l'en-tête (header) des sections du fichier ELF spécifié en entrée.\n\n");
    printf(BOLDWHITE "\t-x <nombre ou nom>\n" RESET);
    printf("\t\tAffiche le contenu de la section donnée par son numéro ou son nom. Doit être spécifié en tant que dernière option.\n\n");
    printf(BOLDWHITE "\t--help\n" RESET);
    printf("\t\tAffiche cette aide.\n\n");
    printf(BOLDWHITE "\t-r\n" RESET);
    printf("\t\tAffiche la table de réimplantation statique.\n\n");
//  printf(BOLDWHITE "\t-R\n" RESET);
//  printf("\t\tAffiche la table de réimplantation dynamique.\n\n");
    printf(BOLDWHITE "    Valeurs de retour :\n" RESET);
    printf("\t0\tLe programme s'est terminé normalement.\n\n");
    printf("\t1\tLes nombres magiques sont erronés.\n\n");
    printf("\t2\tLe nombre d'arguments envoyés au programme est inccorect.\n\n");
    printf("\t3\tLe fichier ELF n'est pas au format 32-bits.\n\n");
    printf("\t4\tL'endianness du fichier n'est pas défini.\n\n");
    printf("\t5\tLa version du fichier ELF n'est pas valide.\n\n");
    printf("\t6\tAucun fichier n'a été spécifié en entrée, ou il n'est pas accessible.\n\n");
    printf("\tAutre\tUne erreur inconnue s'est produite.\n\n");
    printf(BOLDWHITE "AUTEURS\n" RESET);
    printf("\tÉcrit par Alexandre Daubois, Maxence Ginet, Sylvain Marion, Hugo Serem et Noha Wong.\n\n");
}

/**
 * This function chek if the given string in a number ( @a isdigit extended to a string)
 * From http://rosettacode.org/wiki/Determine_if_a_string_is_numeric#C
 *
 * @param s     const char*, string containing (or not) the number
 * @return 0 if the string is not a number, otherwise return non-zero value
 *
 */
int is_numeric (const char * s) {
    if (s == NULL || *s == '\0' || isspace(*s)) {
        return 0;
    }
    char * p;
    strtol(s, &p, 10);
    return *p == '\0';
}
