#include "tape.h"
#include "common.h"

void
tape_init(Tape* tape)
{
    tape->pointer = 0;
    tape->count = 0;
    tape->capacity = 0;
    tape->data = ALLOC_ARRAY(char, tape->capacity);
}