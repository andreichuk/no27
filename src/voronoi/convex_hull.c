#include "no27/common/common.h"
#include "no27/common/memory.h"
#include "no27/common/math.h"
#include "no27/voronoi/convex_hull.h"

/*
 * Algorithm "Convex Hull by Divide-and-Conquer"
 * https://www.cs.umd.edu/class/spring2012/cmsc754/Lects/cmsc754-lects.pdf
 *
 * todo: build a convex hull for 3 points using brute force to speed up the implmenentation
 */

static bool
convex_hull_collinear(const struct convex_hull *left, const struct convex_hull *right) {
    if (!left->collinear || !right->collinear) {
        return false;
    }

    if(point_line_side(*left->leftmost_point->point, *left->rightmost_point->point, *right->leftmost_point->point) != LINE_SIDE_COLLINEAR) {
        return false;
    }

    if (point_line_side(*left->rightmost_point->point, *right->leftmost_point->point, *right->rightmost_point->point) != LINE_SIDE_COLLINEAR) {
        return false;
    }

    return true;
}

static void
convex_hull_merge_upper_tangent(
    struct convex_hull_point *left_point,
    struct convex_hull_point *right_point,
    struct convex_hull_point **left_upper_point,
    struct convex_hull_point **right_upper_point)
{
    struct convex_hull_point *prev_left_point;
    struct convex_hull_point *prev_right_point;

    while (true) {
        prev_left_point = left_point;
        prev_right_point = right_point;

        if (right_point->next_clockwise != right_point)
        {
            while (
                point_line_side(
                    *(left_point->point),
                    *(right_point->next_clockwise->point),
                    *(right_point->point)
                ) == LINE_SIDE_RIGHT)
            {
                right_point = right_point->next_clockwise;
            }
        }

        if (left_point->next_counterclockwise != left_point)
        {
            while (
                point_line_side(
                    *(right_point->point),
                    *(left_point->next_counterclockwise->point),
                    *(left_point->point)
                ) == LINE_SIDE_LEFT)
            {
                left_point = left_point->next_counterclockwise;
            }
        }

        if (left_point == prev_left_point
            &&
            right_point == prev_right_point) {
            break;
        }
    }

    *left_upper_point = left_point;
    *right_upper_point = right_point;
}

static void
convex_hull_merge_lower_tangent(
    struct convex_hull_point *left_point,
    struct convex_hull_point *right_point,
    struct convex_hull_point **left_lower_point,
    struct convex_hull_point **right_lower_point)
{
    struct convex_hull_point *prev_left_point;
    struct convex_hull_point *prev_right_point;

    while (true) {
        prev_left_point = left_point;
        prev_right_point = right_point;

        if (right_point->next_counterclockwise != right_point)
        {
            while (
                point_line_side(
                    *(left_point->point),
                    *(right_point->next_counterclockwise->point),
                    *(right_point->point)
                ) == LINE_SIDE_LEFT)
            {
                right_point = right_point->next_counterclockwise;
            }
        }

        if (left_point->next_clockwise != left_point)
        {
            while (
                point_line_side(
                    *(right_point->point),
                    *(left_point->next_clockwise->point),
                    *(left_point->point)
                ) == LINE_SIDE_RIGHT)
            {
                left_point = left_point->next_clockwise;
            }
        }

        if (left_point == prev_left_point
            &&
            right_point == prev_right_point) {
            break;
        }
    }

    *left_lower_point = left_point;
    *right_lower_point = right_point;
}

static void
convex_hull_merge_free_memory(
    const struct convex_hull_point *start_point,
    const struct convex_hull_point *end_point
) {
    if (start_point->next_clockwise == start_point) {
        return;
    }

    const struct convex_hull_point *point = start_point->next_clockwise;

    while (point != end_point)
    {
        point = point->next_clockwise;
        no27_free(point->next_counterclockwise);
    }
}

struct convex_hull
convex_hull_create_from_point(const struct point * const point) {
    struct convex_hull_point *hull_point = no27_malloc(sizeof(struct convex_hull_point));
    hull_point->next_clockwise = hull_point;
    hull_point->next_counterclockwise = hull_point;
    hull_point->point = point;

    struct convex_hull convex_hull;
    convex_hull.leftmost_point = hull_point;
    convex_hull.rightmost_point = hull_point;
    convex_hull.collinear = true;

    return convex_hull;
}

static struct convex_hull
convex_hull_merge_collinear(const struct convex_hull *left_hull, const struct convex_hull *right_hull) {

    /* convex hull of collinear points is a connection between 2 most distant points */

    struct convex_hull merged_hull;
    struct convex_hull_point *left_point = left_hull->rightmost_point;
    struct convex_hull_point *right_point = right_hull->leftmost_point;

    convex_hull_merge_free_memory(left_hull->leftmost_point, left_hull->leftmost_point);
    convex_hull_merge_free_memory(right_hull->rightmost_point, right_hull->rightmost_point);

    left_hull->leftmost_point->next_clockwise = right_hull->rightmost_point;
    left_hull->leftmost_point->next_counterclockwise = right_hull->rightmost_point;

    right_hull->rightmost_point->next_clockwise = left_hull->leftmost_point;
    right_hull->rightmost_point->next_counterclockwise = left_hull->leftmost_point;

    merged_hull.leftmost_point = left_hull->leftmost_point;
    merged_hull.rightmost_point = right_hull->rightmost_point;
    merged_hull.collinear = true;

    return merged_hull;
}

static struct convex_hull
convex_hull_merge_noncollinear(const struct convex_hull *left_hull, const struct convex_hull *right_hull) {
    struct convex_hull merged_hull;
    struct convex_hull_point *left_point = left_hull->rightmost_point;
    struct convex_hull_point *right_point = right_hull->leftmost_point;

    struct convex_hull_point *left_upper_point;
    struct convex_hull_point *right_upper_point;
    struct convex_hull_point *left_lower_point;
    struct convex_hull_point *right_lower_point;

    convex_hull_merge_upper_tangent(left_hull->rightmost_point, right_hull->leftmost_point, &left_upper_point, &right_upper_point);
    convex_hull_merge_lower_tangent(left_hull->rightmost_point, right_hull->leftmost_point, &left_lower_point, &right_lower_point);

    convex_hull_merge_free_memory(left_upper_point, left_lower_point);
    convex_hull_merge_free_memory(right_lower_point, right_upper_point);

    left_upper_point->next_clockwise = right_upper_point;
    right_upper_point->next_counterclockwise = left_upper_point;

    left_lower_point->next_counterclockwise = right_lower_point;
    right_lower_point->next_clockwise = left_lower_point;

    merged_hull.leftmost_point = left_hull->leftmost_point;
    merged_hull.rightmost_point = right_hull->rightmost_point;
    merged_hull.collinear = false;

    return merged_hull;
}

struct convex_hull
convex_hull_merge(const struct convex_hull *left_hull, const struct convex_hull *right_hull)
{
    if(convex_hull_collinear(left_hull, right_hull)) {
        return convex_hull_merge_collinear(left_hull, right_hull);
    } else {
        return convex_hull_merge_noncollinear(left_hull, right_hull);
    }
}

static struct convex_hull
convex_hull_build(const struct point *points, size_t count)
{
    if (count > 1) {
        struct convex_hull left_convex_hull = convex_hull_build(points, count / 2);
        struct convex_hull right_convex_hull = convex_hull_build(points + count / 2, count - (count / 2));
        struct convex_hull merged_convex_hull = convex_hull_merge(&left_convex_hull, &right_convex_hull);
        return merged_convex_hull;
    }

    return convex_hull_create_from_point(points);
}

struct convex_hull
convex_hull_create(struct point *points, size_t points_count) {
    qsort(points, points_count, sizeof(struct point), points_compare_asc_generic);
    return convex_hull_build(points, points_count);
}

void
convex_hull_dispose(const struct convex_hull *convex_hull) {
    if(convex_hull->leftmost_point == convex_hull->rightmost_point) {
        no27_free(convex_hull->leftmost_point);
        return;
    }

    struct convex_hull_point *point = convex_hull->leftmost_point->next_clockwise;

    do {
        point = point->next_clockwise;
        no27_free(point->next_counterclockwise);

        if(point == convex_hull->leftmost_point) {
            no27_free(point);
            return;
        }
    } while (true);
}