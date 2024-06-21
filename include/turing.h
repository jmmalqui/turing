#ifndef turing_h
#define turing_h_
#include <stdbool.h>
#include "instruction.h"
#include "tape.h"

#define TFLEN 4096
typedef struct {
    char* curr_state;
    int state_set_size;
    char** state_set;
    Tape* tape;
    int instruction_set_size;
    Instruction* instruction_set;
} TuringMachine;

void tm_init(TuringMachine* machine);

TuringMachine* tm_from_file_init(char* tm_file);

void register_instruction(TuringMachine* machine, Instruction* instruction);

char* tm_solve(TuringMachine* machine, char* input);

bool tm_has_state(TuringMachine* machine, char* state);

void tm_debug(TuringMachine* machine);

#endif