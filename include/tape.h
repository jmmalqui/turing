#ifndef turing_tape_h
#define turing_tape_h

typedef struct {
    struct Tape* left;
    struct Tape* right;
    char data;
} Tape;

void init_cell(Tape* cell, char* tm_input);

#endif