#ifndef turing_state_h
#define turing_state_h

#include "common.h"

typedef struct {
    int count;
    int capacity;
    char **states;
    char *current_state;
} StateSet;

int state_set_init(StateSet *state_set);

#endif