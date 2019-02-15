#include <float.h>

#include "no27/common/common.h"
#include "no27/common/math.h"

int
points_compare_asc(const struct point *left, const struct point *right) {
    int x_compare = float_compare_asc(left->x, right->x);
    if (x_compare != 0) {
        return x_compare;
    }

    return float_compare_asc(left->y, right->y);
}

int
points_compare_asc_generic(const void *left, const void *right) {
    return points_compare_asc(left, right);
}

int
random_range(int min, int max) {
    // for [min; max] -> min + rand() / (RAND_MAX / (max - min + 1) + 1)
    return min + rand() / (RAND_MAX / (max - min + 1) + 1);
}

enum line_side
point_line_side(const struct point p1, const struct point p2, const struct point p) {
    // d = (x − x1)(y2 − y1) − (y − y1)(x2 − x1)

    // for counter-clockwise
    // -1 = left side
    //  0 = on the line
    // +1 = right side

    float d = (p.x - p1.x) * (p2.y - p1.y) - (p.y - p1.y) * (p2.x - p1.x);

    if (d > 0) {
        return LINE_SIDE_RIGHT;
    } else if (d < 0) {
        return LINE_SIDE_LEFT;
    } else {
        return LINE_SIDE_COLLINEAR;
    }
}

struct line_segment
    perpendicular_bisector(const struct line_segment seg) {
    // calc midpoint
    float xp = (seg.p1.x + seg.p2.x) / 2.0f;
    float yp = (seg.p1.y + seg.p2.y) / 2.0f;

    // line slope (like tangent)
    float m1 = (seg.p2.y - seg.p1.y) / (seg.p2.x - seg.p1.x);

    float x1;
    float x2;
    float y1;
    float y2;

    if (m1 == 0) {
        x1 = xp;
        y1 = FLT_MAX;

        x2 = xp;
        y2 = FLT_MIN;
    } else {
        // line perpendicular slope
        float m2 = -1 / m1;

        // formula y = m2(x−xp) + yp;
        x1 = FLT_MIN;
        x2 = FLT_MAX;
        y1 = m2 * (x1 - xp) + yp;
        y2 = m2 * (x2 - xp) + yp;
    }

    struct line_segment bisector = {
        { x1, y1 },
        { x2, y2 }
    };

    return bisector;
}