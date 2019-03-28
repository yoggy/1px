#include <stdint.h>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>

#define IMAGE_W 200
#define IMAGE_H 200
#define PIXEL_STRIDE 4
#define BUF_SIZE (IMAGE_W * IMAGE_H * PIXEL_STRIDE)

uint8_t *bufs[4];
int counter = 0;

Fl_Box *b;
Fl_Window *window;
Fl_RGB_Image  *imgs[4];

void draw_pixel(uint8_t *buf, const int &x, const int &y, const uint8_t &r, const uint8_t &g, const uint8_t &b, const uint8_t &a)
{
	int offset = x * PIXEL_STRIDE + y * IMAGE_W * PIXEL_STRIDE;

	buf[offset + 0] = r;   // R
	buf[offset + 1] = g;   // G
	buf[offset + 2] = b;   // B
	buf[offset + 3] = a;   // A
}

uint8_t *create_buf()
{
	uint8_t *buf;
	buf = (uint8_t*)malloc(BUF_SIZE);

	for (int y = 0; y < IMAGE_H; ++y) {
		for (int x = 0; x < IMAGE_W; ++x) {
			draw_pixel(buf, x, y, 0, 0, 0, 255);
		}
	}
	return buf;
}

void create_images()
{
	bufs[0] = create_buf();
	bufs[1] = create_buf();
	bufs[2] = create_buf();
	bufs[3] = create_buf();

	draw_pixel(bufs[0], 100, 100, 255,   0,   0, 255);
	draw_pixel(bufs[1], 100, 100,   0, 255,   0, 255);
	draw_pixel(bufs[2], 100, 100,   0,   0, 255, 255);
	draw_pixel(bufs[3], 100, 100,   0 ,  0,   0, 255);

	imgs[0] = new Fl_RGB_Image(bufs[0], IMAGE_W, IMAGE_H, PIXEL_STRIDE);
	imgs[1] = new Fl_RGB_Image(bufs[1], IMAGE_W, IMAGE_H, PIXEL_STRIDE);
	imgs[2] = new Fl_RGB_Image(bufs[2], IMAGE_W, IMAGE_H, PIXEL_STRIDE);
	imgs[3] = new Fl_RGB_Image(bufs[3], IMAGE_W, IMAGE_H, PIXEL_STRIDE);
}

static void timer_cb(void *data)
{
	printf("counter=%d\n", counter);

	counter++;
	counter = counter % 4;

	b->image(imgs[counter]);
	b->redraw();

	// bufs[?]の中身だけを書き換えてb->redraw()しても表示は変わらないので要注意…(Fl_RGB_Imageが表示内容をキャッシュしてる?)

	Fl::repeat_timeout(0.25, timer_cb, data);
}

int main(int argc, char **argv)
{
	window = new Fl_Window(320, 240);
	window->color(FL_WHITE);

	b = new Fl_Box(10, 10, window->w() - 20, window->h() - 20);
	b->box(FL_THIN_DOWN_BOX);
	b->align(FL_ALIGN_INSIDE | FL_ALIGN_CENTER | FL_ALIGN_CLIP);
	window->resizable(b);

	create_images();

	window->end();
	window->show(argc, argv);

	Fl::add_timeout(.25, timer_cb, NULL);

	return Fl::run();
}
