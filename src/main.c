#include "stdbool.h"
#include "turing.h"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STATE_FUNC_LENGTH 4096
/*
Input:
    string - string to be worked on
    tmfile - file containing the turing machine commands
*/

int
main(int argc, char const* argv[])
{
    char* input;
    char* filename;
    for (int i = 1; i < argc; i++) {
        size_t argv_size =
          strlen(argv[i]) * sizeof(char); // although char is always one byte
        if (i == 1) {
            input = ALLOC_STR(argv_size);
            memcpy(input, argv[i], argv_size);
            printf("[%d] Loading input string: %s\n", i, input);
        }
        if (i == 2) {
            filename = ALLOC_STR(argv_size);
            memcpy(filename, argv[i], argv_size);
            printf("[%d] Loading filename: %s\n", i, filename);
        }
    }
    TuringMachine machine;
    int init_success = tm_init(&machine);
    int load_file_success = tm_load_file(&machine, filename);
    if (init_success != TM_SUCCESS) {
        printf("Machine generation fail.\n");
        return EXIT_FAILURE;
    }
    tm_solve(&machine, input);
    return EXIT_SUCCESS;
}
