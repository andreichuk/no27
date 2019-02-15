#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "no27/voronoi/convex_hull.h"
#include "no27/graphics/graphics.h"

#include "convex_hull_test.h"

static void
points_print(const struct point *points, const size_t points_count) {
    for (size_t index = 0; index < points_count; index += 1) {
        printf("%.0f; %.0f;", (points + index)->x, (points + index)->y);
        printf("\n");
    }
}

int
main(void) {
    convex_hull_test_console();
    return 0;
}