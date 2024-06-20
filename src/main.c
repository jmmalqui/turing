#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdbool.h"
#include "turing.h"
#define MAX_STATE_FUNC_LENGTH 4096
/*
Input:
    string - string to be worked on
    tmfile - file containing the turing machine commands
*/
typedef struct Input {
    char** chars;
} input_t;

typedef struct Output {
    bool left;
    char write;
} output_t;

typedef struct Instruction {
    char* from;
    char* to;
    input_t accept;
    output_t output;
} instruction_t;

// Turing Machine Cell Struct
typedef struct TuringMachineCell {
    struct cell_t* left;
    struct cell_t* right;
    char data;
} cell_t;

// Turing Machine Struct
typedef struct TuringMachine {
    cell_t tape;
    char** state_set;
    char* curr_state;
    // Array of pointers to instructions
    // [instruction 1] [instruction 2] [instruction 3] ...
    //    ^ *instruction
    instruction_t** instruction_set;
} turing_machine_t;

char* remove_slash_n(char* src) {
    return "meow";
}

char* solve(turing_machine_t* machine, char* input) {
    return "meow";
}

void init_tm(turing_machine_t* machine, char* tm_file) {
    printf("Initializing machine\n");
    machine->curr_state = "q0";
    machine->instruction_set =
        (instruction_t**)malloc(64 * sizeof machine->instruction_set);

    FILE* fptr;
    fptr = fopen(tm_file, "r");
    if (!fptr) {
        printf("Error while loading file\n");
        return NULL;
    }

    char* state_func;
    state_func = malloc(MAX_STATE_FUNC_LENGTH * sizeof(char));

    // strtok_r is better than strtok when doing nested tokenization
    while (fgets(state_func, MAX_STATE_FUNC_LENGTH, fptr) != 0) {
        // For splitting each line as follows:
        // q0 123|4,R q1 -> (q0),(123|4,R),(q1)
        char* space_tokdel_saveptr = NULL;

        // For splitting the second token as follows:
        // 123|4,R -> (123),(4,R)
        char* line_tokdel_saveptr = NULL;

        char* token = strtok_r(state_func, " ", &space_tokdel_saveptr);

        int idx = 0;

        instruction_t* instruction;

        while (token) {
            switch (idx) {
                case 0:
                case 1:
                    char* subtok = strtok_r(token, "|", &line_tokdel_saveptr);
                    while (subtok) {
                        subtok = strtok_r(NULL, "|", &line_tokdel_saveptr);
                    }
                case 2:
            }
            token = strtok_r(NULL, " ", &space_tokdel_saveptr);
            idx += 1;
        }
    }
    fclose(fptr);
}

int main(int argc, char const* argv[]) {
    char* input;
    char* filename;
    for (int i = 1; i < argc; i++) {
        printf("%s, len: %d\n", argv[i], strlen(argv[i]));
        size_t argv_size =
            strlen(argv[i]) * sizeof(char);  // although char is always one byte
        switch (i) {
            case 1:
                input = malloc(argv_size);
                memcpy(input, argv[i], argv_size);
            case 2:
                filename = malloc(argv_size);
                memcpy(filename, argv[i], argv_size);
        }
    }
    printf("Arguments copied succesfully\n");
    turing_machine_t* machine;
    init_tm(machine, filename);
    if (!machine) {
        printf("Machine generation fail.\n");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
