#ifndef turing_instruction_h
#define turing_instruction_h
#include <stdbool.h>

/*
Each .tm file contains turing instructions of the following layout:

    [                 Instruction                   ]
    [ Input State  ][       Transition Command      ]
                    [Sub command]      [Output State]

One state can have multiple transition commands pointing at different states
including itself.
*/

typedef struct {
    char* accept_string;
    char* state_output;
    char write;
    bool left;
} TransitionCommand;

typedef struct {
    char* state_input;
    int size;
    TransitionCommand* transition_command;
} Instruction;
#endif
