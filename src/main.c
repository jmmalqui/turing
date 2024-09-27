#include <ctype.h>
#include <memory.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void* ReallocateMemory(void* pointer, size_t size);

#define ALLOCTYPE(type) (type*)malloc(sizeof(type))
#define ALLOCARRAY(type, num) (type*)malloc((num) * sizeof(type))
#define ALLOCSTRING(num) (char*)malloc((num) + 1)
#define GROWARRAY(type, pointer, new_size) \
    (type*)ReallocateMemory(pointer, sizeof(type) * (new_size))
#define FREEARRAY(type, pointer) (type*)ReallocateMemory(pointer, 0)
#define ARRAYGROWTH 2

#define File FILE*
#define WHITESPACE " "
#define SEPARATOR "|"
#define COMMA ","
#define EOL '\n'
typedef struct turing_machine_t     TuringMachine;
typedef struct instruction_t        Instruction;
typedef struct transition_command_t TransitionCommand;

struct transition_command_t {
    char* accept_string;
    char* state_output;
    char  write;
    char  move;
};

struct instruction_t {
    char*              state_input;
    int                count;
    int                capacity;
    TransitionCommand* transition_command;
};

struct turing_machine_t {
    char*        current_state;
    char*        tape;
    int          pointer;
    int          tape_size;
    int          tape_capacity;
    Instruction* instructions;
    int          instructions_count;
    int          instructions_capacity;
};

char* String(const char* string)
{
    char* s = ALLOCSTRING(strlen(string));
    s       = strcpy(s, string);
    return s;
}

void InitializeTape(TuringMachine* tm)
{
    tm->pointer       = 0;
    tm->tape_size     = 0;
    tm->tape_capacity = 16;
    tm->tape          = ALLOCARRAY(char, tm->tape_capacity);
}

void InitializeInstructions(TuringMachine* tm)
{
    tm->instructions_count    = 0;
    tm->instructions_capacity = 16;
    tm->instructions          = ALLOCARRAY(Instruction, tm->instructions_capacity);
}

int InitializeTuringMachine(TuringMachine* tm)
{
    tm->current_state = String("q0");
    InitializeTape(tm);
    InitializeInstructions(tm);
    return 0;
}

int GetSavedInstructionIndex(TuringMachine* tm, char* state_input, bool* is_saved_flag)
{
    for (int i = 0; i < tm->instructions_count; i++) {
        if (strcmp(tm->instructions[i].state_input, state_input) == 0) {
            *is_saved_flag = true;
            return i;
        }
    }
    return NULL;
}

int LoadTransition(Instruction* instruction, TransitionCommand* transition)
{
    instruction->count += 1;
    if (instruction->count >= instruction->capacity - 1) {
        instruction->capacity *= ARRAYGROWTH;

        instruction->transition_command = GROWARRAY(TransitionCommand, instruction->transition_command, instruction->capacity);
    }
    instruction->transition_command[instruction->count - 1] = *transition;
    return EXIT_SUCCESS;
}

int LoadInstructionToMachine(TuringMachine* tm, Instruction* instruction)
{
    tm->instructions_count += 1;
    if (tm->instructions_count >= tm->instructions_capacity - 1) {
        tm->instructions_capacity *= ARRAYGROWTH;
        tm->instructions = GROWARRAY(Instruction, tm->instructions, tm->instructions_capacity);
    }
    tm->instructions[tm->instructions_count - 1] = *instruction;
    return EXIT_SUCCESS;
}

int LoadInstruction(TuringMachine* tm, char* script_line)
{
    Instruction instruction;
    instruction.count              = 0;
    instruction.capacity           = 4;
    instruction.transition_command = ALLOCARRAY(TransitionCommand, 4);

    TransitionCommand transition;

    char* space_saveptr     = NULL;
    char* comma_saveptr     = NULL;
    char* separator_saveptr = NULL;

    char* token = __strtok_r(script_line, WHITESPACE, &space_saveptr);

    int   token_idx = 0;
    char* state_input;

    bool no_instructions   = false;
    bool instruction_saved = false;

    int saved_idx = 0;

    while (token) {
        if (token_idx == 0) {
            state_input = String(token);
            if (tm->instructions_count == 0) {
                instruction.state_input = String(token);
                no_instructions         = true;
            } else {
                saved_idx = GetSavedInstructionIndex(tm, state_input, &instruction_saved);
                if (!instruction_saved) {
                    instruction.state_input = String(token);
                }
            }
        }
        if (token_idx == 1) {
            char* subtoken     = __strtok_r(token, SEPARATOR, &separator_saveptr);
            int   subtoken_idx = 0;
            while (subtoken) {
                if (subtoken_idx == 0) {
                    transition.accept_string = String(subtoken);
                }
                if (subtoken_idx == 1) {
                    char* comma_token     = __strtok_r(subtoken, COMMA, &comma_saveptr);
                    int   comma_token_idx = 0;
                    while (comma_token) {
                        if (comma_token_idx == 0) {
                            transition.write = *comma_token;
                        }
                        if (comma_token_idx == 1) {
                            transition.move = *comma_token;
                        }
                        comma_token = __strtok_r(NULL, COMMA, &comma_saveptr);
                        comma_token_idx += 1;
                    }
                }
                subtoken = __strtok_r(NULL, SEPARATOR, &separator_saveptr);
                subtoken_idx += 1;
            }
        }
        if (token_idx == 2) {
            if (*(token + strlen(token) - 1) == EOL)
                *(token + strlen(token) - 1) = 0;
            transition.state_output = String(token);
        }
        token = __strtok_r(NULL, WHITESPACE, &space_saveptr);
        token_idx += 1;
    }
    if (no_instructions) {
        LoadTransition(&instruction, &transition);
        LoadInstructionToMachine(tm, &instruction);
        no_instructions = false;
        return EXIT_SUCCESS;
    }

    if (instruction_saved) {
        LoadTransition(&tm->instructions[saved_idx], &transition);
        instruction_saved = false;
        return EXIT_SUCCESS;
    } else {
        LoadTransition(&instruction, &transition);
        LoadInstructionToMachine(tm, &instruction);
        return EXIT_SUCCESS;
    }
    return EXIT_SUCCESS;
}
int LoadTuringScript(TuringMachine* tm, char* script_path)
{
    File fptr;
    fptr = fopen(script_path, "r");

    if (!fptr) {
        printf("[ERROR] Couldnt find %s\n", script_path);
        return EXIT_FAILURE;
    }

    char* transition_function = ALLOCSTRING(4096);
    while (fgets(transition_function, 4096, fptr) != 0) {
        LoadInstruction(tm, transition_function);
    }
    fclose(fptr);
    return EXIT_SUCCESS;
}

char* SolveMachine(TuringMachine* tm, char* input)
{

    tm->tape      = String(input);
    tm->tape_size = strlen(input);
    bool reject   = false;
    bool halt     = false;
    while (reject == false && halt == false) {
        for (int i = 0; i < tm->instructions_count; i++) {
            if (strcmp(tm->instructions[i].state_input, tm->current_state) == 0) {
                bool found_instruction = false;
                for (int j = 0; j < tm->instructions[i].count; j++) {
                    bool underbar_in_accept_str         = strchr(tm->instructions[i].transition_command[j].accept_string, '_') != NULL;
                    bool is_observed_NULL               = tm->tape[tm->pointer] == 0;
                    bool observed_char_in_accept_string = (strchr(tm->instructions[i].transition_command[j].accept_string, tm->tape[tm->pointer]) != NULL) && (is_observed_NULL == false);
                    if (observed_char_in_accept_string || (underbar_in_accept_str)) {
                        found_instruction = true;
                        tm->current_state = String(tm->instructions[i].transition_command[j].state_output);
                        if (tm->instructions[i].transition_command[j].write != '/') {
                            tm->tape[tm->pointer] = tm->instructions[i].transition_command[j].write;
                        }
                        switch (tm->instructions[i].transition_command[j].move) {
                        case 'R':
                            tm->pointer += 1;
                            if (tm->pointer >= tm->tape_capacity) {
                                tm->tape = GROWARRAY(char, tm->tape, tm->tape_capacity* ARRAYGROWTH);
                            }
                            break;
                        case 'L':
                            tm->pointer -= 1;
                            if (tm->pointer < 0) {
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
                if (!found_instruction) {
                    reject = true;
                }
            }
        }
    }
    if (halt) {
        tm->tape[tm->pointer + 1] = '\0';
        return tm->tape;
    }
    char* reject_str = String("Reject");
    if (reject) {
        return reject_str;
    }
    return NULL;
}

void* ReallocateMemory(void* pointer, size_t new_size)
{
    if (new_size == 0) {
        free(pointer);
        printf("gg");
        return NULL;
    }
    void* result = realloc(pointer, new_size);
    if (!result) {
        printf("[ERROR] memory reallocation failed.\n");
        exit(1);
    }
    return result;
}

void help(void)
{
    printf("Turing Machine C\n");
}

int main(int argc, char* argv[])
{
    if (argc == 1) {
        help();
        return EXIT_FAILURE;
    }
    char* input    = String(argv[1]);
    char* filename = String(argv[2]);

    printf("%s %s\n", input, filename);

    TuringMachine machine;

    int init_state        = InitializeTuringMachine(&machine);
    int load_script_state = LoadTuringScript(&machine, filename);

    if (init_state != 0) {
        printf("[ERROR] Initialization of turing machine went wrong.\n");
        return EXIT_FAILURE;
    }
    if (load_script_state == EXIT_FAILURE) {
        printf("[ERROR] Script loading went wrong.\n");
        return EXIT_FAILURE;
    }
    printf("solving\n");
    char* solution = SolveMachine(&machine, input);
    printf("%s\n", solution);

    return EXIT_SUCCESS;
}
