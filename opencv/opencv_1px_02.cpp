//
// opencv_1px_02.cpp - OpenCV 3.0を使用した1px描画
// 
// cv::Mat::ptr<uchar>(int y)を使用した危険高速バージョン
//
#ifdef _WIN32
#pragma warning(disable : 4819)
#endif

// opencv 3.0
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#ifdef _WIN32
#ifdef _DEBUG
#pragma comment(lib, "opencv_world300d.lib")
#else
#pragma comment(lib, "opencv_world300.lib")
#endif
#endif

int main(int argc, char* argv[])
{
	int count = 0;

	// create BGR image
	cv::Mat canvas_img(cv::Size(200, 200), CV_8UC3);

	while (true) {
		// fill black
		canvas_img = cv::Scalar(0, 0, 0);

		int x = 100;
		int y = 100;
		uchar *p = canvas_img.ptr<uchar>(y) + (x * canvas_img.channels());

		// draw 1pix (direct access)
		switch (count % 4) {
		case 0:
			*p++ = 0x00; // b
			*p++ = 0x00; // g
			*p++ = 0xff; // r
			break;
		case 1:
			*p++ = 0x00;
			*p++ = 0xff;
			*p++ = 0x00;
			break;
		case 2:
			*p++ = 0xff;
			*p++ = 0x00;
			*p++ = 0x00;
			break;
		default:
			*p++ = 0x00;
			*p++ = 0x00;
			*p++ = 0x00;
			break;
		}

		// show image
		cv::imshow("opencv_1px", canvas_img);

		int c = cv::waitKey(250);
		if (c == 27) break;

		count++;
	}
	cv::destroyAllWindows();

	return 0;
}

