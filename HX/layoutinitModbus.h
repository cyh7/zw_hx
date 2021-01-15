#ifndef _LAYOUT_INIT_H_
#define _LAYOUT_INIT_H_

#include "CLayout.h"

void InitLayoutModbus(CLayout& layout, CWnd* parent)
{
	layout.Initial(parent);

	layout.RegisterControl(IDC_COMBO1, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_COMBO2, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_COMBO3, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_COMBO4, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_COMBO5, CLayout::e_stretch_all);

	//layout.RegisterControl(IDC_STATIC1, CLayout::e_stretch_all);
	//layout.RegisterControl(IDC_STATIC2, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_STATIC3, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_STATIC4, CLayout::e_stretch_all);

	//����ѡ���
	layout.RegisterControl(IDC_STATIC5, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_STATIC6, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_STATIC7, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_STATIC8, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_STATIC9, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_STATIC10, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_STATIC11, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_STATIC12, CLayout::e_stretch_all);

	//layout.RegisterControl(IDC_LIST1, CLayout::e_stretch_all);
	//layout.RegisterControl(IDC_LIST2, CLayout::e_stretch_all);

	layout.RegisterControl(IDC_EDIT1, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_EDIT2, CLayout::e_stretch_all);

	layout.RegisterControl(IDC_BUTTON1, CLayout::e_stretch_all);
	
	layout.RegisterControl(IDC_BUTTON_OPEN, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_BUTTON_SEND_ONCE, CLayout::e_stretch_all);


	//layout.RegisterControl(IDC_BUTTON3, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_BUTTON_CLEAN, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_MOD_BTN_OPCAM, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_MOD_BTN_CLOSECAM, CLayout::e_stretch_all);
	//�·���ť��
	layout.RegisterControl(IDC_MOD_BTN_OPMON, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_MOD_BTN_OPVS, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_MOD_BTN_OPCAD, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_MOD_BTN_OPDATA, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_MOD_BTN_OPMOD, CLayout::e_stretch_all);
	//������
	layout.RegisterControl(IDC_MOD_BTN_CHANGE, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_STATIC13, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_STATIC14, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_STATIC15, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_STATIC16, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_STATIC17, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_STATIC18, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_STATIC19, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_STATIC20, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_STATIC21, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_STATIC22, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_STATIC23, CLayout::e_stretch_all);

	layout.RegisterControl(IDC_STATIC24, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_STATIC25, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_STATIC26, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_STATIC27, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_EDIT_TYPE, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_EDIT_FRAMELENGTH, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_EDIT_FRAMEWIDTH, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_EDIT_IMAGETHRESHOLD, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_EDIT_Filter_radius, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_EDIT_IMAGE_close_radius, CLayout::e_stretch_all);

	layout.RegisterControl(IDC_EDIT_XFLOOR, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_EDIT_YFLOOR, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_EDIT_THETAFLOOR, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_EDIT_XCEILING, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_EDIT_YCEILING, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_EDIT_THETACEILING, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_EDIT_BAOGUANG, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_EDIT_BAOGUANG2, CLayout::e_stretch_all);

	layout.RegisterControl(IDC_MOD_STATIC_NAME, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_MOD_PIC_LOGO, CLayout::e_stretch_all);
}

#endif