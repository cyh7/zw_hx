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
	CString backboard; //背板型号
	double x_floor; //x下限
	double x_ceil; //x上限
	double y_floor; //x下限
	double y_ceil; //x上限
	double theta_floor; //角度下限
	double theta_ceil; //角度上限
	int frame_length;
	int frame_width;
	int image_threshold;
};

class CInfoFile
{
public:
	CInfoFile();
	~CInfoFile();

	//读取登陆信息
	void ReadLogin(CString &name, CString &pwd);

	//修改密码
	void WritePwd(char* name, char* pwd);

	// 读取商品数据
	void ReadDocline(CString& type, double& x_floor, double& x_ceil, double& y_floor, double& y_ceil, double& theta_floor, double& theta_ceil, int &frame_length, int& frame_width, int& image_threshold ,int& image_Filter_block_radius, int& image_closing_radius,int& baoguang_time, int& baoguang2_time);

	//商品写入文件
	void WirteDocline(CString& type, double& x_floor, double& x_ceil, double& y_floor, double& y_ceil, double& theta_floor, double& theta_ceil, int& frame_length, int& frame_width, int& image_threshold, int& image_Filter_block_radius, int& image_closing_radius, int& baoguang_time, int& baoguang2_time);

	

	//添加新商品
	//void Addline(CString name, int num, int price);

	//list<msg> ls;	//存储商品容器
	//int num;			//用来记录商品个数
	CString DoubleToCString(double x);
	CString IntToCString(int x);
	
	//从内存中读取裁剪框位置
	void ReadEditFrameLocation(CString& aaa, int& edit_frame_left_x, int& edit_frame_left_y, int& edit_frame_right_x, int& edit_frame_right_y);
	//将裁剪框位置存储到内存中
	void WirteEditFrameLocation(CString aaa, int& edit_frame_left_x, int& edit_frame_left_y, int& edit_frame_right_x, int& edit_frame_right_y);
};

