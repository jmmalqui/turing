#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mem.h"
#include "stdbool.h"
#include "turing.h"

int main(int argc, char const *argv[])
{
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <input> <path>\n", argv[0]);
        return EXIT_FAILURE;
    }

    char *input = (char *) argv[1];
    char *filename = (char *) argv[2];

    TuringMachine machine;
    int init_success = tm_init(&machine);
    int load_file_success = tm_load_file(&machine, filename);

    if (init_success != TM_SUCCESS)
    {
        fprintf(stderr, "Machine generation fail.\n");
        return EXIT_FAILURE;
    }

    if (load_file_success != TM_SUCCESS)
    {
        fprintf(stderr, "File loading failed\n");
        return EXIT_FAILURE;
    }

    char *solution = tm_solve(&machine, input);
    printf("%s\n", solution);

    return EXIT_SUCCESS;
}
