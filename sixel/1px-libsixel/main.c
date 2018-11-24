//
// 1px-libsixel.c  - libsixel version
//
// reference:
//     libsixel
//     https://github.com/saitoha/libsixel
//
// github :
//     https ://github.com/yoggy/1px
//
// license :
//     Copyright(c) 2015 yoggy <yoggy0@gmail.com>
//     Released under the MIT license
//     http://opensource.org/licenses/mit-license.php;
//
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sixel/sixel.h>
#include <termios.h>
#include <sys/select.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>

void update_pixbuf(uint8_t *pixbuf, int w, int h)
{
	int x = 100, y = 100;
	int pixbuf_size = w * h * 3;

	memset(pixbuf, 0, pixbuf_size);

	struct timeval t;
	gettimeofday(&t, NULL);
	int c = t.tv_usec / 1000 / 250;

	uint8_t r, g, b;

	switch(c) {
		case 0:
			r = 255; g = 0; b = 0;
			break;
		case 1:
			r = 0; g = 255; b = 0;
			break;
		case 2:
			r = 0; g = 0; b = 255;
			break;
		case 3:
			r = 0; g = 0; b = 0;
			break;
	}

	pixbuf[x * 3 + y * w * 3 + 0] = r;
	pixbuf[x * 3 + y * w * 3 + 1] = g;
	pixbuf[x * 3 + y * w * 3 + 2] = b;
}

static int sixel_write(char *data, int size, void *priv)
{
	return fwrite(data, 1, size, (FILE *)priv);
}

int main(int argc, char *argv[])
{
	SIXELSTATUS status;
	sixel_output_t *context  = NULL;
	sixel_dither_t *dither   = NULL;

	uint8_t *pixbuf = NULL;
	int w = 200, h = 200;
	int pixbuf_size = w * h * 3;

	pixbuf = (uint8_t*)malloc(pixbuf_size);
	update_pixbuf(pixbuf, w, h);

	status = sixel_output_new(&context, sixel_write, stdout, NULL);
	if (SIXEL_FAILED(status)) {
		printf("ERROR: sixel_output_new() failed...\n");
		exit(1);
	}

	while(1) {
		update_pixbuf(pixbuf, w, h);

		// create dither data (pallet)
		status = sixel_dither_new(&dither, 256, NULL);
		status = sixel_dither_initialize(
				dither,
				pixbuf,
				w, h,
				SIXEL_PIXELFORMAT_RGB888,
				SIXEL_LARGE_AUTO,
				SIXEL_REP_AUTO,
				SIXEL_QUALITY_AUTO);

		// move cursor to top-left
		printf("\033[H\0337"); 

		// output sixel data to stdout
		status = sixel_encode(pixbuf, w, h, SIXEL_PIXELFORMAT_RGB888, dither, context);
		if (SIXEL_FAILED(status)) {
			printf("ERROR: sixel_encode() failed...\n");
			exit(1);
		}

		// free dither data
		sixel_dither_unref(dither);
	}

	sixel_output_unref(context);

	return 0;
}

/* vim : set ai si expandtab ts=4 sw=4 */
