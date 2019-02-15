#pragma once

struct point {
    float x;
    float y;
};

struct line_segment {
    struct point p1;
    struct point p2;
};

enum line_side { LINE_SIDE_LEFT, LINE_SIDE_COLLINEAR, LINE_SIDE_RIGHT };

int
random_range(int, int);

int
points_compare_asc(const struct point *, const struct point *);

int
points_compare_asc_generic(const void *, const void *);

enum line_side
point_line_side(const struct point, const struct point, const struct point);


struct line_segment
perpendicular_bisector(const struct line_segment);