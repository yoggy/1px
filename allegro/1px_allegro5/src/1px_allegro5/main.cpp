#include <stdio.h>
#include <allegro5/allegro.h>

int main()
{
	al_init();
	al_install_keyboard();
	al_install_mouse();

	ALLEGRO_DISPLAY* display = al_create_display(200, 200);
	al_set_window_title(display, "1px_allegro5");

	ALLEGRO_TIMER* timer = al_create_timer(0.250f); // 250ms
	if (!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}
	al_start_timer(timer);

	ALLEGRO_EVENT_QUEUE* queue = al_create_event_queue();
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_mouse_event_source());
	al_register_event_source(queue, al_get_timer_event_source(timer)); // see also...https://www.allegro.cc/forums/thread/616511
	
	ALLEGRO_COLOR color_red   = al_map_rgba_f(1.0f, 0.0f, 0.0f, 1.0f);
	ALLEGRO_COLOR color_green = al_map_rgba_f(0.0f, 1.0f, 0.0f, 1.0f);
	ALLEGRO_COLOR color_blue  = al_map_rgba_f(0.0f, 0.0f, 1.0f, 1.0f);
	ALLEGRO_COLOR color_black = al_map_rgba_f(0.0f, 0.0f, 0.0f, 1.0f);

	ALLEGRO_COLOR colors[4] = { color_red, color_green, color_blue, color_black};
	int color_idx = 0;

	bool running = true;
	while (running)
	{
		// process events
		ALLEGRO_EVENT ev;
		while (al_get_next_event(queue, &ev))
		{
			if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
				running = false;
			}
			else if (ev.type == ALLEGRO_EVENT_KEY_UP && ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
				running = false;
			}
			else if (ev.type == ALLEGRO_EVENT_TIMER) {
				color_idx = (color_idx == 3) ? 0 : (color_idx + 1);
			}
		}

		// draw
		al_clear_to_color(color_black);
		al_draw_pixel(100.0f, 100.0f, colors[color_idx]);

		// swap buffers
		al_flip_display();
	}

	al_destroy_timer(timer);
	al_destroy_event_queue(queue);
	al_destroy_display(display);
}
