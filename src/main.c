#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mem.h"
#include "stdbool.h"
#include "turing.h"
int main(int argc, char const* argv[])
{
    char* input;
    char* filename;
    for (int i = 1; i < argc; i++) {
        size_t argv_size = strlen(argv[i]) * sizeof(char);
        if (i == 1) {
            input = ALLOC_STR(argv_size);
            memcpy(input, argv[i], argv_size);
        }
        if (i == 2) {
            filename = ALLOC_STR(argv_size);
            memcpy(filename, argv[i], argv_size);
        }
    }
    TuringMachine machine;

    int init_success      = tm_init(&machine);
    int load_file_success = tm_load_file(&machine, filename);
    if (init_success != TM_SUCCESS) {
        printf("Machine generation fail.\n");
        return EXIT_FAILURE;
    }
    if (load_file_success != TM_SUCCESS) {
        printf("File loading failed\n");
        return EXIT_FAILURE;
    }
    char* solution = tm_solve(&machine, input);
    printf("%s\n", solution);
    return EXIT_SUCCESS;
}
