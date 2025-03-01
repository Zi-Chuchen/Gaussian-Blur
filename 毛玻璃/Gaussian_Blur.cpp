/*
*名称：Gaussian_Blur
*制作者：紫初辰
*介绍：使用easyx进行高斯模糊
*编译环境：vc2022
*备注:请将该源码文件拷贝至源文件目录
*/
#include "Gaussian_Blur.h"
#include <windows.h>
#include <tchar.h>
#include <easyx.h>
#include <assert.h>
#include <thread>
#include <vector>

Gaussian_Blur::Gaussian_Blur() = default;

Gaussian_Blur::~Gaussian_Blur() {
	save_ok = false;
	clear_COLOR_array(COLOR_image_array, height);
	clear_COLOR_array(COLOR_image_result_array, height + radius * 2);
	clear_distance_table(distance_table);
	clear_result_vector(result);
}

void Gaussian_Blur::set_blur_object(IMAGE* image, execute_method method) {
	IMAGE_object = image;
	width = image->getwidth();
	height = image->getheight();
	if (!(width && height)) {
		assert(!("ERROR:WIDTH OR HEIGHT IS 0"));
	}
	this->~Gaussian_Blur();
	COLOR_image_array = build_COLOR_array(width, height);
	DWORD* image_buffer = GetImageBuffer(image);
	deal_with_buffer(image_buffer, method);
	COLOR_image_result_array = build_COLOR_array(width + radius * 2, height + radius * 2);
	copy_image(COLOR_image_result_array);
	save_ok = true;
}

void Gaussian_Blur::set_blur_object(DWORD* image_buffer, int width, int height, execute_method method) {
	DWORD_object = image_buffer;
	width = width;
	height = height;
	int size = sizeof(image_buffer);
	int area = size / sizeof(image_buffer[0]);
	if (!(width && height)) {
		assert(!("ERROR:WIDTH OR HEIGHT IS 0"));
	}
	else if (width * height > area) {
		assert(!("ERROR:EXCESSIVE WIDTH OR HEIGHT"));
	}
	this->~Gaussian_Blur();
	COLOR_image_array = build_COLOR_array(width, height);
	deal_with_buffer(image_buffer, method);
	COLOR_image_result_array = build_COLOR_array(width + radius * 2, height + radius * 2);
	copy_image(COLOR_image_result_array);
	save_ok = true;
}

void Gaussian_Blur::set_radius(int radius) {
	if (radius < 1) {
		assert(!("ERROR:RADIUS SMALL WITH 1"));
	}
	clear_distance_table(distance_table);
	this->radius = radius;
	distance_table = build_distance_table();
	COLOR_image_result_array = build_COLOR_array(width + radius * 2, height + radius * 2);
	copy_image(COLOR_image_result_array);
}

void Gaussian_Blur::set_sigma(double sigma) {
	if (sigma < 0) {
		assert(!("ERROR:SIGMA SMALL WITH 0"));
	}
	clear_distance_table(distance_table);
	this->sigma = sigma;
	distance_table = build_distance_table();
}

void Gaussian_Blur::gaussian_blur(execute_method method) {
	if (!save_ok) {
		return;
	}
	IMAGE* s = new IMAGE();
	s->Resize(width, height);
	result.push_back(s);
	switch (method)
	{
	case execute_method::new_thread:
	{
		COLOR** i = new COLOR * (*COLOR_image_result_array);
		double** d = new double* (*distance_table);
		std::thread t(&Gaussian_Blur::blur, this, i, d);
		t.detach();
	}
		break;
	case execute_method::main_thread:
	{
		COLOR** i = new COLOR* (*COLOR_image_result_array);
		double** d = new double* (*distance_table);
		blur(i, d);
	}
		break;
	}
}

void Gaussian_Blur::get_blur_result(IMAGE* save_ptr) {
	*save_ptr = *(result.back());
}

void Gaussian_Blur::read_COLOR(DWORD* image_buffer) {
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			COLOR_image_array[i][j] = RGB_to_COLOR(image_buffer[i * width + j]);
}

void Gaussian_Blur::deal_with_buffer(DWORD* image_buffer, execute_method method) {
	switch (method) {
	case execute_method::new_thread:
	{
		std::thread t(&Gaussian_Blur::read_COLOR, this, image_buffer);
		t.detach();
	}
	break;

	case execute_method::main_thread:
		read_COLOR(image_buffer);
		break;
	}
}

Gaussian_Blur::COLOR Gaussian_Blur::RGB_to_COLOR(DWORD rgb)
{
	COLOR result;
	result.R = GetRValue(rgb);
	result.G = GetGValue(rgb);
	result.B = GetBValue(rgb);
	return result;
}

void Gaussian_Blur::blur(COLOR** COLOR_image_result_array,double ** distance_table) {
	DWORD* image_buffer = GetImageBuffer(result.back());
	for (int row = 0; row < height; row++)
	{
		for (int col = 0; col < width; col++)
		{
			double red_all = 0, green_all = 0, blue_all = 0;
			for (int area_row = -radius; area_row < radius; area_row++)
			{
				for (int area_col = -radius; area_col < radius; area_col++)
				{
					int tx = area_row + radius;
					int ty = area_col + radius;
					int x = row + tx;
					int y = col + ty;
					red_all += COLOR_image_result_array[x][y].R *
						distance_table[tx][ty];
					green_all += COLOR_image_result_array[x][y].G *
						distance_table[tx][ty];
					blue_all += COLOR_image_result_array[x][y].B *
						distance_table[tx][ty];
				}
			}
			image_buffer[row * width + col] = RGB((BYTE)(red_all / weight_sum), (BYTE)(green_all / weight_sum), (BYTE)(blue_all / weight_sum));
		}
	}
	delete []COLOR_image_result_array;
	delete []distance_table;
}

void Gaussian_Blur::copy_image(COLOR** result)
{
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
			result[i + radius][j + radius] = COLOR_image_array[i][j];
	for (int i = 0; i < radius; i++)
	{
		for (int j = 0; j < radius; j++)
		{
			result[i][j] = COLOR_image_array[0][0];
			result[i + radius + height][j] = COLOR_image_array[height - 1][0];
			result[i][j + radius + width] = COLOR_image_array[0][width - 1];
			result[i + radius + height][j + radius + width] = COLOR_image_array[height - 1][width - 1];
		}
	}
	for (int i = 0; i < radius; i++)
	{
		for (int j = 0; j < width; j++)
		{
			result[i][j + radius] = COLOR_image_array[0][j];
			result[(i + radius + height)][j + radius] = COLOR_image_array[height - 1][j];
		}
	}
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < radius; j++)
		{
			result[i + radius][j] = COLOR_image_array[i][0];
			result[i + radius][j + radius + width] = COLOR_image_array[i][width - 1];
		}
	}
}

double** Gaussian_Blur::build_distance_table() {
	weight_sum = 0;
	double** result = new double* [2 * radius];
	for (int i = 0; i < 2 * radius; i++)
		result[i] = new double[2 * radius];
	for (int i = -radius; i < radius; i++)
	{
		for (int j = -radius; j < radius; j++)
		{
			double value = exp(-(0.5 * (i * i + j * j) / (sigma * sigma)));
			result[i + radius][j + radius] = value;
			weight_sum += value;
		}
	}
	return result;
}

void Gaussian_Blur::clear_distance_table(double** distance_table) {
	if (distance_table == NULL) {
		return;
	}
	for (int i = 0; i < 2 * radius; i++) {
		if (distance_table[i] != NULL) {
			delete[] distance_table[i];
			distance_table[i] = NULL;
		}
	}
	delete[] distance_table;
	distance_table = NULL;
}

Gaussian_Blur::COLOR** Gaussian_Blur::build_COLOR_array(int width, int height) {
	COLOR** array = new COLOR * [height];
	for (int i = 0; i < height; i++)
		array[i] = new COLOR[width];
	return array;
}

void Gaussian_Blur::clear_COLOR_array(COLOR** array, int height)
{
	if (array == NULL) {
		return;
	}
	for (int i = 0; i < height; i++) {
		if (array[i] != NULL) {
			delete[] array[i];
			array[i] = NULL;
		}
	}
	delete[] array;
	array = NULL;
}
void Gaussian_Blur::clear_result_vector(std::vector<IMAGE*> vec){
	for (int i = 0; i < vec.size(); i++) {
		if (vec[i]) {
			delete vec[i];
		}
		vec[i] = NULL;
	}
}
