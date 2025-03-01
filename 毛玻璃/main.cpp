#include <easyx.h>
#include <iostream>
#include <tchar.h>
#include <windows.h>
#include "Gaussian_Blur.h"
#include <cstdlib>
#include <time.h>

int main() {
	IMAGE image;
	loadimage(&image, _T("PNG"),_T("test"));//µ¼ÈëÍ¼Æ¬
	initgraph(image.getwidth(), image.getheight(),EX_SHOWCONSOLE);
	putimage(0, 0, &image);
	Gaussian_Blur gaussian;
	gaussian.set_blur_object(&image);
	gaussian.set_radius(10);
	gaussian.set_sigma(10);
	//gaussian.gaussian_blur();
	srand(time(NULL));
	while (1) {
		int radius = 1 + rand() % 10;
		gaussian.set_radius(radius);
		gaussian.gaussian_blur(/*Gaussian_Blur::execute_method::new_thread*/);
		IMAGE result;
		gaussian.get_blur_result(&result);
		putimage(0, 0, &result);
		std::cout << "once" << std::endl;
	}
	closegraph();
	return 0;
}