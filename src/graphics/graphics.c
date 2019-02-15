#include <stdlib.h>
#include <stdio.h>
#include <GLFW/glfw3.h>

#include "no27/common/memory.h"
#include "no27/graphics/graphics.h"

struct graphics_driver_ctx {
    struct GLFWwindow *window;
};

void
gui_dispose(struct graphics_ctx *graphics_ctx) {
    glfwTerminate();
    no27_free(graphics_ctx->driver_ctx);
}

static void
key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

enum error
    gui_init(struct graphics_ctx *graphics_ctx, struct size window_size, struct position window_position) {
    enum error error;
    struct graphics_driver_ctx *driver_ctx = NULL;
    struct GLFWwindow* window;

    if (glfwInit() != GLFW_TRUE) {
        fprintf(stderr, "failed to init GLFW\n");
        error = E_DRAWINITFAIL;
        goto cleanup;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);

    window = glfwCreateWindow(window_size.width, window_size.height, "no27", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr, "failed to create a window\n");
        error = E_DRAWINITFAIL;
        goto cleanup;
    }

    glfwMakeContextCurrent(window);

    glfwSetWindowPos(window, window_position.offset_left, window_position.offset_top);

    glViewport(0, 0, window_size.width, window_size.height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, window_size.width, 0, window_size.height, 0, 1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // The swap interval indicates how many frames to wait until swapping the buffers, commonly known as vsync
    glfwSwapInterval(1);

    glfwSetKeyCallback(window, key_callback);

    driver_ctx = no27_malloc(sizeof(struct graphics_driver_ctx));

    driver_ctx->window = window;
    graphics_ctx->window_ctx.size = window_size;
    graphics_ctx->window_ctx.position = window_position;
    graphics_ctx->driver_ctx = driver_ctx;

    return E_SUCCESS;

cleanup:
    glfwTerminate();

    if (driver_ctx != NULL) {
        no27_free(driver_ctx);
        driver_ctx = NULL;
    }

    return error;
}

void
line_segments_draw(const struct graphics_ctx *ctx, struct line_segment *segments, size_t lines_count) {
    glLineWidth(2.5);
    glColor3f(1.0, 0.0, 0.0);

    glBegin(GL_LINES);

    for (size_t index = 0; index < lines_count; index += 1) {
        struct line_segment *seg = segments + index;
        glVertex3f(seg->p1.x, seg->p1.y, 0);
        glVertex3f(seg->p2.x, seg->p2.y, 0);
    }

    glEnd();
}

void
points_draw(const struct graphics_ctx *graphics_ctx, struct point *points, size_t points_count) {
    glPointSize(10);
    glColor3f(0.0, 1.0, 0.0);

    glBegin(GL_POINTS);

    for (size_t index = 0; index < points_count; index += 1) {
        struct point *point = points + index;
        glVertex3f(point->x, point->y, 0);
    }

    glEnd();
}

static void
draw_action(struct graphics_ctx *graphics_ctx, void(*draw_logic)(const struct graphics_ctx *)) {
    glClear(GL_COLOR_BUFFER_BIT);
    draw_logic(graphics_ctx);
    glfwSwapBuffers(graphics_ctx->driver_ctx->window);
}

void
gui_loop(struct graphics_ctx *graphics_ctx, void(*draw_logic)(const struct graphics_ctx *)) {
    draw_action(graphics_ctx, draw_logic);

    while (!glfwWindowShouldClose(graphics_ctx->driver_ctx->window)) {
        int action = glfwGetKey(graphics_ctx->driver_ctx->window, GLFW_KEY_ENTER);

        if (action == GLFW_PRESS) {
            draw_action(graphics_ctx, draw_logic);
        }

        glfwWaitEvents();
    }
}