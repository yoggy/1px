#pragma once

#include <SDL.h>

#ifdef WIN32
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#endif

class SDLWindowBase
{
public:
	SDLWindowBase();
	virtual ~SDLWindowBase();

	virtual bool create(const char *title, const int &x, const int &y, const int &width, const int &height, const int &flags = 0);
	virtual void close();

	int get_window_width();
	int get_window_height();
	void get_window_size(int &width, int &height);
	bool get_fullscreen();
	void set_fullscreen(const bool &flag);

	virtual void draw() = 0;

protected:
	SDL_Window   *win_;
	SDL_Renderer *ren_;
};

