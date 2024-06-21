#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdbool.h"
#include "turing.h"
#define MAX_STATE_FUNC_LENGTH 4096
/*
Input:
    string - string to be worked on
    tmfile - file containing the turing machine commands
*/

// void init_tm(turing_machine_t* machine, char* tm_file) {
//     FILE* fptr;
//     fptr = fopen(tm_file, "r");
//     if (!fptr) {
//         printf("Error while loading file\n");
//         return NULL;
//     }

//     char* state_func;
//     state_func = malloc(MAX_STATE_FUNC_LENGTH * sizeof(char));

//     // strtok_r is better than strtok when doing nested tokenization
//     while (fgets(state_func, MAX_STATE_FUNC_LENGTH, fptr) != 0) {
//         // For splitting each line as follows:
//         // q0 123|4,R q1 -> (q0),(123|4,R),(q1)
//         char* space_tokdel_saveptr = NULL;

//         // For splitting the second token as follows:
//         // 123|4,R -> (123),(4,R)
//         char* line_tokdel_saveptr = NULL;

//         char* token = strtok_r(state_func, " ", &space_tokdel_saveptr);

//         int idx = 0;

//         instruction_t* instruction;

//         while (token) {
//             switch (idx) {
//                 case 0:
//                 case 1:
//                     char* subtok = strtok_r(token, "|",
//                     &line_tokdel_saveptr); while (subtok) {
//                         subtok = strtok_r(NULL, "|", &line_tokdel_saveptr);
//                     }
//                 case 2:
//             }
//             token = strtok_r(NULL, " ", &space_tokdel_saveptr);
//             idx += 1;
//         }
//     }
//     fclose(fptr);
// }

int main(int argc, char const* argv[]) {
    char* input;
    char* filename;
    for (int i = 1; i < argc; i++) {
        size_t argv_size =
            strlen(argv[i]) * sizeof(char);  // although char is always one byte
        switch (i) {
            case 1:
                input = malloc(argv_size + 1);
                memcpy(input, argv[i], argv_size);
            case 2:
                filename = malloc(argv_size + 1);
                memcpy(filename, argv[i], argv_size);
        }
    }
    printf("Arguments copied succesfully\n");
    TuringMachine* machine = tm_from_file_init(filename);
    if (!machine) {
        printf("Machine generation fail.\n");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
