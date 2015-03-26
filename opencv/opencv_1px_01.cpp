//
// opencv_1px_01.cpp - OpenCV 3.0を使用した1px描画
// 
// cv::Mat::at<cv::Vec3b>(cv::Point p)を使用した安全低速バージョン
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

		// choice color
		cv::Vec3b color;
		switch (count % 4) {
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

		// show image
		cv::imshow("opencv_1px", canvas_img);

		int c = cv::waitKey(250);
		if (c == 27) break;

		count++;
	}
	cv::destroyAllWindows();

	return 0;
}

