#pragma once

#include "../common/SDLWindowBase.h"

#pragma warning(disable : 4819)
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#ifdef _WIN32
#ifdef _DEBUG
#pragma comment(lib, "opencv_world300d.lib")
#else
#pragma comment(lib, "opencv_world300.lib")
#endif
#endif

class CvMatWindow : public SDLWindowBase
{
public:
	CvMatWindow();

	virtual void close();

	void set_background_color(const cv::Scalar &color);
	void set_image(const cv::Mat &img);
	virtual void draw();

protected:
	bool create_texture_(const cv::Size &size);
	void delete_texture_();
	bool copy_(const cv::Mat &src_img, SDL_Texture *dst_tex, const cv::Size &dst_tex_size);
	SDL_Rect get_render_copy_rect_();

protected:
	SDL_Texture *tex_;
	cv::Size tex_size_;
	cv::Scalar background_color_;
	cv::Mat image_;
};

