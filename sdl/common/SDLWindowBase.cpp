#include "SDLWindowBase.h"

#include <iostream>

SDLWindowBase::SDLWindowBase()
	: win_(NULL), ren_(NULL)
{

}

SDLWindowBase::~SDLWindowBase()
{
	close();
}

bool SDLWindowBase::create(const char *title, const int &x, const int &y, const int &width, const int &height, const int &flags)
{
	win_ = SDL_CreateWindow(title, x, y, width, height, flags);
	if (win_ == NULL){
		std::cerr << "error : SDL_CreateWindow() : " << SDL_GetError() << std::endl;
		close();
		return false;
	}

	ren_ = SDL_CreateRenderer(win_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren_ == NULL){
		std::cerr << "error : SDL_CreateRenderer() :" << SDL_GetError() << std::endl;
		close();
		return false;
	}

	SDL_ShowWindow(win_);
	SDL_ShowCursor(SDL_DISABLE);

	return true;
}

void SDLWindowBase::close()
{
	if (ren_ != NULL) {
		SDL_DestroyRenderer(ren_);
		ren_ = NULL;
	}
	if (win_ != NULL) {
		SDL_DestroyWindow(win_);
		win_ = NULL;
	}
}

int SDLWindowBase::get_window_width()
{
	int w, h;
	get_window_size(w, h);
	return w;
}

int SDLWindowBase::get_window_height()
{
	int w, h;
	get_window_size(w, h);
	return h;
}

void SDLWindowBase::get_window_size(int &width, int &height)
{
	if (win_ == NULL) return;
	SDL_GetWindowSize(win_, &width, &height);
}

bool SDLWindowBase::get_fullscreen()
{
	if (win_ == NULL) return false;

	int flag = SDL_GetWindowFlags(win_);
	if (flag & SDL_WINDOW_FULLSCREEN) return true;

	return false;
}

void SDLWindowBase::set_fullscreen(const bool &flag)
{
	if (win_ == NULL) return;

	SDL_SetWindowFullscreen(win_, flag);
}
