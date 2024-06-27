#include "turing.h"

#include <stdbool.h>

#include "common.h"
#include "ctype.h"
#include "memory.h"
#include "stdio.h"
#include "stdlib.h"
#include "utils.h"

int tm_init(TuringMachine* machine) {
    machine->current_state = ALLOC_STR(2);
    machine->current_state = strcpy(machine->current_state, "q0");
    tape_init(&machine->tape);
    instruction_set_init(&machine->instruction_set);

    return TM_SUCCESS;
}

int tm_load_file(TuringMachine* machine, char* tm_file) {
    FILE* fptr;
    fptr = fopen(tm_file, "r");

    if (!fptr) return TM_ERROR;

    char* transition_function = ALLOC_STR(4096);

    while (fgets(transition_function, 4096, fptr) != 0) {
        add_instruction(&machine->instruction_set, transition_function);
    }
    fclose(fptr);
    return TM_SUCCESS;
}
char* tm_solve(TuringMachine* machine, char* input) {
    printf("\nSolving turing machine for %s\n", input);

    return input;
}

void tm_dealloc(TuringMachine* machine) { free(machine); }
