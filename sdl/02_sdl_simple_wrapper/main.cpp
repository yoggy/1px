#include "Draw1pxWindow.h"
#include <string>
#include <iostream>

Draw1pxWindow *win = NULL;
bool fullscreen_flag = false;

void draw()
{
	win->draw();
}

void mainloop()
{
	SDL_Event evt;
	bool loop_flag = true;
	while (loop_flag) {
		while (SDL_PollEvent(&evt)) {
			switch (evt.type){
			case SDL_KEYUP:
				switch (evt.key.keysym.sym) {
				case SDLK_ESCAPE:
					// press esc key to quit
					loop_flag = false;
					break;
				case SDLK_f:
					// toggle fullscreen mode
					win->set_fullscreen(!win->get_fullscreen());
					break;
				default:
					break;
				}
				break;
			case SDL_QUIT:
				loop_flag = false;
				break;
			default:
				break;
			}
		}
		draw();
	}
}

bool init_sdl()
{
	bool rv;
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		std::cout << "errror: SDL_Init() : " << SDL_GetError() << std::endl;
		return false;
	}

	win = new Draw1pxWindow();
	rv = win->create("1px - 02_simple_wrapper", 100, 100, 640, 480);
	if (rv == false) {
		std::cout << "errror: SDL_Init() : " << SDL_GetError() << std::endl;
		delete win;
		win = NULL;
		return false;
	}
	return true;
}

void finish_sdl()
{
	win->close();
	delete win;
	win = NULL;

	SDL_Quit();
}

int main(int argc, char* argv[])
{
	if (init_sdl() == false) return -1;
	mainloop();
	finish_sdl();

	return 0;
}

