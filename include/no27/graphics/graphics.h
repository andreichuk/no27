#pragma once

#include "no27/common/common.h"
#include "no27/common/math.h"

struct size {
    short width;
    short height;
};

struct position {
    short offset_left;
    short offset_top;
};

struct graphics_driver_ctx;

struct window_ctx {
    struct size size;
    struct position position;
};

struct graphics_ctx {
    struct window_ctx window_ctx;
    struct graphics_driver_ctx *driver_ctx;
};

void
gui_dispose(struct graphics_ctx *);

enum ERR
gui_init(struct graphics_ctx *, struct size, struct position);

void
line_segments_draw(const struct graphics_ctx *, struct line_segment *, size_t);

void
points_draw(const struct graphics_ctx *, struct point *, size_t);

void
gui_loop(struct graphics_ctx *, void (*)(const struct graphics_ctx *));