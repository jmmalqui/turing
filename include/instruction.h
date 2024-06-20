#ifndef INSTR_H_
#define INSTR_H_
#include <stdbool.h>
typedef struct Instruction {
    char* from;
    char* to;
    char* accept_string;
    char write;
    bool left;
} instruction_t;

typedef struct InstructionSet {
    int size;
    instruction_t instruction;
} instruction_set_t;

#endif
