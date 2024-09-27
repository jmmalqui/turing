#include "instruction.h"

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "mem.h"
#define WHITESPACE " "
#define VLINE "|"
#define COMMA ","

int instruction_set_init(InstructionSet* instruction_set)
{
    instruction_set->count        = 0;
    instruction_set->capacity     = 16;
    instruction_set->instructions = ALLOC_ARRAY(Instruction, instruction_set->capacity);
    return TM_SUCCESS;
}

int add_instruction(InstructionSet* instruction_set, char* tm_file_line)
{
    Instruction instruction;
    instruction.count              = 0;
    instruction.capacity           = 4;
    instruction.transition_command = ALLOC_ARRAY(TransitionCommand, instruction.capacity);

    TransitionCommand transition_comm;

    char* space_saveptr = NULL;
    char* vline_saveptr = NULL;
    char* comma_saveptr = NULL;

    char* token = __strtok_r(tm_file_line, WHITESPACE, &space_saveptr);

    int   token_idx = 0;
    char* state_input;

    bool no_instructions_flag      = false;
    bool instruction_already_saved = false;
    int  saved_idx                 = 0;
    while (token) {
        if (token_idx == 0) {
            state_input = ALLOC_STR(strlen(token));
            state_input = strcpy(state_input, token);

            if (instruction_set->count == 0) {
                instruction.state_input = ALLOC_STR(strlen(token));
                instruction.state_input = strcpy(instruction.state_input, state_input);
                no_instructions_flag    = true;
            } else {
                saved_idx = get_saved_instruction_idx(instruction_set, state_input,
                    &instruction_already_saved);
                if (!instruction_already_saved) {
                    instruction.state_input = ALLOC_STR(strlen(token));
                    instruction.state_input = strcpy(instruction.state_input, state_input);
                }
            }
        }
        if (token_idx == 1) {
            char* subtoken     = __strtok_r(token, VLINE, &vline_saveptr);
            int   subtoken_idx = 0;
            while (subtoken) {
                if (subtoken_idx == 0) {
                    transition_comm.accept_string = ALLOC_STR(strlen(subtoken));
                    transition_comm.accept_string = strcpy(transition_comm.accept_string, subtoken);
                }
                if (subtoken_idx == 1) {
                    char* comma_token     = __strtok_r(subtoken, COMMA, &comma_saveptr);
                    int   comma_token_idx = 0;
                    while (comma_token) {
                        if (comma_token_idx == 0) {
                            transition_comm.write = *comma_token;
                        }
                        if (comma_token_idx == 1) {
                            transition_comm.move = *comma_token;
                        }
                        comma_token = __strtok_r(NULL, COMMA, &comma_saveptr);
                        comma_token_idx += 1;
                    }
                }
                subtoken = __strtok_r(NULL, VLINE, &vline_saveptr);
                subtoken_idx += 1;
            }
        }
        if (token_idx == 2) {
            if (*(token + strlen(token) - 1) == '\n')
                *(token + strlen(token) - 1) = 0;
            transition_comm.state_output = ALLOC_STR(strlen(token));
            transition_comm.state_output = strcpy(transition_comm.state_output, token);
        }
        token = __strtok_r(NULL, WHITESPACE, &space_saveptr);
        token_idx += 1;
    }
    if (no_instructions_flag) {
        add_transition_command(&instruction, &transition_comm);
        add_instruction_to_set(instruction_set, &instruction);
        no_instructions_flag = false;
        return TM_SUCCESS;
    }
    if (instruction_already_saved) {
        add_transition_command(&instruction_set->instructions[saved_idx],
            &transition_comm);
        instruction_already_saved = false;
        return TM_SUCCESS;
    }
    if (!instruction_already_saved) {
        add_transition_command(&instruction, &transition_comm);
        add_instruction_to_set(instruction_set, &instruction);
        return TM_SUCCESS;
    }

    return TM_SUCCESS;
}

int add_transition_command(Instruction* instruction,
    TransitionCommand*                  transition_command)
{
    instruction->count += 1;
    if (instruction->count >= instruction->capacity - 1) {
        instruction->capacity *= ARRAY_GROWTH;
        instruction->transition_command = GROW_ARRAY(TransitionCommand, instruction->transition_command,
            instruction->capacity);
    }
    instruction->transition_command[instruction->count - 1] = *transition_command;
    return TM_SUCCESS;
}

int add_instruction_to_set(InstructionSet* instrucion_set,
    Instruction*                           instruction)
{
    instrucion_set->count += 1;
    if (instrucion_set->count >= instrucion_set->capacity - 1) {
        instrucion_set->capacity *= ARRAY_GROWTH;
        instrucion_set->instructions = GROW_ARRAY(
            Instruction, instrucion_set->instructions, instrucion_set->capacity);
    }
    instrucion_set->instructions[instrucion_set->count - 1] = *instruction;
    return TM_SUCCESS;
}

int get_saved_instruction_idx(InstructionSet* instruction_set,
    char* state_input, bool* saved_flag)
{
    for (int i = 0; i < instruction_set->count; i++) {
        if (strcmp(instruction_set->instructions[i].state_input, state_input) == 0) {
            *saved_flag = true;
            return i;
        }
    }
    return NULL;
}
