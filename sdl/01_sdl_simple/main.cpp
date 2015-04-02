#include <SDL.h>

#include <string>
#include <iostream>

#ifdef WIN32
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#endif

SDL_Window *win = nullptr;
SDL_Renderer *ren = nullptr;

unsigned int fullscreen_flag = 0;

void draw() {
	// clear background
	SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);
	SDL_RenderClear(ren);

	// draw 200x200 rectangle
	SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = 200;
	rect.h = 200;
	SDL_RenderFillRect(ren, &rect);

	// choose color
	int n = (SDL_GetTicks() / 250) % 4;
	switch (n) {
	case 0:
		SDL_SetRenderDrawColor(ren, 255, 0, 0, 255);
		break;
	case 1:
		SDL_SetRenderDrawColor(ren, 0, 255, 0, 255);
		break;
	case 2:
		SDL_SetRenderDrawColor(ren, 0, 0, 255, 255);
		break;
	case 3:
		SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
		break;
	}

	// draw point
	SDL_RenderDrawPoint(ren, 100, 100);

	// update the screen with any rendering performed
	SDL_RenderPresent(ren);
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
					fullscreen_flag = !fullscreen_flag;
					SDL_SetWindowFullscreen(win, fullscreen_flag);
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
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0){
		std::cout << "errror: SDL_Init() : " << SDL_GetError() << std::endl;
		return false;
	}

	int flags = 0;
	//flags |= SDL_WINDOW_BORDERLESS;

	win = SDL_CreateWindow("1px - 01_sdl_simple", 100, 100, 640, 480, flags);
	if (win == nullptr){
		std::cout << "errror: SDL_CreateWindow() : " << SDL_GetError() << std::endl;
		SDL_Quit();
		return false;
	}

	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == nullptr){
		SDL_DestroyWindow(win);
		std::cout << "errror: SDL_CreateRenderer() :" << SDL_GetError() << std::endl;
		return false;
	}

	SDL_ShowWindow(win);
	SDL_ShowCursor(SDL_DISABLE);

	return true;
}

void finish_sdl()
{
	if (ren != nullptr) {
		SDL_DestroyRenderer(ren);
	}
	if (win != nullptr) {
		SDL_DestroyWindow(win);
	}
	SDL_Quit();
}

int main(int argc, char* argv[])
{
	if (init_sdl() == false) return -1;
	mainloop();
	finish_sdl();

	return 0;
}
