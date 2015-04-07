//
// 1px-gtk.c - GTK3+ version
//
// github:
//     https://github.com/yoggy/1px
//
// license:
//     Copyright (c) 2015 yoggy <yoggy0@gmail.com>
//     Released under the MIT license
//     http://opensource.org/licenses/mit-license.php
//
// usage:
//   $ sudo apt-get install build-essential
//   $ sudo apt-get install libgtk-3-dev
//   $ make
//   $ ./1px-gtk
//
#include <gtk/gtk.h>
#include <stdlib.h>

GtkApplication *app;
GtkWidget      *window;
GtkWidget      *canvas;

int frame_count = 0;

gboolean on_draw(GtkWidget *widget, cairo_t *cr, gpointer data)
{
	guint w, h;
	w = gtk_widget_get_allocated_width(widget);
	h = gtk_widget_get_allocated_width(widget);

	// clear background
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_paint(cr);

	// choose drawing color
	switch(frame_count % 4) {
	case 0:
		cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);
		break;
	case 1:
		cairo_set_source_rgb(cr, 0.0, 1.0, 0.0);
		break;
	case 2:
		cairo_set_source_rgb(cr, 0.0, 0.0, 1.0);
		break;
	case 3:
		cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
		break;
	}

	// draw pixel
	cairo_rectangle (cr, w/2, h/2, 1, 1);
	cairo_fill(cr);
	
	frame_count ++;

	return FALSE;
}

gboolean on_timer(gpointer data)
{
	gtk_widget_queue_draw(canvas); // redraw request
	return TRUE;                   // periodic
}

gboolean on_key_press(GtkWidget *widget, GdkEventKey *evt, gpointer data)
{
	switch (evt->keyval) {
	case GDK_KEY_Escape:
		exit(0);
		break;
	default:
		return FALSE;
	}

	return TRUE;
}

void activate(GtkApplication* app, gpointer user_data)
{
	window = gtk_application_window_new(app);
	gtk_window_set_title(GTK_WINDOW (window), "1px - gtk version");

	canvas = gtk_drawing_area_new();
	gtk_widget_set_size_request(canvas, 200, 200);
	gtk_container_add(GTK_CONTAINER(window), canvas);

	g_signal_connect (G_OBJECT(canvas), "draw", G_CALLBACK(on_draw), NULL);
	gtk_widget_add_events(window, GDK_KEY_PRESS_MASK);
	g_signal_connect(G_OBJECT(window), "key-press-event", G_CALLBACK(on_key_press), NULL);

	gtk_widget_show_all (window);

	g_timeout_add(250, (GSourceFunc)on_timer, NULL);
}

int main(int argc, char **argv)
{
	int status;

	app = gtk_application_new("org.gtk.one_pixel", G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);

	status = g_application_run(G_APPLICATION (app), argc, argv);

	g_object_unref(app);
	app = NULL;

	return status;
}
