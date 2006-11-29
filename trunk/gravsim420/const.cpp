#ifndef CONST_CPP
#define CONST_CPP

#include "const.h"
#include <math.h>

float randf()
{
    return ((float)rand() / ((float)RAND_MAX + 1.0f))*(1.0f - 0.000001f) + 0.000001f;
}

#endif
