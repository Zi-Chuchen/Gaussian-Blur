#pragma once
/*
*���ƣ�Gaussian_Blur
*�����ߣ��ϳ���
*���ܣ�ʹ��easyx���и�˹ģ��
*���뻷����vc2022
*��ע:�뽫��ͷ�ļ�������ͷ�ļ�Ŀ¼
*/
#include <windows.h>
#include <tchar.h>
#include <easyx.h>
#include <assert.h>
#include <thread>
#include <vector>

class Gaussian_Blur {
public:
	typedef enum class execute_method {//ִ��ģʽ
		new_thread,//��һ�����߳�(���ߴ���,����������)
		main_thread//�������߳�(�����ߴ���,��������)

	}execute_method;

	typedef struct COLOR{//�Զ��嶨��RGB����,���㴦��
		BYTE R;
		BYTE G;
		BYTE B;
	}COLOR;

public:
	Gaussian_Blur();
	~Gaussian_Blur();
	void set_blur_object(IMAGE*,execute_method = execute_method::main_thread);//����ģ������,����1,����IMAGE*����,ȫ��ģ��;��һ��������ִ��ģʽ,Ĭ�������߳̽���,���execute_method����
	void set_blur_object(DWORD*, int, int, execute_method = execute_method::main_thread);//����ģ������,����1,����DWORD*����,����2��int,�ֱ�Ϊ��͸�,ȫ��ģ��;��һ��������ִ��ģʽ,Ĭ�������߳̽���,���execute_method����
	void set_radius(int);//����ģ���뾶,ʵ��ģ���������ο�Ϊ2r+1
	void set_sigma(double);//����ģ������
	void gaussian_blur(execute_method = execute_method::main_thread);//��ʼģ��;��������ִ��ģʽ,Ĭ�������߳̽���,���execute_method����
	void get_blur_result(IMAGE*);//��ȡģ�����
private:
	void read_COLOR(DWORD*);//������
	void deal_with_buffer(DWORD*, execute_method);//����ͼƬ������
	COLOR RGB_to_COLOR(DWORD);//RGBת�Զ���COLOR����
	void copy_image(COLOR**);//����ͼ��(����߿�)
	void blur(COLOR**,double**);//ģ��
	double** build_distance_table();//���������
	void clear_distance_table(double**);//�ͷž�����ڴ�
	COLOR** build_COLOR_array(int, int);//����ͼ��COLOR������
	void clear_COLOR_array(COLOR**, int);//�ͷ�ͼ��COLOR������
	void clear_result_vector(std::vector<IMAGE*>);//�ͷ�ͼ����vector
private:
	int width = 0;//ͼƬ��
	int height = 0;//ͼƬ��
	IMAGE* IMAGE_object = NULL;//ģ������IMAGE*�洢����
	DWORD* DWORD_object = NULL;//ģ������DWORD*�洢����
	bool save_ok = false;//�Ƿ�洢�ɹ�
	std::vector<IMAGE*> result{};//������
	int radius = 5;//ģ���뾶
	double sigma = 5;//ģ������
	double weight_sum = 0;//���������Ԫ��֮��
	COLOR** COLOR_image_array = NULL;//ԭͼ��COLOR��������ַ
	COLOR** COLOR_image_result_array = NULL;//�������ͼ��COLOR��������ַ
	double** distance_table = NULL;//������ַ
};