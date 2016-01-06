#include "compute_args.h"

int main(int argc, char **argv) {
    int return_value = EXIT_SUCCESS;

    if (argc < 2) {
        print_help();
        return ERROR_MISSING_ARG;
    }

    if (argc >= 3) {
        return_value = compute_multiple_args(argc, argv);
    }

    return return_value;
}
