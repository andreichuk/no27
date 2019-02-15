#include "no27/common/common.h"

int
float_compare_asc(float left, float right) {
    if (left > right) {
        return  1;
    } else if (left < right) {
        return -1;
    } else {
        return 0;
    }
}