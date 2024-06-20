#include "turing.h"
#include "memory.h"
#include "stdio.h"
#include "stdlib.h"
#include "utils.h"

turing_machine_t* tm_from_file_init(char* tm_file) {
    turing_machine_t* machine =
        (turing_machine_t*)malloc(sizeof(turing_machine_t));
    machine->instruction_set;
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

        instruction_t instr = {0};
        instruction_t* instruction = &instr;
        while (token) {
            switch (token_idx) {
                case 0:
                    instruction->from =
                        (char*)malloc(1 + strlen(token) * sizeof(char));

                    instruction->from = strcpy(instruction->from, token);

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
                    instruction->to =
                        (char*)malloc(1 + strlen(token) * sizeof(char));
                    instruction->to = strcpy(instruction->to, token);
            }

            token = __strtok_r(NULL, " ", &space_token_delim_saveptr);
            token_idx += 1;
        }
        printf("intruction from: %s\n", instruction->from);
        printf("intruction to: %s\n", instruction->to);
        register_instruction(machine, instruction);
    }
    fclose(fptr);
    return machine;
}

void register_instruction(turing_machine_t* machine,
                          instruction_t* Instruction) {}
