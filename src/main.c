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
#define UNDERBAR '_'
#define EOL '\n'

#define MOVERIGHT 'R'
#define MOVELEFT 'L'
#define HALT 'S'
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
    int          instructions_count;
    int          instructions_capacity;
    bool         halt;
    bool         reject;
    Instruction* instructions;
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
    tm->tape_capacity = 4;
}

void InitializeInstructions(TuringMachine* tm)
{
    tm->instructions_count    = 0;
    tm->instructions_capacity = 16;
    tm->instructions          = ALLOCARRAY(Instruction, tm->instructions_capacity);
}

int InitializeTuringMachine(TuringMachine* tm)
{
    tm->halt   = false;
    tm->reject = false;
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
    memmove(&instruction->transition_command[instruction->count - 1], transition, sizeof(TransitionCommand));
    free(transition);
    return EXIT_SUCCESS;
}

int LoadInstructionToMachine(TuringMachine* tm, Instruction* instruction)
{
    tm->instructions_count += 1;
    if (tm->instructions_count >= tm->instructions_capacity - 1) {
        tm->instructions_capacity *= ARRAYGROWTH;
        tm->instructions = GROWARRAY(Instruction, tm->instructions, tm->instructions_capacity);
    }

    memmove(&tm->instructions[tm->instructions_count - 1], instruction, sizeof(Instruction));
    free(instruction);
    return EXIT_SUCCESS;
}

int LoadInstruction(TuringMachine* tm, char* script_line)
{
    Instruction* instruction        = (Instruction*)malloc(sizeof(Instruction));
    instruction->count              = 0;
    instruction->capacity           = 120;
    instruction->transition_command = ALLOCARRAY(TransitionCommand, instruction->capacity);

    TransitionCommand* transition = (TransitionCommand*)malloc(sizeof(TransitionCommand));

    char* space_saveptr     = NULL;
    char* comma_saveptr     = NULL;
    char* separator_saveptr = NULL;

    char* token = __strtok_r(script_line, WHITESPACE, &space_saveptr);

    int token_idx = 0;

    bool no_instructions   = false;
    bool instruction_saved = false;

    int saved_idx = 0;

    while (token) {
        if (token_idx == 0) {
            if (tm->instructions_count == 0) {
                instruction->state_input = String(token);
                no_instructions          = true;
            } else {
                char* state_input = String(token);
                saved_idx         = GetSavedInstructionIndex(tm, state_input, &instruction_saved);
                free(state_input);
                if (!instruction_saved) {
                    instruction->state_input = String(token);
                }
            }
        }
        if (token_idx == 1) {
            char* subtoken     = __strtok_r(token, SEPARATOR, &separator_saveptr);
            int   subtoken_idx = 0;
            while (subtoken) {
                if (subtoken_idx == 0) {
                    transition->accept_string = String(subtoken);
                }
                if (subtoken_idx == 1) {
                    char* comma_token     = __strtok_r(subtoken, COMMA, &comma_saveptr);
                    int   comma_token_idx = 0;
                    while (comma_token) {
                        if (comma_token_idx == 0) {
                            transition->write = *comma_token;
                        }
                        if (comma_token_idx == 1) {
                            transition->move = *comma_token;
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
            transition->state_output = String(token);
        }
        token = __strtok_r(NULL, WHITESPACE, &space_saveptr);
        token_idx += 1;
    }
    if (no_instructions) {
        LoadTransition(instruction, transition);
        LoadInstructionToMachine(tm, instruction);
        no_instructions = false;
        return EXIT_SUCCESS;
    }

    if (instruction_saved) {
        LoadTransition(&tm->instructions[saved_idx], transition);
        instruction_saved = false;
        free(instruction->transition_command);
        free(instruction);
        return EXIT_SUCCESS;
    } else {
        LoadTransition(instruction, transition);
        LoadInstructionToMachine(tm, instruction);

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
    free(transition_function);
    return EXIT_SUCCESS;
}

bool CurrentStateSameAsInputState(TuringMachine* tm, int instruction_idx)
{
    return strcmp(tm->instructions[instruction_idx].state_input, tm->current_state) == 0;
}

bool TuringMachineShouldContinue(TuringMachine* tm)
{
    return tm->halt == false && tm->reject == false;
}

char TapeLookup(TuringMachine* tm)
{
    return tm->tape[tm->pointer];
}

bool IsUnderbarOnAcceptStr(TuringMachine* tm, int instruction_idx, int transition_idx)
{
    return strchr(tm->instructions[instruction_idx].transition_command[transition_idx].accept_string, UNDERBAR) != NULL;
}

bool IsCurrentCharOnAcceptStr(TuringMachine* tm, int instruction_idx, int transition_idx)
{
    return strchr(tm->instructions[instruction_idx].transition_command[transition_idx].accept_string, TapeLookup(tm)) != NULL && (TapeLookup(tm) != 0);
}

void ParseMoveInstruction(TuringMachine* tm, int instruction_idx, int transition_idx)
{
    switch (tm->instructions[instruction_idx].transition_command[transition_idx].move) {
    case MOVERIGHT:
        tm->pointer++;
        if (tm->pointer >= tm->tape_capacity) {
            tm->tape = GROWARRAY(char, tm->tape, tm->tape_capacity* ARRAYGROWTH);
        }
        break;
    case MOVELEFT:
        tm->pointer--;
        if (tm->pointer < 0) {
            tm->reject = true;
        }
        break;
    case HALT:
        tm->halt = true;
        break;
    default:
        break;
    }
}

void SolveMachine(TuringMachine* tm, char* input)
{
    tm->current_state = String("q0");
    tm->tape          = String(input);
    tm->tape_size     = strlen(input);

    while (TuringMachineShouldContinue(tm)) {
        for (int i = 0; i < tm->instructions_count; i++) {
            if (CurrentStateSameAsInputState(tm, i)) {
                bool instruction_found = false;
                for (int j = 0; j < tm->instructions[i].count; j++) {
                    TransitionCommand tc = tm->instructions[i].transition_command[j];
                    if (IsUnderbarOnAcceptStr(tm, i, j) || IsCurrentCharOnAcceptStr(tm, i, j)) {
                        instruction_found = true;
                        free(tm->current_state);
                        tm->current_state = String(tc.state_output);
                        if (tc.write != '/') {
                            if (tm->pointer > tm->tape_size) {
                                tm->tape_size *= ARRAYGROWTH;
                                tm->tape = GROWARRAY(char, tm->tape, tm->tape_size);
                            }
                            tm->tape[tm->pointer] = tc.write;
                        }
                        ParseMoveInstruction(tm, i, j);
                        break;
                    }
                }
                if (!instruction_found)
                    tm->reject = true;
            }
        }
    }
    if (tm->reject) {
        free(tm->tape);
        tm->tape = String("Reject");
    }
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
    printf("No arguments were given.\n");
    printf("Arguments:  ./turing <input_str>  <script_file>.\n");
}

void FreeInstruction(Instruction* instruction)
{
    free(instruction->state_input);
    for (int i = 0; i < instruction->count; i++) {
        /* printf("inst: %s\n", instruction->state_input); */
        free(instruction->transition_command[i].accept_string);
        free(instruction->transition_command[i].state_output);
    }
    free(instruction->transition_command);
}

void FreeTuringMachine(TuringMachine* tm)
{
    for (int i = 0; i < tm->instructions_count; i++) {
        FreeInstruction(&tm->instructions[i]);
    }
    free(tm->instructions);
    free(tm->current_state);
    free(tm->tape);
}

int main(int argc, char* argv[])
{
    if (argc == 1) {
        help();
        return EXIT_FAILURE;
    }
    char* input    = String(argv[1]);
    char* filename = String(argv[2]);

    printf("Input: %s\nFile: %s\n", input, filename);

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
    SolveMachine(&machine, input);
    printf("Solution: %s\n", machine.tape);
    free(input);
    free(filename);
    FreeTuringMachine(&machine);
    return EXIT_SUCCESS;
}
