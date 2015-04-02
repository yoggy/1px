#include "CvMatWindow.h"
#include <string>
#include <iostream>

cv::Mat canvas_img;
CvMatWindow *win = NULL;
bool fullscreen_flag = false;

void init_opencv()
{
	canvas_img.create(cv::Size(200, 200), CV_8UC3);
	win->set_background_color(cv::Scalar(128, 128, 128));
}

void draw()
{
	// clear canvas_img background
	canvas_img.setTo(cv::Scalar(0, 0, 0)); 

	// choose color
	cv::Vec3b color;
	int n = (SDL_GetTicks() / 250) % 4;
	switch (n % 4) {
	case 0:
		color = cv::Vec3b(0, 0, 255);
		break;
	case 1:
		color = cv::Vec3b(0, 255, 0);
		break;
	case 2:
		color = cv::Vec3b(255, 0, 0);
		break;
	default:
		color = cv::Vec3b(0, 0, 0);
		break;
	}
	// draw 1pix
	canvas_img.at<cv::Vec3b>(cv::Point(100, 100)) = color;

	// update window using canvas_img
	win->set_image(canvas_img);
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

	win = new CvMatWindow();
	rv = win->create("1px - 03_sdl_opencv", 100, 100, 640, 480);
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
	init_opencv();
	mainloop();
	finish_sdl();

	return 0;
}

