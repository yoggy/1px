//
// 1px-cogl-01.c - 1px cogl version
//
// reference:
//     https://github.com/GNOME/cogl/tree/master/examples
//
// github:
//     https://github.com/yoggy/1px
//
// license:
//     Copyright (c) 2017 yoggy <yoggy0@gmail.com>
//     Released under the MIT license
//     http://opensource.org/licenses/mit-license.php;
//
#include <cogl/cogl.h>
#include <glib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>

CoglContext     *ctx      = NULL;
CoglFramebuffer *fb       = NULL;
CoglPipeline    *pipeline = NULL;

CoglPrimitive   *primitives[4];

unsigned int redraw_idle  = 0;
CoglBool is_dirty         = FALSE;
CoglBool frame_sync_ready = FALSE;

uint64_t st;

uint64_t get_current_millis()
{
    struct timespec spec;
    clock_gettime(CLOCK_MONOTONIC, &spec);

    uint64_t t;
    t  = spec.tv_sec * 1000;
    t += (spec.tv_nsec / 1000000);

    return t;
}

uint64_t get_duration()
{
    uint64_t d = get_current_millis() - st;
    return d;
}

void clear_flag() 
{
    redraw_idle      = 0;
    is_dirty         = FALSE;
    frame_sync_ready = FALSE;
}

static CoglBool paint_cb(void *user_data)
{
    clear_flag();

    cogl_framebuffer_clear4f(fb, COGL_BUFFER_BIT_COLOR, 0, 0, 0, 1);

    uint32_t idx = (uint32_t)((get_duration() / 250) % 4);
    printf("idx=%d\n", idx);
    cogl_primitive_draw(primitives[idx], fb, pipeline);

    cogl_onscreen_swap_buffers(fb);

    return FALSE; // un-register idle callback
}

static void queue_redraw()
{
    if (is_dirty && frame_sync_ready && redraw_idle == 0) {
        redraw_idle = g_idle_add(paint_cb, NULL);
    }
}

static void frame_event_cb(CoglOnscreen *onscreen, CoglFrameEvent event, CoglFrameInfo *info, void *user_data)
{
    if (event == COGL_FRAME_EVENT_SYNC) {
        frame_sync_ready = TRUE;
        queue_redraw();
    }
}

static void dirty_cb(CoglOnscreen *onscreen, const CoglOnscreenDirtyInfo *info, void *user_data)
{
    is_dirty = TRUE;
    queue_redraw();
}

static gboolean timeout_cb(void *user_data) 
{
    is_dirty = TRUE;
    queue_redraw();

    return TRUE; // periodic
}

int main (int argc, char **argv)
{
    CoglError *err = NULL;
    GSource   *cogl_source;
    GMainLoop *loop;

    clear_flag();

    ctx = cogl_context_new(NULL, &err);
    if (!ctx) {
        g_error ("cogl_context_new() failed: %s\n", err->message);
    }

    fb = cogl_onscreen_new(ctx, 1280, 720);
    cogl_onscreen_show(fb);

    pipeline = cogl_pipeline_new (ctx);

    CoglVertexP2C4 vertices_red[]   = {{0, 0, 0xff, 0x00, 0x00, 0xff}};
    CoglVertexP2C4 vertices_green[] = {{0, 0, 0xff, 0xff, 0x00, 0xff}};
    CoglVertexP2C4 vertices_blue[]  = {{0, 0, 0xff, 0x00, 0xff, 0xff}};
    CoglVertexP2C4 vertices_black[] = {{0, 0, 0xff, 0x00, 0x00, 0xff}};

    primitives[0] = cogl_primitive_new_p2c4(ctx, COGL_VERTICES_MODE_POINTS, 1, vertices_red);
    primitives[1] = cogl_primitive_new_p2c4(ctx, COGL_VERTICES_MODE_POINTS, 1, vertices_green);
    primitives[2] = cogl_primitive_new_p2c4(ctx, COGL_VERTICES_MODE_POINTS, 1, vertices_blue);
    primitives[3] = cogl_primitive_new_p2c4(ctx, COGL_VERTICES_MODE_POINTS, 1, vertices_black);

    cogl_source = cogl_glib_source_new(ctx, G_PRIORITY_DEFAULT);
    g_source_attach(cogl_source, NULL);

    cogl_onscreen_add_frame_callback(fb, frame_event_cb, NULL, NULL);
    cogl_onscreen_add_dirty_callback(fb, dirty_cb, NULL, NULL);

    loop = g_main_loop_new(NULL, FALSE);

    g_timeout_add(33, timeout_cb, loop);
    g_idle_add(paint_cb, NULL);

    st = get_current_millis();
    g_main_loop_run (loop);

    return 0;
}

