#include "turing.h"
#include <stdbool.h>
#include "ctype.h"
#include "memory.h"
#include "stdio.h"
#include "stdlib.h"
#include "utils.h"
void tm_debug(TuringMachine* machine) {
    printf("[Machine contents]\n");
    printf("Current state: %s\n", machine->curr_state);
    printf("Current num of states: %d\n", machine->state_set_size);
    printf("Current num of instructions: %d\n", machine->instruction_set_size);
}

TuringMachine* tm_from_file_init(char* tm_file) {
    // Turing machine initialization
    TuringMachine* machine = (TuringMachine*)malloc(sizeof(TuringMachine));
    machine->curr_state = (char*)malloc(3);
    machine->curr_state = strcpy(machine->curr_state, "q0");
    /*
    State set initialization
    */
    machine->state_set_size = 16;
    machine->state_set =
        (char**)malloc(machine->state_set_size * sizeof(char*));
    printf("State set init success\n");
    /*
    Tape initialization
    */
    machine->tape = (Tape*)malloc(sizeof(Tape));
    machine->tape->data = 0;
    machine->tape->right = NULL;
    machine->tape->left = NULL;
    printf("Tape init success\n");
    /*
    Instruction size initialization
    */
    machine->instruction_set_size = 16;
    machine->instruction_set = (Instruction*)malloc(
        machine->instruction_set_size * sizeof(Instruction));
    tm_debug(machine);

    FILE* fptr;
    fptr = fopen(tm_file, "r");
    if (!fptr) {
        printf("Error loading file\n");
        return NULL;
    }
    char* transition_function;
    transition_function = malloc(TFLEN);

    while (fgets(transition_function, TFLEN, fptr) != 0) {
        printf("Instruction: %s\n", transition_function);
        char* space_token_delim_saveptr = NULL;
        char* line_token_delim_saveptr = NULL;

        char* token =
            __strtok_r(transition_function, " ", &space_token_delim_saveptr);

        int token_idx = 0;

        char* state_input;
        while (token) {
            switch (token_idx) {
                case 0:
                    state_input = (char*)malloc(strlen(token) + 1);
                    state_input = strcpy(state_input, token);
                    char* p = state_input;
                    long num;
                    while (*p) {
                        if (isdigit(*p) ||
                            (*p == '-' || *p == '+') && isdigit(*(p + 1))) {
                            num = strtol(p, &p, 10);
                            printf("%ld\n", num);
                        } else {
                            p++;
                        }
                    }
                    if (num > machine->instruction_set_size) {
                        printf(
                            "Reallocating more memory to make room for new "
                            "states\n");
                        machine->instruction_set_size = (int)num;
                        /* Change this later you silly goose (check the
                         * the return type) */
                        Instruction* dummy = (Instruction*)realloc(
                            machine->instruction_set,
                            machine->instruction_set_size *
                                sizeof(Instruction));
                        if (!dummy) {
                            printf(
                                "Realloc failed when trying to allocate memory "
                                "for the instruction set");
                            return NULL;
                        }
                        machine->instruction_set = dummy;
                    }
                    tm_debug(machine);
                case 1:
                    char* subtoken =
                        __strtok_r(token, "|", &line_token_delim_saveptr);
                    while (subtoken) {
                        subtoken =
                            __strtok_r(NULL, "|", &line_token_delim_saveptr);
                    }

                case 2:
                    if (*(token + strlen(token) - 1) == '\n') {
                        *(token + strlen(token) - 1) = 0;
                    }
            }

            token = __strtok_r(NULL, " ", &space_token_delim_saveptr);
            token_idx += 1;
        }
        // register_instruction(machine, instruction);
    }
    fclose(fptr);
    return machine;
}

void register_instruction(TuringMachine* machine, Instruction* Instruction) {}
