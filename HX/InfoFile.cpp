#include "pch.h"
#include "InfoFile.h"
#include <vector>

CInfoFile::CInfoFile()
{
}


CInfoFile::~CInfoFile()
{
}

//读取登陆信息
void CInfoFile::ReadLogin(CString &name, CString &pwd)
{
	ifstream ifs; //创建文件输入对象
	ifs.open(_F_LOGIN); //打开文件

	char buf[1024] = { 0 };

	ifs.getline(buf, sizeof(buf)); //读取一行内容
	name = CString(buf);			 //char *转换为CString

	ifs.getline(buf, sizeof(buf));
	pwd = CString(buf);

	ifs.close(); //关闭文件
}

//修改密码
void CInfoFile::WritePwd(char* name, char* pwd)
{
	ofstream ofs;	 //创建文件输出对象
	ofs.open(_F_LOGIN); //打开文件

	ofs << name << endl; //name写入文件
	ofs << pwd << endl;	//pwd写入文件

	ofs.close();	//关闭文件
}

void CInfoFile::ReadDocline(CString& type, double& x_floor, double& x_ceil, double& y_floor, double& y_ceil, double& theta_floor, double& theta_ceil, int& frame_length, int& frame_width, int& image_threshold, int& image_Filter_block_radius,int& image_closing_radius, int& baoguang_time, int& baoguang2_time)
{
	std::vector<CString> vecResult;
	std::vector<CString> strTmp;
	//msg tmp;
	CString str;//用于接收分割字符的临时变量
	CString strline;

	//读取区域设定
	char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
	//设定“简体中文”区域
	setlocale(LC_CTYPE, "chs");
	CStdioFile file;//定义一个CStdioFile类的对象 file
	BOOL flag = file.Open(_T(".\\stock.txt"), CFile::modeRead);//open函数需要传两个参数，前一个是文件路径，后一个是文件的打开模式
	if (flag == FALSE)
	{
		//MessageBox(_T("文件打开失败"));
	}
	while (file.ReadString(strline))
	{
		vecResult.push_back(strline);
	}
	file.Close();
	//恢复区域设定
	setlocale(LC_CTYPE, old_locale);
	free(old_locale);
	//分割字符部分//


	int curPos = 0;
	str = vecResult[0].Tokenize(_T(" "), curPos);
	while (str.Trim() != _T(""))
	{
		strTmp.push_back(str);
		str = vecResult[0].Tokenize(_T(" "), curPos);
	}
	type = strTmp[0];
	x_floor = _wtof(strTmp[1]);
	x_ceil = _wtof(strTmp[2]);
	y_floor = _wtof(strTmp[3]);
	y_ceil = _wtof(strTmp[4]);
	theta_floor = _wtof(strTmp[5]);
	theta_ceil = _wtof(strTmp[6]);
	frame_length = _wtoi(strTmp[7]);
	frame_width = _wtoi(strTmp[8]);
	image_threshold = _wtoi(strTmp[9]);
	image_Filter_block_radius = _wtoi(strTmp[10]);
	image_closing_radius = _wtoi(strTmp[11]);
	baoguang_time = _wtoi(strTmp[12]);
	baoguang2_time = _wtoi(strTmp[13]);
}

//写入文件
void CInfoFile::WirteDocline(CString& type, double& x_floor, double& x_ceil, double& y_floor, double& y_ceil, double& theta_floor, double& theta_ceil, int& frame_length, int& frame_width, int& image_threshold, int& image_Filter_block_radius, int& image_closing_radius, int& baoguang_time, int& baoguang2_time)
{
	//读取区域设定
	char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
	//设定“简体中文”区域
	setlocale(LC_CTYPE, "chs");
	//创建文件并以文本方式打开
	CStdioFile csdioFile;
	BOOL flag = csdioFile.Open(_T(".\\stock.txt"), CFile::modeReadWrite);
	//写入简体中文数据
	csdioFile.WriteString(type);

	//恢复区域设定
	setlocale(LC_CTYPE, old_locale);
	free(old_locale);

	/*const char* query;
	char temp[10];
	::wsprintfA(temp, "%ls", (LPCTSTR)type);
	query = temp;*/

	csdioFile.WriteString(_T(" "));
	csdioFile.WriteString(DoubleToCString(x_floor));
	csdioFile.WriteString(_T(" "));
	csdioFile.WriteString(DoubleToCString(x_ceil));
	csdioFile.WriteString(_T(" "));
	csdioFile.WriteString(DoubleToCString(y_floor));
	csdioFile.WriteString(_T(" "));
	csdioFile.WriteString(DoubleToCString(y_ceil));
	csdioFile.WriteString(_T(" "));
	csdioFile.WriteString(DoubleToCString(theta_floor));
	csdioFile.WriteString(_T(" "));
	csdioFile.WriteString(DoubleToCString(theta_ceil));
	csdioFile.WriteString(_T(" "));
	csdioFile.WriteString(IntToCString(frame_length));
	csdioFile.WriteString(_T(" "));
	csdioFile.WriteString(IntToCString(frame_width));
	csdioFile.WriteString(_T(" "));
	csdioFile.WriteString(IntToCString(image_threshold));
	csdioFile.WriteString(_T(" "));
	csdioFile.WriteString(IntToCString(image_Filter_block_radius));
	csdioFile.WriteString(_T(" "));
	csdioFile.WriteString(IntToCString(image_closing_radius));
	csdioFile.WriteString(_T(" "));
	csdioFile.WriteString(IntToCString(baoguang_time));
	csdioFile.WriteString(_T(" "));
	csdioFile.WriteString(IntToCString(baoguang2_time));
	csdioFile.WriteString(_T("\r\n"));

	/*MidData = SendFreqData[4];
	char MyChar[10];
	_itoa_s(MidData, MyChar, 10);*/

	csdioFile.Close();
	//char str[1024];
	//wsprintfA(str, "%ls", type);
	//
	//ofs.write(str, type.GetLength());
	////ofs <<" " << type << " ";
	//ofstream ofs(_F_STOCK);
	//ofs <<" " << x_floor << " ";
	//ofs << x_ceil << " ";
	//ofs << y_floor << " ";
	//ofs << y_ceil << " ";
	//ofs << theta_floor << " ";
	////ofs << theta_ceil << " ";
	//ofs << theta_ceil << endl;



	//ofs.close();//关闭文件
}

CString CInfoFile::DoubleToCString(double x)
{
	// TODO: 在此处添加实现代码.
	char temp[1024];
	CString sTemp;
	_gcvt_s(temp, 12, x, 8);//浮点型转为字符串
	sTemp = CA2CT(temp);
	return sTemp;
}


CString CInfoFile::IntToCString(int x)
{
	// TODO: 在此处添加实现代码.
	char temp[1024];
	CString sTemp;
	_itoa_s(x, temp, 10);
	sTemp = temp;
	return sTemp;
}


void CInfoFile::ReadEditFrameLocation( CString& aaa, int& edit_frame_left_x, int& edit_frame_left_y, int& edit_frame_right_x, int& edit_frame_right_y)
{
	std::vector<CString> vecResult;
	std::vector<CString> strTmp;
	//msg tmp;
	CString str;//用于接收分割字符的临时变量
	CString strline;

	//读取区域设定
	char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
	//设定“简体中文”区域
	setlocale(LC_CTYPE, "chs");
	CStdioFile file;//定义一个CStdioFile类的对象 file
	BOOL flag = file.Open(_T(".\\editframelocation.txt"), CFile::modeRead);//open函数需要传两个参数，前一个是文件路径，后一个是文件的打开模式
	if (flag == FALSE)
	{
		//MessageBox(_T("文件打开失败"));
	}
	while (file.ReadString(strline))
	{
		vecResult.push_back(strline);
	}
	file.Close();
	//恢复区域设定
	setlocale(LC_CTYPE, old_locale);
	free(old_locale);
	//分割字符部分//


	int curPos = 0;
	str = vecResult[0].Tokenize(_T(" "), curPos);
	while (str.Trim() != _T(""))
	{
		strTmp.push_back(str);
		str = vecResult[0].Tokenize(_T(" "), curPos);
	}
	
	aaa = strTmp[0];
	edit_frame_left_x = _wtoi(strTmp[1]);
	edit_frame_left_y = _wtoi(strTmp[2]);
	edit_frame_right_x = _wtoi(strTmp[3]);
	edit_frame_right_y = _wtoi(strTmp[4]);
	
}



//写入文件
void CInfoFile::WirteEditFrameLocation(CString aaa, int& edit_frame_left_x, int& edit_frame_left_y, int& edit_frame_right_x, int& edit_frame_right_y)
{
	//读取区域设定
	char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
	//设定“简体中文”区域
	setlocale(LC_CTYPE, "chs");
	//创建文件并以文本方式打开
	CStdioFile csdioFile;
	BOOL flag = csdioFile.Open(_T(".\\editframelocation.txt"), CFile::modeReadWrite);
	//写入简体中文数据
	csdioFile.WriteString(aaa);

	//恢复区域设定
	setlocale(LC_CTYPE, old_locale);
	free(old_locale);

	/*const char* query;
	char temp[10];
	::wsprintfA(temp, "%ls", (LPCTSTR)type);
	query = temp;*/

	
	csdioFile.WriteString(_T(" "));
	csdioFile.WriteString(IntToCString(edit_frame_left_x));
	csdioFile.WriteString(_T(" "));
	csdioFile.WriteString(IntToCString(edit_frame_left_y));
	csdioFile.WriteString(_T(" "));
	csdioFile.WriteString(IntToCString(edit_frame_right_x));
	csdioFile.WriteString(_T(" "));
	csdioFile.WriteString(IntToCString(edit_frame_right_y));
	csdioFile.WriteString(_T(" "));
	

	csdioFile.Close();
}