#ifndef turing_tape_h
#define turing_tape_h

typedef struct
{
    char* data;
    int pointer;
    int count;
    int capacity;
} Tape;

void
tape_init(Tape* tape);

void
move_right(void);

void
overwrite_this_cell(Tape* tape, char* data);

void
tape_free(Tape* tape);
#endif