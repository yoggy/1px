#include "CvMatWindow.h"
#include <iostream>

CvMatWindow::CvMatWindow() 
	: SDLWindowBase(), tex_(NULL)
{
}

void CvMatWindow::close()
{
	delete_texture_();
	SDLWindowBase::close();
}

void CvMatWindow::set_background_color(const cv::Scalar &color)
{
	background_color_ = color;
}

void CvMatWindow::set_image(const cv::Mat &img)
{
	if (img.empty()) return;

	if (tex_ == NULL) {
		create_texture_(img.size());
	}
	copy_(img, tex_, tex_size_);
}

bool CvMatWindow::create_texture_(const cv::Size &size)
{
	if (tex_ != NULL) {
		delete_texture_();
	}

	tex_ = SDL_CreateTexture(ren_, SDL_PIXELFORMAT_BGR24, SDL_TEXTUREACCESS_STREAMING, size.width, size.height);
	if (tex_ == NULL) {
		std::cerr << "error : SDL_CreateTexture() :" << SDL_GetError() << std::endl;
		return false;
	}

	tex_size_ = size;

	return true;
}

void CvMatWindow::delete_texture_()
{
	if (tex_ != NULL) {
		SDL_DestroyTexture(tex_);
		tex_ = NULL;
	}
}

bool CvMatWindow::copy_(const cv::Mat &src_img, SDL_Texture *dst_tex, const cv::Size &dst_tex_size)
{
	// check
	if (src_img.empty()) {
		std::cerr << "error : CvMatWindow::copy_() : src_img is empty..." << std::endl;
		return false;
	}
	if (dst_tex == NULL) {
		std::cerr << "error : CvMatWindow::copy_() : dst_tex is NULL..." << std::endl;
		return false;
	}
	if (src_img.size() != dst_tex_size) {
		std::cerr << "error : CvMatWindow::copy_() : tex_size_ != img.size()" << std::endl;
		return false;
	}

	// lock the texture pixel buffer (write only)
	uchar *buf;
	int pitch;
	int rv = SDL_LockTexture(tex_, NULL, (void**)&buf, &pitch);
	if (rv) {
		std::cerr << "error : CvMatWindow::copy_() : SDL_LockTexture() : " << SDL_GetError() << std::endl;
		return false;
	}

	// cv::Mat -> texture (pixel buffer) 
	cv::Mat dst_img(dst_tex_size, CV_8UC3, (void*)buf);
	src_img.copyTo(dst_img);

	// unlock 
	SDL_UnlockTexture(tex_);
	return true;
}

SDL_Rect CvMatWindow::get_render_copy_rect_()
{
	int window_w, window_h;
	get_window_size(window_w, window_h);

	int tex_w = tex_size_.width;
	int tex_h = tex_size_.height;

	float window_aspect = window_w / (float)window_h;
	float tex_aspect = tex_w / (float)tex_h;

	int x, y, w, h;
	if (window_aspect >= tex_aspect) {
		// pillar box
		float p = window_h / (float)tex_h;
		float zoom_w = tex_w * p;
		float zoom_h = (float)window_h;

		x = (int)((window_w - zoom_w) / 2);
		y = 0;
		w = (int)zoom_w;
		h = (int)zoom_h;
	}
	else {
		// letter box
		float p = window_w / (float)tex_w;
		float zoom_w = (float)window_w;
		float zoom_h = tex_h * p;

		x = 0;
		y = (int)((window_h - zoom_h) / 2);
		w = (int)zoom_w;
		h = (int)zoom_h;
	}

	SDL_Rect r = {x, y, w, h};
	return r;
}

void CvMatWindow::draw()
{
	// clear background
	SDL_SetRenderDrawColor(ren_, (Uint8)background_color_.val[2], (Uint8)background_color_.val[1], (Uint8)background_color_.val[0], 255);
	SDL_RenderClear(ren_);

	if (tex_ != NULL) {
		SDL_Rect rect_dst = get_render_copy_rect_();
		SDL_RenderCopy(ren_, tex_, NULL, &rect_dst);
	}

	// update the screen with any rendering performed
	SDL_RenderPresent(ren_);
}