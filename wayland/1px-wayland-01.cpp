//
// 1px-wayland-01.c - 1px wayland version
//
// reference:
//     https://github.com/wayland-project/weston/blob/master/clients/simple-shm.c
//
// github:
//     https://github.com/yoggy/1px
//
// license:
//     Copyright (c) 2017 yoggy <yoggy0@gmail.com>
//     Released under the MIT license
//     http://opensource.org/licenses/mit-license.php;
//
#include <stdio.h>
#include <wayland-client.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#include <memory.h>
#include <time.h>

#include <string>

struct wl_display       *display       = NULL;
struct wl_compositor    *compositor    = NULL;
struct wl_surface       *surface       = NULL;
struct wl_shell         *shell         = NULL;
struct wl_shell_surface *shell_surface = NULL;
struct wl_shm           *shm           = NULL;
struct wl_buffer        *buffer        = NULL;

void *shm_data = NULL;

int WIDTH  = 200;
int HEIGHT = 200;

uint64_t st;

uint64_t get_current_millis()
{
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);

    uint64_t t;
    t  = spec.tv_sec * 1000;
    t += (spec.tv_nsec / 1000000);

    return t;
}

static void handle_ping(void *data, struct wl_shell_surface *shell_surface, uint32_t serial)
{
    wl_shell_surface_pong(shell_surface, serial);
}

static void handle_configure(void *data, struct wl_shell_surface *shell_surface, uint32_t edges, int32_t width, int32_t height)
{
}

static void handle_popup_done(void *data, struct wl_shell_surface *shell_surface)
{
}

static const struct wl_shell_surface_listener shell_surface_listener = {
    handle_ping,
    handle_configure,
    handle_popup_done
};

int os_create_anonymous_file(off_t size)
{
    int fd;
    long flags;

    const char *path = getenv("XDG_RUNTIME_DIR");
    if (!path) {
        errno = ENOENT;
        return -1;
    }

    std::string tmpname = "";
    tmpname += path;
    tmpname += "/weston-shared-XXXXXX";

    fd = mkstemp((char*)tmpname.c_str());
    if (fd >= 0) {
        flags = fcntl(fd, F_GETFD);
        fcntl(fd, F_SETFD, flags | FD_CLOEXEC);
        unlink(tmpname.c_str());
    }
    else {
        fprintf(stderr, "mkstemp() failed: %s\n", tmpname.c_str());
        exit(1);
        close(fd);
        return -1;
    }
    if (ftruncate(fd, size) != 0 ) {
        fprintf(stderr, "ftruncate() failed: %s\n", tmpname.c_str());
        close(fd);
        return -1;
    }

    return fd;
}

static struct wl_buffer * create_buffer() {
    struct wl_shm_pool *pool;
    int stride = WIDTH * 4;
    int size = stride * HEIGHT;
    int fd;
    struct wl_buffer *buff;

    fd = os_create_anonymous_file(size);
    if (fd < 0) {
        fprintf(stderr, "os_create_anonymouse_file() failed: %m\n", size);
        exit(1);
    }

    shm_data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shm_data == MAP_FAILED) {
        fprintf(stderr, "mmap() failed: %m\n");
        close(fd);
        exit(1);
    }

    pool = wl_shm_create_pool(shm, fd, size);
    buff = wl_shm_pool_create_buffer(pool, 0, WIDTH, HEIGHT, stride, WL_SHM_FORMAT_XRGB8888);

    wl_shm_pool_destroy(pool);

    return buff;
}

static void shm_format(void *data, struct wl_shm *wl_shm, uint32_t format)
{
}

struct wl_shm_listener shm_listener = {
    shm_format
};

static void global_registry_handler(void *data, struct wl_registry *registry, uint32_t id, const char *interface, uint32_t version)
{
    std::string interface_str = interface;
    if (interface_str == "wl_compositor") {
        compositor = (wl_compositor*)wl_registry_bind(registry, id, &wl_compositor_interface, 1);
    }
    else if (interface_str == "wl_shell") {
        shell = (wl_shell*)wl_registry_bind(registry, id, &wl_shell_interface, 1);
    }
    else if (interface_str == "wl_shm") {
        shm = (wl_shm*)wl_registry_bind(registry, id, &wl_shm_interface, 1);
        wl_shm_add_listener(shm, &shm_listener, NULL);
    }
}

static void global_registry_remover(void *data, struct wl_registry *registry, uint32_t id)
{
}

static const struct wl_registry_listener registry_listener = {
    global_registry_handler,
    global_registry_remover
};

static void frame_redraw_cb(void* data, wl_callback* callback, uint32_t time);
static wl_callback_listener frame_listeners = {
    frame_redraw_cb,
};

static void frame_redraw_cb(void* data, wl_callback* callback, uint32_t time)
{
    const uint32_t colors[] = {0xff000000, 0xffff0000, 0xff00ff00, 0xff0000ff}; // ARGB
    static int idx = 0;
    uint32_t *p;

    // background color
    p = (uint32_t*)shm_data;
    memset(p, 0, WIDTH * 4 * HEIGHT);

    // draw pixel
    int x = WIDTH / 2;
    int y = HEIGHT / 2;
    p = (uint32_t*)shm_data + x + (y * WIDTH);
    *p = colors[idx];

    // change color
    uint64_t et   = get_current_millis();
    uint64_t diff = et - st;
    if (diff > 250) {
        idx = (idx + 1) % 4;
        st = et;
    }

    wl_surface_attach(surface, buffer, 0, 0);
    wl_surface_damage(surface, 0, 0, WIDTH, HEIGHT);
    if (callback) wl_callback_destroy(callback);

    callback = wl_surface_frame(surface);
    wl_callback_add_listener(callback, &frame_listeners, NULL);
    wl_surface_commit(surface); 
}

int main(int argc, char **argv)
{
    st = get_current_millis();

    display = wl_display_connect(NULL);

    struct wl_registry *registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, NULL);

    wl_display_dispatch(display);
    wl_display_roundtrip(display);

    surface = wl_compositor_create_surface(compositor);
    shell_surface = wl_shell_get_shell_surface(shell, surface);
    wl_shell_surface_set_toplevel(shell_surface);
    wl_shell_surface_add_listener(shell_surface, &shell_surface_listener, NULL);

    // create window
    buffer = create_buffer();

    wl_surface_attach(surface, buffer, 0, 0);
    wl_callback* callback = wl_surface_frame(surface);
    wl_callback_add_listener(callback, &frame_listeners, NULL);
    wl_surface_damage(surface, 0, 0, WIDTH, HEIGHT);  
    wl_surface_commit(surface);

    while (wl_display_dispatch(display) != -1) {
    }

    wl_shell_destroy(shell);
    wl_buffer_destroy(buffer);
    wl_shm_destroy(shm);
    wl_compositor_destroy(compositor);
    wl_display_disconnect(display);

    exit(0);
}

