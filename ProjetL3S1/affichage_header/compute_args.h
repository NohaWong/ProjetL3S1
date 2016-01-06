#ifndef COMPUTE_ARGS_H
#define COMPUTE_ARGS_H

#include "readelf.h"
#include "printelf.h"

int compute_multiple_args (int argc, char **argv);
int compute_no_args (const char *filename);
void print_help();

#endif
