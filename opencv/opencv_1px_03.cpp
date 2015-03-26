//
// opencv_1px_03.cpp - OpenCV 3.0を使用した1px描画
// 
// 1px操作をすると負けた気がするので、意地でもピクセル操作を行わないバージョン
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

		// set ROI (Region of Interesting)
		cv::Rect roi(100, 100, 1, 1);

		// draw 1px
		switch (count % 4) {
		case 0:
			canvas_img(roi) = cv::Scalar(0, 0, 255);
			break;
		case 1:
			canvas_img(roi) = cv::Scalar(0, 255, 0);
			break;
		case 2:
			canvas_img(roi) = cv::Scalar(255, 0, 0);
			break;
		default:
			canvas_img(roi) = cv::Scalar(0, 0, 0);
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

