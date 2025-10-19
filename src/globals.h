#pragma once

#include <genesis.h>

typedef struct
{
    u16 joy;
    u16 changed;
    u16 state;
} InputType;

extern InputType input;