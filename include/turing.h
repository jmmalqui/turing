#ifndef turing_h
#define turing_h_
#include <stdbool.h>

#include "instruction.h"
#include "state.h"
#include "tape.h"

typedef struct
{
    // StateSet state_set; This is useless
    char *current_state;
    Tape tape;
    InstructionSet instruction_set;
} TuringMachine;

int tm_init(TuringMachine *machine);

int tm_load_file(TuringMachine *machine, char *tm_file);

char *tm_solve(TuringMachine *machine, char *input);

bool tm_has_state(TuringMachine *machine, char *state);

void tm_debug(TuringMachine *machine);

void tm_dealloc(TuringMachine *machine);

#endif