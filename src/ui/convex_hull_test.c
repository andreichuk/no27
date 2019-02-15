#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "no27/common/memory.h"
#include "no27/voronoi/convex_hull.h"

#include "convex_hull_test.h"

static void
copy_points(struct point **dst_points, const struct point *src_points, const size_t points_count) {
    size_t size = points_count * sizeof(struct point);
    *dst_points = no27_malloc(size);
    memcpy(*dst_points, src_points, size);
}

static void
points_generate(struct point *points, size_t points_count, int point_min_x, int point_max_x, int point_min_y, int point_max_y) {
    // simple generator of unique points
    for (size_t index = 0; index < points_count; index += 1) {
        struct point *point = points + index;
        bool unique;
        do {
            unique = true;
            point->x = (float)random_range(point_min_x, point_max_x);
            point->y = (float)random_range(point_min_y, point_max_y);

            for (size_t inner_index = 0; inner_index < index; inner_index += 1) {
                struct point *prev_point = points + inner_index;
                if (prev_point->x == point->x && prev_point->y == point->y) {
                    unique = false;
                    break;
                }
            }
        } while (!unique);
    }
}

static void
draw_logic(const struct graphics_ctx *graphics_ctx) {
    struct convex_hull convex_hull;
    struct convex_hull_point *point_current;
    struct line_segment line_segment;

    const short padding = 20;
    const int point_min_x = padding;
    const int point_min_y = padding;
    int point_max_x = graphics_ctx->window_ctx.size.width - padding;
    int point_max_y = graphics_ctx->window_ctx.size.height - padding;

    struct point original_points[2000];
    size_t points_count = sizeof(original_points) / sizeof(original_points[0]);
    points_generate(original_points, points_count, point_min_x, point_max_x, point_min_y, point_max_y);

    struct point *points = NULL;

    copy_points(&points, original_points, points_count);

    convex_hull = convex_hull_create(points, points_count);
    point_current = convex_hull.leftmost_point;

    if (point_current->next_clockwise != point_current) {
        do {
            line_segment.p1 = *point_current->point;
            line_segment.p2 = *point_current->next_clockwise->point;

            line_segments_draw(graphics_ctx, &line_segment, 1);

            point_current = point_current->next_clockwise;

        } while (point_current != convex_hull.leftmost_point);
    }

    points_draw(graphics_ctx, points, points_count);

    convex_hull_dispose(&convex_hull);
    no27_free(points);
}

void
convex_hull_test_gui(void) {
    struct size window_size = { 1400, 800 };
    struct position window_position = { 50, 50 };
    struct graphics_ctx graphics_ctx;
    enum error error = gui_init(&graphics_ctx, window_size, window_position);
    if (error != E_SUCCESS) {
        return;
    }

    gui_loop(&graphics_ctx, draw_logic);

    gui_dispose(&graphics_ctx);
}

void
convex_hull_test_console(void) {
    struct convex_hull convex_hull;

    const int point_min_x = 0;
    const int point_min_y = 0;
    const int point_max_x = 5000;
    const int point_max_y = 5000;

    struct point original_points[10000];
    size_t points_count = sizeof(original_points) / sizeof(original_points[0]);
    points_generate(original_points, points_count, point_min_x, point_max_x, point_min_y, point_max_y);

    struct point *points = NULL;

    for (size_t index = 0; index < 10000; index += 1) {
        copy_points(&points, original_points, points_count);

        if (index % 100 == 0) {
            printf("%zi\n", index);
        }

        convex_hull = convex_hull_create(points, points_count);
        convex_hull_dispose(&convex_hull);

        no27_free(points);
    }
}
