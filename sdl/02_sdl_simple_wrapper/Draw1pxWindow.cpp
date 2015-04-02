#include "Draw1pxWindow.h"


Draw1pxWindow::Draw1pxWindow() : SDLWindowBase()
{
}

void Draw1pxWindow::draw()
{
	// clear background
	SDL_SetRenderDrawColor(ren_, 255, 255, 255, 255);
	SDL_RenderClear(ren_);

	// draw 200x200 rectangle
	SDL_SetRenderDrawColor(ren_, 0, 0, 0, 255);
	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = 200;
	rect.h = 200;
	SDL_RenderFillRect(ren_, &rect);

	// choose color
	int n = (SDL_GetTicks() / 250) % 4;
	switch (n) {
	case 0:
		SDL_SetRenderDrawColor(ren_, 255, 0, 0, 255);
		break;
	case 1:
		SDL_SetRenderDrawColor(ren_, 0, 255, 0, 255);
		break;
	case 2:
		SDL_SetRenderDrawColor(ren_, 0, 0, 255, 255);
		break;
	case 3:
		SDL_SetRenderDrawColor(ren_, 0, 0, 0, 255);
		break;
	}

	// draw point
	SDL_RenderDrawPoint(ren_, 100, 100);

	// update the screen with any rendering performed
	SDL_RenderPresent(ren_);
}