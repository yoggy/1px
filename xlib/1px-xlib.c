//
// 1px-xlib.c - Xlib version
//
// github:
//     https://github.com/yoggy/1px
//
// license:
//     Copyright (c) 2015 yoggy <yoggy0@gmail.com>
//     Released under the MIT license
//     http://opensource.org/licenses/mit-license.php
// 
//
#include <X11/Xlib.h>
#include <X11/keysym.h>
#include <stdio.h>
#include <memory.h>
#include <unistd.h>

int frame_count = 0;

int main(int argc, char *argv[]) 
{
	Display *display = NULL;
	int screen;
	Colormap colormap;
	XColor color, exact;
	Window root, w;
	XSetWindowAttributes attr;
	GC gc;
	XEvent evt, evt_expose;
	char key_buf[16];
	int len;
	KeySym keysym;

	unsigned long color_black, color_red, color_green, color_blue;

	display = XOpenDisplay(NULL);
	if (display == NULL) {
		fprintf(stderr, "cannot open display...");
		return -1;
	}

	screen = DefaultScreen(display);

	// setup color
	color_black = BlackPixel(display, screen);

	colormap = DefaultColormap(display, screen);
	XAllocNamedColor(display, colormap, "red", &color, &exact);
	color_red = color.pixel;
	XAllocNamedColor(display, colormap, "green", &color, &exact);
	color_green = color.pixel;
	XAllocNamedColor(display, colormap, "blue", &color, &exact);
	color_blue = color.pixel;

	// setup window
	root = DefaultRootWindow(display); 
	w = XCreateSimpleWindow(display, root, 100, 100, 200, 200, 5, color_black, color_black);
	XStoreName(display, w, "1px - xlib version");

	attr.backing_store = WhenMapped;
	XChangeWindowAttributes(display, w, CWBackingStore, &attr); 

	gc = XCreateGC(display, root, 0, 0); 

	XSelectInput(display, w, ExposureMask | KeyPressMask);

	// show window
	XMapWindow(display, w);

	// event loop
	while(1) {
		if (XEventsQueued(display, QueuedAfterFlush) == 0) {
			usleep( 250 * 1000);	

			// choose color
			switch(frame_count % 4) {
				case 0:
					XSetForeground(display, gc, color_red);
					break;
				case 1:
					XSetForeground(display, gc, color_green);
					break;
				case 2:
					XSetForeground(display, gc, color_blue);
					break;
				case 3:
					XSetForeground(display, gc, color_black);
					break;
			}

			// draw a 1px dot
			XDrawPoint(display, w, gc, 100, 100);
			XFlush(display);

			frame_count ++;
		}
		else {
			XNextEvent(display, &evt);
			switch(evt.type) {
				case Expose:
					break;
				case KeyPress:
					len = XLookupString(&evt.xkey, key_buf, 16, &keysym, NULL);
					if (keysym == XK_Escape) {
						goto finish;
					}
					break;
			}
		}
	}

finish:
	XDestroyWindow(display, w);
	XCloseDisplay(display);

	return 0;
}
