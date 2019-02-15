#pragma once

#include <stdlib.h>

enum error {
    E_SUCCESS,

    E_DRAWINITFAIL,
    E_DRAWWINDOWFAIL
};

int
float_compare_asc(float, float);