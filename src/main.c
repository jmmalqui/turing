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
        switch (i) {
            case 1:
                input = malloc(argv_size + 1);
                memcpy(input, argv[i], argv_size);
            case 2:
                filename = malloc(argv_size + 1);
                memcpy(filename, argv[i], argv_size);
        }
    }
    printf("Arguments copied succesfully\n");
    TuringMachine* machine = tm_from_file_init(filename);
    if (!machine) {
        printf("Machine generation fail.\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
