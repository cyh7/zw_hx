#include "pch.h"
#include "InfoFile.h"
#include <vector>

CInfoFile::CInfoFile()
{
}


CInfoFile::~CInfoFile()
{
}

//��ȡ��½��Ϣ
void CInfoFile::ReadLogin(CString &name, CString &pwd)
{
	ifstream ifs; //�����ļ��������
	ifs.open(_F_LOGIN); //���ļ�

	char buf[1024] = { 0 };

	ifs.getline(buf, sizeof(buf)); //��ȡһ������
	name = CString(buf);			 //char *ת��ΪCString

	ifs.getline(buf, sizeof(buf));
	pwd = CString(buf);

	ifs.close(); //�ر��ļ�
}

//�޸�����
void CInfoFile::WritePwd(char* name, char* pwd)
{
	ofstream ofs;	 //�����ļ��������
	ofs.open(_F_LOGIN); //���ļ�

	ofs << name << endl; //nameд���ļ�
	ofs << pwd << endl;	//pwdд���ļ�

	ofs.close();	//�ر��ļ�
}

void CInfoFile::ReadDocline(CString& type, double& x_floor, double& x_ceil, double& y_floor, double& y_ceil, double& theta_floor, double& theta_ceil, int& frame_length, int& frame_width, int& image_threshold, int& image_Filter_block_radius,int& image_closing_radius, int& baoguang_time, int& baoguang2_time)
{
	std::vector<CString> vecResult;
	std::vector<CString> strTmp;
	//msg tmp;
	CString str;//���ڽ��շָ��ַ�����ʱ����
	CString strline;

	//��ȡ�����趨
	char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
	//�趨���������ġ�����
	setlocale(LC_CTYPE, "chs");
	CStdioFile file;//����һ��CStdioFile��Ķ��� file
	BOOL flag = file.Open(_T(".\\stock.txt"), CFile::modeRead);//open������Ҫ������������ǰһ�����ļ�·������һ�����ļ��Ĵ�ģʽ
	if (flag == FALSE)
	{
		//MessageBox(_T("�ļ���ʧ��"));
	}
	while (file.ReadString(strline))
	{
		vecResult.push_back(strline);
	}
	file.Close();
	//�ָ������趨
	setlocale(LC_CTYPE, old_locale);
	free(old_locale);
	//�ָ��ַ�����//


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

//д���ļ�
void CInfoFile::WirteDocline(CString& type, double& x_floor, double& x_ceil, double& y_floor, double& y_ceil, double& theta_floor, double& theta_ceil, int& frame_length, int& frame_width, int& image_threshold, int& image_Filter_block_radius, int& image_closing_radius, int& baoguang_time, int& baoguang2_time)
{
	//��ȡ�����趨
	char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
	//�趨���������ġ�����
	setlocale(LC_CTYPE, "chs");
	//�����ļ������ı���ʽ��
	CStdioFile csdioFile;
	BOOL flag = csdioFile.Open(_T(".\\stock.txt"), CFile::modeReadWrite);
	//д�������������
	csdioFile.WriteString(type);

	//�ָ������趨
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



	//ofs.close();//�ر��ļ�
}

CString CInfoFile::DoubleToCString(double x)
{
	// TODO: �ڴ˴����ʵ�ִ���.
	char temp[1024];
	CString sTemp;
	_gcvt_s(temp, 12, x, 8);//������תΪ�ַ���
	sTemp = CA2CT(temp);
	return sTemp;
}


CString CInfoFile::IntToCString(int x)
{
	// TODO: �ڴ˴����ʵ�ִ���.
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
	CString str;//���ڽ��շָ��ַ�����ʱ����
	CString strline;

	//��ȡ�����趨
	char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
	//�趨���������ġ�����
	setlocale(LC_CTYPE, "chs");
	CStdioFile file;//����һ��CStdioFile��Ķ��� file
	BOOL flag = file.Open(_T(".\\editframelocation.txt"), CFile::modeRead);//open������Ҫ������������ǰһ�����ļ�·������һ�����ļ��Ĵ�ģʽ
	if (flag == FALSE)
	{
		//MessageBox(_T("�ļ���ʧ��"));
	}
	while (file.ReadString(strline))
	{
		vecResult.push_back(strline);
	}
	file.Close();
	//�ָ������趨
	setlocale(LC_CTYPE, old_locale);
	free(old_locale);
	//�ָ��ַ�����//


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



//д���ļ�
void CInfoFile::WirteEditFrameLocation(CString aaa, int& edit_frame_left_x, int& edit_frame_left_y, int& edit_frame_right_x, int& edit_frame_right_y)
{
	//��ȡ�����趨
	char* old_locale = _strdup(setlocale(LC_CTYPE, NULL));
	//�趨���������ġ�����
	setlocale(LC_CTYPE, "chs");
	//�����ļ������ı���ʽ��
	CStdioFile csdioFile;
	BOOL flag = csdioFile.Open(_T(".\\editframelocation.txt"), CFile::modeReadWrite);
	//д�������������
	csdioFile.WriteString(aaa);

	//�ָ������趨
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