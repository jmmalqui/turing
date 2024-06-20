#ifndef CELL_H_
#define CELL_H_

typedef struct TapeCell {
    struct tape_cell_t* left;
    struct tape_cell_t* right;
    char data;
} tape_cell_t;

void init_cell(tape_cell_t* cell, char* tm_input);

#endif