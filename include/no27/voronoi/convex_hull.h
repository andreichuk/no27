#pragma once

#include <stdlib.h>
#include <stdbool.h>

#include "math.h"

struct convex_hull_point {
    const struct point *point;
    struct convex_hull_point *next_counterclockwise;
    struct convex_hull_point *next_clockwise;
};

struct convex_hull {
    struct convex_hull_point *leftmost_point;
    struct convex_hull_point *rightmost_point;
    bool collinear;
};

struct convex_hull
convex_hull_create_from_point(const struct point * const);

struct convex_hull
convex_hull_merge(const struct convex_hull *, const struct convex_hull *);

struct convex_hull
convex_hull_create(struct point *, size_t);

void
convex_hull_dispose(const struct convex_hull *);