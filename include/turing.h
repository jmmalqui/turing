#ifndef TURING_H_
#define TURING_H_
#include "instruction.h"
#include "tape.h"
#define TFLEN 4096
typedef struct TuringMachine {
    char* curr_state;
    char** state_set;
    tape_cell_t tape;
    instruction_set_t instruction_set;
} turing_machine_t;

turing_machine_t* tm_from_file_init(char* tm_file);
void register_instruction(turing_machine_t* machine,
                          instruction_t* instruction);
char* tm_solve(turing_machine_t* machine, char* input);

#endif