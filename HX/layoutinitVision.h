#ifndef _LAYOUT_INIT_H_
#define _LAYOUT_INIT_H_

#include "CLayout.h"
#include "Resource.h"
void InitLayoutVision(CLayout& layout, CWnd* parent)
{
	layout.Initial(parent);
	
	layout.RegisterControl(IDC_VIS_BTN_OPMON, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_VIS_BTN_OPCAD, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_VIS_BTN_OPDATA, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_VIS_BTN_OPMOD, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_VIS_BTN_OPVS, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_VS_PIC_LEFT, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_VS_PIC_RIGHT, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_VS_LIST_LOCATE, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_VS_BTN_RESEND, CLayout::e_stretch_all);

	layout.RegisterControl(IDC_VS_STATIC_NAME, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_VS_PIC_LOGO, CLayout::e_stretch_all);

	layout.RegisterControl(IDC_VS_STATIC_TYPE, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_VS_STATIC_BATCH, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_VS_EDIT_TYPE, CLayout::e_stretch_all);
	layout.RegisterControl(IDC_VS_EDIT_BATCH, CLayout::e_stretch_all);
}

#endif
