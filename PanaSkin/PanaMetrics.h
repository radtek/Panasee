// PanaMetrics.h: interface for the CPanaMetrics class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PANAMETRICS_H__C30BEB69_A4FD_4137_A604_A5CF078E457E__INCLUDED_)
#define AFX_PANAMETRICS_H__C30BEB69_A4FD_4137_A604_A5CF078E457E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "panaskin.h"

class PANASKIN_API CPanaMetrics  
{
public:
	CPanaMetrics();
	virtual ~CPanaMetrics();
	
		
	enum PASK_UI_METRICS {
		PASK_METRICS_BTN_POS_RIGHTFRAME,  //右下角按钮距离窗口右边缘的距离
		PASK_METRICS_BTN_POS_BOTTOMFRAME, //右下角按钮距离窗口底部边缘的距离
		PASK_METRICS_BTN_POS_SEPRATE,     //按钮之间横向的间隔
		PASK_METRICS_BTN_POS_HEIGHT,      //按钮的标准高度
		PASK_METRICS_BTN_POS_WIDTH,       //按钮的标准宽度
		PASK_METRICS_CLIENT_LEFT,         //窗口客户区域控件可摆放位置距离左边缘的像素数
		PASK_METRICS_CLIENT_TOP,          //窗口客户区域控件可摆放位置距离顶部边缘的像素数
		PASK_METRICS_CLIENT_BOTTOM,       //窗口客户区域控件可摆放位置距离底部边缘的像素数
		PASK_METRICS_CLIENT_RIGHT,        //窗口客户区域控件可摆放位置距离右边缘的像素数
		PASK_METRICS_CLIENT_SEPRATE,      //窗口客户区域分割线距离底部边缘的像素数
		PASK_METRICS_FRAME_LEFT,          //分组框客户区域控件可摆放位置距离左边缘的像素数
		PASK_METRICS_FRAME_TOP,           //分组框客户区域控件可摆放位置距离顶部边缘的像素数
		PASK_METRICS_FRAME_BOTTOM,        //分组框客户区域控件可摆放位置距离底部边缘的像素数
		PASK_METRICS_FRAME_RIGHT,         //分组框客户区域控件可摆放位置距离右边缘的像素数
		PASK_METRICS_TEXT_POS_BOTTOMCTRL, //文本标签距离下面被说明的控件的距离
		PASK_METRICS_TEXT_POS_RIGHTCTRL,  //文本标签距离右边被说明的控件的距离
		PASK_METRICS_CTRL_POS_SEPRATE_HORZ,  //水平方向两个控件间的间隔距离
		PASK_METRICS_CTRL_POS_SEPRATE_VERT,  //垂直方向两个控件间的间隔距离
		
	}; 

	

	static UINT GetMetrics(PASK_UI_METRICS metrics);
	static CFont* GetDlgFont();
};

#endif // !defined(AFX_PANAMETRICS_H__C30BEB69_A4FD_4137_A604_A5CF078E457E__INCLUDED_)
