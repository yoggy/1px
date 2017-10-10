#define LINUX
#define EGL_API_FB
#define WL_EGL_PLATFORM

#include <stdio.h>
#include <wayland-client.h>
#include <wayland-cursor.h>
#include <wayland-egl.h>
#include <EGL/egl.h>
#include <GLES2/gl2.h>

#include <linux/input.h>

#include <poll.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#include <memory.h>
#include <time.h>

#include <string>

struct wl_display       *display       = NULL;
struct wl_registry      *registry      = NULL;
struct wl_compositor    *compositor    = NULL;
struct wl_surface       *surface       = NULL;
struct wl_shell         *shell         = NULL;
struct wl_shell_surface *shell_surface = NULL;
struct wl_shm           *shm           = NULL;
struct wl_seat          *seat          = NULL;
struct wl_keyboard      *keyboard      = NULL;

EGLDisplay     egl_display;
EGLContext     egl_context;
EGLConfig      egl_config;
EGLSurface     egl_surface;
wl_egl_window* egl_window = NULL;

bool break_flag = false;

int WIDTH  = 200;
int HEIGHT = 200;

uint64_t st;

void draw_gl();

uint64_t get_current_millis()
{
    struct timespec spec;
    clock_gettime(CLOCK_MONOTONIC, &spec);

    uint64_t t;
    t  = spec.tv_sec * 1000;
    t += (spec.tv_nsec / 1000000);

    return t;
}

////////////////////////////////////////////////////////////////////////////////////////

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

static void keyboard_handle_keymap(void *data, struct wl_keyboard *keyboard, uint32_t format, int fd, uint32_t size)
{
}

static void keyboard_handle_enter(void *data, struct wl_keyboard *keyboard, uint32_t serial, struct wl_surface *surface, struct wl_array *keys)
{
}

static void keyboard_handle_leave(void *data, struct wl_keyboard *keyboard, uint32_t serial, struct wl_surface *surface)
{
}

static void keyboard_handle_key(void *data, struct wl_keyboard *keyboard, uint32_t serial, uint32_t time, uint32_t key, uint32_t state)
{
	if (key == KEY_ESC && state) {
        break_flag = true;
	}
}

static void keyboard_handle_modifiers(void *data, struct wl_keyboard *keyboard, uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group)
{
}

static const struct wl_keyboard_listener keyboard_listener = {
	keyboard_handle_keymap,
	keyboard_handle_enter,
	keyboard_handle_leave,
	keyboard_handle_key,
	keyboard_handle_modifiers,
};

static void seat_handle_capabilities(void *data, struct wl_seat *seat, uint32_t caps)
{
	if ((caps & WL_SEAT_CAPABILITY_KEYBOARD) && !keyboard) {
		keyboard = wl_seat_get_keyboard(seat);
		wl_keyboard_add_listener(keyboard, &keyboard_listener, NULL);
	} 
	else if (!(caps & WL_SEAT_CAPABILITY_KEYBOARD) && keyboard) {
		wl_keyboard_destroy(keyboard);
		keyboard = NULL;
	}
}

static const struct wl_seat_listener seat_listener = {
    seat_handle_capabilities,
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
    }
    else if (interface_str == "wl_seat") {
        seat = (wl_seat*)wl_registry_bind(registry, id, &wl_seat_interface, 1);
        wl_seat_add_listener(seat, &seat_listener, NULL);
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
    draw_gl();
}

////////////////////////////////////////////////////////////////////////////////////////

const EGLint context_attr[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE,};
const EGLint config_attr[] = {
    EGL_SURFACE_TYPE,    EGL_WINDOW_BIT,
    EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
    EGL_RED_SIZE,        8,
    EGL_GREEN_SIZE,      8,
    EGL_BLUE_SIZE,       8,
    EGL_ALPHA_SIZE,      8,
    EGL_DEPTH_SIZE,      16,
    EGL_BIND_TO_TEXTURE_RGBA, EGL_TRUE,
    EGL_NONE,
};

void setup_wl_egl()
{
    egl_display = eglGetDisplay((EGLNativeDisplayType)display);
    if (eglGetDisplay == EGL_NO_DISPLAY) {
        fprintf(stderr, "eglGetDisplay() failed...\n");
        exit(1);
    }

    EGLint major, minor;
    if (eglInitialize(egl_display, &major, &minor) != EGL_TRUE) {
        fprintf(stderr, "eglInitialize() failed...\n");
        exit(1);
    }
    printf("eglInitialize() major=%d, minor=%d\n", major, minor);

    eglBindAPI(EGL_OPENGL_API);

    EGLint num_config;
    eglChooseConfig (egl_display, config_attr, &egl_config, 1, &num_config);
    egl_context = eglCreateContext(egl_display, egl_config, EGL_NO_CONTEXT, context_attr);
    if (egl_context == EGL_NO_CONTEXT) {
        fprintf(stderr, "eglCreateContext() failed...\n");
        exit(1);
    }

    egl_window = wl_egl_window_create(surface, WIDTH, HEIGHT);
    egl_surface = eglCreateWindowSurface(egl_display, egl_config, egl_window, NULL);
    if (eglMakeCurrent(egl_display, egl_surface, egl_surface, egl_context) != EGL_TRUE) {
        fprintf(stderr, "eglMakeCurrent() failed...\n");
        exit(1);
    }
}

////////////////////////////////////////////////////////////////////////////////////////

static const GLchar *vertex_shader_source[] = {
    "attribute vec4 pos;",
    "void main(void)",
    "{",
    "  gl_Position = pos;",
    "}",
};

static const GLchar *fragment_shader_source[] = {
    "uniform vec4 color;",
    "uniform vec2 center_pos;",
    "void main(void)", 
    "{", 
    "  float d = distance(gl_FragCoord.xy, center_pos);",
    "  if (d <= 1.0) {", 
    "    gl_FragColor = color;", 
    "  } else {",
    "    gl_FragColor = vec4(0, 0, 0, 1);", 
    "  }",
    "}",
};

static const GLfloat vertex_data[] = {
    -1.0f,  1.0f, 0.0f, 1.0f,
     1.0f,  1.0f, 0.0f, 1.0f,
     1.0f, -1.0f, 0.0f, 1.0f,
    -1.0f,  1.0f, 0.0f, 1.0f,
     1.0f, -1.0f, 0.0f, 1.0f,
    -1.0f, -1.0f, 0.0f, 1.0f,
}; 
static const GLint vertex_size = 6;

GLint vertex_shader;
GLint fragment_shader;
GLint program;
GLuint vertex_buffer;
GLuint color_buffer;

GLint shader_attr_pos;
GLint shader_uniform_color;
GLint shader_uniform_center_pos;

void print_compilie_info(GLint shader)
{
    GLsizei buf_size;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH , &buf_size);
    GLchar* buf = (GLchar*)malloc(buf_size);
    GLint len;
    glGetShaderInfoLog(shader, buf_size, &len, buf);
    fprintf(stderr, "%s\n", buf);
    free(buf);
    buf = NULL;
}

void print_link_info(GLint program)
{
    GLsizei buf_size;
    glGetShaderiv(program, GL_INFO_LOG_LENGTH , &buf_size);
    GLchar* buf = (GLchar*)malloc(buf_size);
    GLint len;
    glGetProgramInfoLog(program, buf_size, &len, buf);
    fprintf(stderr, "%s\n", buf);
    free(buf);
    buf = NULL;
}

GLint load_shader(GLint type, const GLchar *source[], int source_size)
{
    int shader;
    int compiled;

    shader = glCreateShader(type);
    glShaderSource(shader, source_size, source, 0);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_FALSE) {
        print_compilie_info(shader);
        fprintf(stderr, "glCompileShader() failed...type=%s\n", 
                (type == GL_VERTEX_SHADER ? "GL_VERTEX_SHADER" : "GL_FRAGMENT_SHADER")
               );
        exit(1);
    }

    return shader;
}

void setup_shader()
{
    GLint linked;

    vertex_shader   = load_shader(GL_VERTEX_SHADER, vertex_shader_source, sizeof(vertex_shader_source)/sizeof(GLchar*));
    fragment_shader = load_shader(GL_FRAGMENT_SHADER, fragment_shader_source, sizeof(fragment_shader_source)/sizeof(GLchar*));

    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);

    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (linked == GL_FALSE) {
        print_link_info(program);
        fprintf(stderr, "glLinkProgram() failed....\n");
        exit(1);
    }
}

void setup_gl_resource()
{
    setup_shader();

    glUseProgram(program);

    // get index for uniform variables
    shader_attr_pos = glGetAttribLocation(program, "pos");
    shader_uniform_color = glGetUniformLocation(program, "color");
    shader_uniform_center_pos = glGetUniformLocation(program, "center_pos");

    // setup vertex buffer
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 4 * vertex_size, vertex_data, GL_STATIC_DRAW);
}

void teardown_gl_resource()
{
    glDeleteProgram(program);
    glDeleteBuffers(1, &vertex_buffer);
    glDeleteBuffers(1, &color_buffer);
}

void draw_gl() 
{
    printf("draw_egl() t=%lld\n", get_current_millis());

    eglMakeCurrent(egl_display, egl_surface, egl_surface, egl_context);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(program);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glVertexAttribPointer(shader_attr_pos, 4, GL_FLOAT, GL_FALSE, 0, NULL);

    int idx = (get_current_millis() / 250) % 4;
    switch(idx) {
        case 0:
            glUniform4f(shader_uniform_color, 1.0, 0.0, 0.0, 1.0); // RGBA
            break;
        case 1:
            glUniform4f(shader_uniform_color, 0.0, 1.0, 0.0, 1.0); // RGBA
            break;
        case 2:
            glUniform4f(shader_uniform_color, 0.0, 0.0, 1.0, 1.0); // RGBA
            break;
        case 3:
            glUniform4f(shader_uniform_color, 0.0, 0.0, 0.0, 1.0); // RGBA
            break;
    }

    glUniform2f(shader_uniform_center_pos, WIDTH/2, HEIGHT/2); // RGBA

    glDrawArrays(GL_TRIANGLES, 0, vertex_size);

    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    eglSwapBuffers(egl_display, egl_surface);
}

////////////////////////////////////////////////////////////////////////////////////////

void setup() 
{
    display = wl_display_connect(NULL);

    registry = wl_display_get_registry(display);
    wl_registry_add_listener(registry, &registry_listener, NULL);

    wl_display_dispatch(display);
    wl_display_roundtrip(display);

    surface = wl_compositor_create_surface(compositor);
    shell_surface = wl_shell_get_shell_surface(shell, surface);
    wl_shell_surface_set_toplevel(shell_surface);
    wl_shell_surface_add_listener(shell_surface, &shell_surface_listener, NULL);

    setup_wl_egl();
    setup_gl_resource();

    wl_callback* callback = wl_surface_frame(surface);
    wl_callback_add_listener(callback, &frame_listeners, NULL);
    wl_surface_damage(surface, 0, 0, WIDTH, HEIGHT);  
    wl_surface_commit(surface);
}

void teardown()
{
    teardown_gl_resource();

    if (seat) {
        wl_seat_destroy(seat);
        seat = NULL;
    }
    if (shm) {
        wl_shm_destroy(shm);
        shm = NULL;
    }

    if (egl_surface) {
        eglMakeCurrent(egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        eglDestroySurface(egl_context, egl_surface);
        wl_egl_window_destroy(egl_window);
        egl_surface = NULL;
        egl_window  = NULL;
    }
    if (egl_context) {
        eglDestroyContext(egl_display, egl_context);
        egl_context = NULL;
    }
    if (egl_display) {
        eglTerminate(egl_display);
        egl_display = NULL;
    }

    if (registry) {
        wl_registry_destroy(registry);
        registry = NULL;
    }
    if (shell) {
        wl_shell_destroy(shell);
        shell = NULL;
    }
    if (compositor) {
        wl_compositor_destroy(compositor);
        compositor = NULL;
    }
    if (display) {
        wl_display_disconnect(display);
        display = NULL;
    }
}

int main(int argc, char **argv)
{
    setup();

    pollfd fds[] = {{wl_display_get_fd(display), POLLIN},};
    while(break_flag == false) {
        while(wl_display_prepare_read(display) != 0 ) {
            wl_display_dispatch_pending(display);
        }

        wl_display_flush(display);
        if (poll(fds, 1, 0) > 0) {
            wl_display_read_events(display);
            wl_display_dispatch_pending(display);
        } else
        {
            wl_display_cancel_read(display);
        }

        draw_gl();
    }

    teardown();

    return 0;
}
