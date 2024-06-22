#ifndef turing_h
#define turing_h_
#include "instruction.h"
#include "tape.h"
#include <stdbool.h>

#define TFLEN 4096
typedef struct
{
    char* current_state;
    int state_count;
    int state_capacity;
    char** state_set;
    Tape* tape;
    int instruction_count;
    int instruction_capacity;
    Instruction* instruction_set;
} TuringMachine;

TuringMachine*
tm_init(void);

TuringMachine*
tm_from_file_init(char* tm_file);

void
register_instruction(TuringMachine* machine, Instruction* instruction);

char*
tm_solve(TuringMachine* machine, char* input);

bool
tm_has_state(TuringMachine* machine, char* state);

void
tm_debug(TuringMachine* machine);

void
tm_dealloc(TuringMachine* machine);

#endif