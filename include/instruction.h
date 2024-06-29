#ifndef turing_instruction_h
#define turing_instruction_h
#include <stdbool.h>

#include "mem.h"
/*
Each .tm file contains turing instructions of the following layout:

    [                 Instruction                   ]
    [ Input State  ][       Transition Command      ]
                    [Sub command]      [Output State]

One state can have multiple transition commands pointing at different states
including itself.
*/

typedef struct
{
    char *accept_string;
    char *state_output;
    char write;
    char move;
} TransitionCommand;

typedef struct
{
    char *state_input;
    int count;
    int capacity;
    TransitionCommand *transition_command;
} Instruction;

typedef struct
{
    int count;
    int capacity;
    Instruction *instructions;
} InstructionSet;

int instruction_set_init(InstructionSet *instructions);
int add_instruction(InstructionSet *instructions, char *tm_file_line);

#endif
