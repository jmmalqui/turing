#include "turing.h"

#include <stdbool.h>

#include "common.h"
#include "ctype.h"
#include "mem.h"
#include "memory.h"
#include "stdio.h"
#include "stdlib.h"
#include "utils.h"
int tm_init(TuringMachine *machine)
{
    machine->current_state = ALLOC_STR(2);
    machine->current_state = strcpy(machine->current_state, "q0");
    tape_init(&machine->tape);
    instruction_set_init(&machine->instruction_set);
    return TM_SUCCESS;
}

int tm_load_file(TuringMachine *machine, char *tm_file)
{
    FILE *fptr;
    fptr = fopen(tm_file, "r");

    if (!fptr)
        return TM_ERROR;

    char *transition_function = ALLOC_STR(4096);
    while (fgets(transition_function, 4096, fptr) != 0)
    {
        add_instruction(&machine->instruction_set, transition_function);
    }
    fclose(fptr);
    return TM_SUCCESS;
}
char *tm_solve(TuringMachine *machine, char *input)
{
    machine->tape.data = strcpy(machine->tape.data, input);
    machine->tape.count = strlen(input);
    int tape_idx = 0;
    bool reject = false;
    bool halt = false;
    while (reject == false && halt == false)
    {

        for (int i = 0; i < machine->instruction_set.count; i++)
        {
            if (strcmp(machine->instruction_set.instructions[i].state_input, machine->current_state) == 0)
            {
                bool found_instruction = false;
                for (int j = 0; j < machine->instruction_set.instructions[i].count; j++)
                {

                    bool outside_word = machine->tape.pointer >= machine->tape.count;
                    bool underbar_in_accept_str =
                        strchr(machine->instruction_set.instructions[i].transition_command[j].accept_string, '_') !=
                        NULL;
                    bool is_observed_NULL = machine->tape.data[machine->tape.pointer] == NULL;
                    bool observed_char_in_accept_string =
                        (strchr(machine->instruction_set.instructions[i].transition_command[j].accept_string,
                                machine->tape.data[machine->tape.pointer]) != NULL) &&
                        (is_observed_NULL == false);

                    if (observed_char_in_accept_string || (is_observed_NULL && underbar_in_accept_str))
                    {
                        found_instruction = true;
                        // Change state
                        machine->current_state = ALLOC_STR(
                            strlen(machine->instruction_set.instructions[i].transition_command[j].state_output));
                        machine->current_state =
                            strcpy(machine->current_state,
                                   machine->instruction_set.instructions[i].transition_command[j].state_output);
                        // Overwrite
                        if (machine->instruction_set.instructions[i].transition_command[j].write != '/')
                        {
                            machine->tape.data[machine->tape.pointer] =
                                machine->instruction_set.instructions[i].transition_command[j].write;
                        }
                        // Move
                        switch (machine->instruction_set.instructions[i].transition_command[j].move)
                        {
                        case 'R':
                            machine->tape.pointer += 1;
                            if (machine->tape.pointer >= machine->tape.capacity)
                            {
                                machine->tape.data =
                                    GROW_ARRAY(char, machine->tape.data, machine->tape.capacity *ARRAY_GROWTH);
                            }
                            break;
                        case 'L':
                            machine->tape.pointer -= 1;
                            if (machine->tape.pointer < 0)
                            {
                                reject = true;
                            }
                            break;
                        case 'S':
                            halt = true;
                            break;
                        default:
                            break;
                        }
                        break;
                    }
                }
                if (!found_instruction)
                {
                    reject = true;
                }
            }
        }
    }
    if (halt)
    {
        return machine->tape.data;
    }
    char *reject_str = ALLOC_STR(6);
    reject_str = strcpy(reject_str, "reject");
    if (reject)
    {
        return reject_str;
    }
}

void tm_dealloc(TuringMachine *machine)
{
    free(machine);
}
