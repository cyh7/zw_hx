#pragma once

#include <list>
#include <fstream>
#include <iostream>
#include <string>

#define _F_LOGIN "./login.ini"
#define _F_STOCK "./stock.txt"
#define _F_EDIT "./editframelocation.txt"

using namespace std;

struct msg
{
	CString backboard; //�����ͺ�
	double x_floor; //x����
	double x_ceil; //x����
	double y_floor; //x����
	double y_ceil; //x����
	double theta_floor; //�Ƕ�����
	double theta_ceil; //�Ƕ�����
	int frame_length;
	int frame_width;
	int image_threshold;
};

class CInfoFile
{
public:
	CInfoFile();
	~CInfoFile();

	//��ȡ��½��Ϣ
	void ReadLogin(CString &name, CString &pwd);

	//�޸�����
	void WritePwd(char* name, char* pwd);

	// ��ȡ��Ʒ����
	void ReadDocline(CString& type, double& x_floor, double& x_ceil, double& y_floor, double& y_ceil, double& theta_floor, double& theta_ceil, int &frame_length, int& frame_width, int& image_threshold ,int& image_Filter_block_radius, int& image_closing_radius,int& baoguang_time, int& baoguang2_time);

	//��Ʒд���ļ�
	void WirteDocline(CString& type, double& x_floor, double& x_ceil, double& y_floor, double& y_ceil, double& theta_floor, double& theta_ceil, int& frame_length, int& frame_width, int& image_threshold, int& image_Filter_block_radius, int& image_closing_radius, int& baoguang_time, int& baoguang2_time);

	

	//�������Ʒ
	//void Addline(CString name, int num, int price);

	//list<msg> ls;	//�洢��Ʒ����
	//int num;			//������¼��Ʒ����
	CString DoubleToCString(double x);
	CString IntToCString(int x);
	
	//���ڴ��ж�ȡ�ü���λ��
	void ReadEditFrameLocation(CString& aaa, int& edit_frame_left_x, int& edit_frame_left_y, int& edit_frame_right_x, int& edit_frame_right_y);
	//���ü���λ�ô洢���ڴ���
	void WirteEditFrameLocation(CString aaa, int& edit_frame_left_x, int& edit_frame_left_y, int& edit_frame_right_x, int& edit_frame_right_y);
};

