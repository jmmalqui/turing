#include "state.h"

#include "common.h"
#include "mem.h"
int state_set_init(StateSet *state_set)
{
    state_set->count = 0;
    state_set->capacity = 16;
    state_set->states = ALLOC_ARRAY(char *, state_set->capacity);
    state_set->current_state = NULL;

    return 0;
}