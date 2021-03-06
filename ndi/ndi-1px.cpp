//
// ndi-1px.cpp  - NewTek NDI Sender version
//
// reference:
//      NDI SDK
//      https://www.newtek.com/ndi/sdk/
//
// github :
//     https ://github.com/yoggy/1px
//
// license :
//     Copyright(c) 2015 yoggy <yoggy0@gmail.com>
//     Released under the MIT license
//     http://opensource.org/licenses/mit-license.php;
//
#include <Processing.NDI.Lib.h>
#include <stdio.h>
#include <stdlib.h>
#include <chrono>

int main(int argc, char *argv)
{
	bool rv;
	rv = NDIlib_initialize();
	if (rv == false) {
		printf("ERROR : NDIlib_initialize() failed...\n");
		return -1;
	}

	rv = NDIlib_is_supported_CPU();
	if (rv == false) {
		printf("ERROR : NDIlib_is_supported_CPU() == false...\n");
		return -1;
	}

	// setup send instance
	NDIlib_send_create_t conf;
	conf.p_ndi_name = "1px";

	NDIlib_send_instance_t pNDI_send = NDIlib_send_create(&conf);
	if (pNDI_send == NULL) {
		printf("ERROR : NDIlib_send_create() failed...\n");
		return -1;
	}

	// prepare video frame
	NDIlib_video_frame_v2_t frame;
	frame.xres = 200;
	frame.yres = 200;
	frame.picture_aspect_ratio = (float)frame.xres / (float)frame.yres;
	frame.frame_rate_N = 60;
	frame.frame_rate_D = 1;
	frame.frame_format_type = NDIlib_frame_format_type_progressive;
	frame.FourCC = NDIlib_FourCC_type_BGRX;
	frame.p_data = (uint8_t*)malloc(frame.xres * frame.yres * 4);

	using namespace std::chrono;
	auto st = high_resolution_clock::now();
	int idx = 0;
	uint8_t r, g, b;
	for (const auto start = high_resolution_clock::now(); high_resolution_clock::now() - start < minutes(3);) {
		// clear frame buffer
		memset((void*)frame.p_data, 0, frame.xres * frame.yres * 4);

		// choice color
		switch (idx) {
		case 0:
			r = 255; g = 0; b = 0;
			break;
		case 1:
			r = 0; g = 255; b = 0;
			break;
		case 2:
			r = 0; g = 0; b = 255;
			break;
		case 3:
			r = 0; g = 0; b = 0;
			break;
		}

		// set 1px
		int x = 100;
		int y = 100;
		frame.p_data[x * 4 + frame.xres * 4 * y + 0] = b;
		frame.p_data[x * 4 + frame.xres * 4 * y + 1] = g;
		frame.p_data[x * 4 + frame.xres * 4 * y + 2] = r;

		// send frame
		NDIlib_send_send_video_v2(pNDI_send, &frame);

		// change color every 250 ms
		auto count = duration_cast<milliseconds>(high_resolution_clock::now() - st).count();
		if (count >= 250) {
			idx = (idx + 1) % 4;
			st = high_resolution_clock::now();
		}
	}

	free(frame.p_data);
	NDIlib_send_destroy(pNDI_send);
	NDIlib_destroy();

	return 0;
}
