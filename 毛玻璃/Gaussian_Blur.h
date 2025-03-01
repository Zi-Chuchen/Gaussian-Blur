#pragma once
/*
*名称：Gaussian_Blur
*制作者：紫初辰
*介绍：使用easyx进行高斯模糊
*编译环境：vc2022
*备注:请将该头文件拷贝至头文件目录
*/
#include <windows.h>
#include <tchar.h>
#include <easyx.h>
#include <assert.h>
#include <thread>
#include <vector>

class Gaussian_Blur {
public:
	typedef enum class execute_method {//执行模式
		new_thread,//开一个新线程(多线处理,不阻塞主线)
		main_thread//不开新线程(在主线处理,阻塞主线)

	}execute_method;

	typedef struct COLOR{//自定义定义RGB类型,方便处理
		BYTE R;
		BYTE G;
		BYTE B;
	}COLOR;

public:
	Gaussian_Blur();
	~Gaussian_Blur();
	void set_blur_object(IMAGE*,execute_method = execute_method::main_thread);//设置模糊对象,重载1,接受IMAGE*类型,全部模糊;后一参数设置执行模式,默认在主线程进行,详见execute_method定义
	void set_blur_object(DWORD*, int, int, execute_method = execute_method::main_thread);//设置模糊对象,重载1,接受DWORD*类型,还有2个int,分别为宽和高,全部模糊;后一参数设置执行模式,默认在主线程进行,详见execute_method定义
	void set_radius(int);//设置模糊半径,实际模糊的正方形宽为2r+1
	void set_sigma(double);//设置模糊方差
	void gaussian_blur(execute_method = execute_method::main_thread);//开始模糊;参数设置执行模式,默认在主线程进行,详见execute_method定义
	void get_blur_result(IMAGE*);//获取模糊结果
private:
	void read_COLOR(DWORD*);//读像素
	void deal_with_buffer(DWORD*, execute_method);//处理图片缓冲区
	COLOR RGB_to_COLOR(DWORD);//RGB转自定义COLOR类型
	void copy_image(COLOR**);//复制图像(处理边框)
	void blur(COLOR**,double**);//模糊
	double** build_distance_table();//构建距离表
	void clear_distance_table(double**);//释放距离表内存
	COLOR** build_COLOR_array(int, int);//构建图像COLOR缓冲区
	void clear_COLOR_array(COLOR**, int);//释放图像COLOR缓冲区
	void clear_result_vector(std::vector<IMAGE*>);//释放图像结果vector
private:
	int width = 0;//图片宽
	int height = 0;//图片长
	IMAGE* IMAGE_object = NULL;//模糊对象IMAGE*存储数据
	DWORD* DWORD_object = NULL;//模糊对象DWORD*存储数据
	bool save_ok = false;//是否存储成功
	std::vector<IMAGE*> result{};//处理结果
	int radius = 5;//模糊半径
	double sigma = 5;//模糊方差
	double weight_sum = 0;//距离表所有元素之和
	COLOR** COLOR_image_array = NULL;//原图像COLOR缓冲区地址
	COLOR** COLOR_image_result_array = NULL;//处理结束图像COLOR缓冲区地址
	double** distance_table = NULL;//距离表地址
};