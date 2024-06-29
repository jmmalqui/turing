#include "instruction.h"

#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "mem.h"
#define WHITESPACE " "
#define VLINE "|"
#define COMMA ","

int instruction_set_init(InstructionSet *instruction_set)
{
    instruction_set->count = 0;
    instruction_set->capacity = 16;
    instruction_set->instructions = ALLOC_ARRAY(Instruction, instruction_set->capacity);
    return TM_SUCCESS;
}

int add_instruction(InstructionSet *instruction_set, char *tm_file_line)
{
    Instruction instruction;
    instruction.count = 0;
    instruction.capacity = 4;
    instruction.transition_command = ALLOC_ARRAY(TransitionCommand, instruction.capacity);

    TransitionCommand transition_comm;

    char *space_saveptr = NULL;
    char *vline_saveptr = NULL;
    char *comma_saveptr = NULL;

    char *token = strtok_r(tm_file_line, WHITESPACE, &space_saveptr);

    int token_idx = 0;
    char *state_input;

    bool no_instructions_flag = false;
    bool instruction_already_saved = false;
    int saved_idx = 0;

    // TODO: Escape the indentation hell here    
    while (token)
    {
        if (token_idx == 0)
        {
            state_input = ALLOC_STR(strlen(token));
            state_input = strcpy(state_input, token);

            if (instruction_set->count == 0)
            {
                instruction.state_input = ALLOC_STR(strlen(token));
                instruction.state_input = strcpy(instruction.state_input, state_input);
                no_instructions_flag = true;
            }
            else
            {
                for (int i = 0; i < instruction_set->count; i++)
                {
                    if (strcmp(instruction_set->instructions[i].state_input, state_input) == 0)
                    {
                        instruction_already_saved = true;
                        saved_idx = i;
                        break;
                    }
                }
                if (!instruction_already_saved)
                {
                    instruction.state_input = ALLOC_STR(strlen(token));
                    instruction.state_input = strcpy(instruction.state_input, state_input);
                }
            }
        }
        if (token_idx == 1)
        {
            char *subtoken = strtok_r(token, VLINE, &vline_saveptr);
            int subtoken_idx = 0;
            while (subtoken)
            {
                if (subtoken_idx == 0)
                {
                    transition_comm.accept_string = ALLOC_STR(strlen(subtoken));
                    transition_comm.accept_string = strcpy(transition_comm.accept_string, subtoken);
                }
                if (subtoken_idx == 1)
                {
                    char *comma_token = strtok_r(subtoken, COMMA, &comma_saveptr);
                    int comma_token_idx = 0;
                    while (comma_token)
                    {
                        if (comma_token_idx == 0)
                        {
                            transition_comm.write = *comma_token;
                        }
                        if (comma_token_idx == 1)
                        {
                            transition_comm.move = *comma_token;
                        }
                        comma_token = strtok_r(NULL, COMMA, &comma_saveptr);
                        comma_token_idx += 1;
                    }
                }
                subtoken = strtok_r(NULL, VLINE, &vline_saveptr);
                subtoken_idx += 1;
            }
        }
        if (token_idx == 2)
        {
            if (*(token + strlen(token) - 1) == '\n')
                *(token + strlen(token) - 1) = 0;
            transition_comm.state_output = ALLOC_STR(strlen(token));
            transition_comm.state_output = strcpy(transition_comm.state_output, token);
        }
        token = strtok_r(NULL, WHITESPACE, &space_saveptr);
        token_idx += 1;
    }
    if (no_instructions_flag)
    {
        instruction.count += 1;
        if (instruction.count >= instruction.capacity - 1)
        {
            instruction.transition_command =
                GROW_ARRAY(TransitionCommand, instruction.transition_command, instruction.capacity * ARRAY_GROWTH);
            instruction.capacity *= ARRAY_GROWTH;
        }
        instruction.transition_command[instruction.count - 1] = transition_comm;

        instruction_set->count += 1;
        if (instruction_set->count >= instruction.capacity - 1)
        {
            instruction_set->instructions =
                GROW_ARRAY(Instruction, instruction_set->instructions, instruction_set->capacity * ARRAY_GROWTH);
            instruction_set->capacity *= ARRAY_GROWTH;
        }
        instruction_set->instructions[instruction_set->count - 1] = instruction;
        no_instructions_flag = false;
        return TM_SUCCESS;
    }
    if (instruction_already_saved)
    {
        instruction_set->instructions[saved_idx].count += 1;
        if (instruction_set->instructions[saved_idx].count >= instruction_set->instructions[saved_idx].capacity - 1)
        {
            instruction_set->instructions[saved_idx].transition_command =
                GROW_ARRAY(TransitionCommand, instruction_set->instructions[saved_idx].transition_command,
                           instruction_set->instructions[saved_idx].capacity * ARRAY_GROWTH);
            instruction_set->instructions[saved_idx].capacity *= ARRAY_GROWTH;
        }
        instruction_set->instructions[saved_idx]
            .transition_command[instruction_set->instructions[saved_idx].count - 1] = transition_comm;
        instruction_already_saved = false;
        return TM_SUCCESS;
    }
    if (!instruction_already_saved)
    {
        instruction.count += 1;
        if (instruction.count >= instruction.capacity - 1)
        {
            instruction.transition_command =
                GROW_ARRAY(TransitionCommand, instruction.transition_command, instruction.capacity * ARRAY_GROWTH);
            instruction.capacity *= ARRAY_GROWTH;
        }
        instruction.transition_command[instruction.count - 1] = transition_comm;
        instruction_set->count += 1;
        if (instruction_set->count >= instruction.capacity - 1)
        {
            instruction_set->instructions =
                GROW_ARRAY(Instruction, instruction_set->instructions, instruction_set->capacity * ARRAY_GROWTH);
            instruction_set->capacity *= ARRAY_GROWTH;
        }
        instruction_set->instructions[instruction_set->count - 1] = instruction;
        return TM_SUCCESS;
    }

    return TM_SUCCESS;
}