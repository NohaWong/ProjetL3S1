#ifndef COMPUTE_ARGS_H
#define COMPUTE_ARGS_H

#include "../commun/readelf.h"
#include "printelf.h"

/**
 * This function retrieve all arguments given in command line by the @a main
 * function.
 *
 * @param argc     integer, arguments count
 * @param argv     char**, arguments value represented by strings
 * @return Return a code defined is the error enumeration in @a printfelf.h
 *
 */
int compute_multiple_args (int argc, char **argv);

/**
 * @todo
 */
int compute_no_args (const char *filename);

/**
 * This function prints help on screen.
 */
void print_help();

/**
 * This function chek if the given string in a number ( @a isdigit extended to a string)
 * From http://rosettacode.org/wiki/Determine_if_a_string_is_numeric#C
 *
 * @param s     const char*, string containing (or not) the number
 * @return 0 if the string is not a number, otherwise return non-zero value
 *
 */
int is_numeric (const char * s);

#endif // COMPUTE_ARGS_H
