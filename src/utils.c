#include "utils.h"

#include "common.h"
#include "memory.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

int get_state_number(char* string)
{
    char* p = string;
    int   state_number;
    while (*p) {
        if (isdigit(*p) || (*p == '-' || *p == '+') && isdigit(*(p + 1))) {
            state_number = (int)strtol(p, &p, 10);
        } else {
            p++;
        }
    }
    return state_number;
}
