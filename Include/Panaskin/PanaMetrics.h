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
		PASK_METRICS_BTN_POS_RIGHTFRAME,  //���½ǰ�ť���봰���ұ�Ե�ľ���
		PASK_METRICS_BTN_POS_BOTTOMFRAME, //���½ǰ�ť���봰�ڵײ���Ե�ľ���
		PASK_METRICS_BTN_POS_SEPRATE,     //��ť֮�����ļ��
		PASK_METRICS_BTN_POS_HEIGHT,      //��ť�ı�׼�߶�
		PASK_METRICS_BTN_POS_WIDTH,       //��ť�ı�׼���
		PASK_METRICS_CLIENT_LEFT,         //���ڿͻ�����ؼ��ɰڷ�λ�þ������Ե��������
		PASK_METRICS_CLIENT_TOP,          //���ڿͻ�����ؼ��ɰڷ�λ�þ��붥����Ե��������
		PASK_METRICS_CLIENT_BOTTOM,       //���ڿͻ�����ؼ��ɰڷ�λ�þ���ײ���Ե��������
		PASK_METRICS_CLIENT_RIGHT,        //���ڿͻ�����ؼ��ɰڷ�λ�þ����ұ�Ե��������
		PASK_METRICS_CLIENT_SEPRATE,      //���ڿͻ�����ָ��߾���ײ���Ե��������
		PASK_METRICS_FRAME_LEFT,          //�����ͻ�����ؼ��ɰڷ�λ�þ������Ե��������
		PASK_METRICS_FRAME_TOP,           //�����ͻ�����ؼ��ɰڷ�λ�þ��붥����Ե��������
		PASK_METRICS_FRAME_BOTTOM,        //�����ͻ�����ؼ��ɰڷ�λ�þ���ײ���Ե��������
		PASK_METRICS_FRAME_RIGHT,         //�����ͻ�����ؼ��ɰڷ�λ�þ����ұ�Ե��������
		PASK_METRICS_TEXT_POS_BOTTOMCTRL, //�ı���ǩ�������汻˵���Ŀؼ��ľ���
		PASK_METRICS_TEXT_POS_RIGHTCTRL,  //�ı���ǩ�����ұ߱�˵���Ŀؼ��ľ���
		PASK_METRICS_CTRL_POS_SEPRATE_HORZ,  //ˮƽ���������ؼ���ļ������
		PASK_METRICS_CTRL_POS_SEPRATE_VERT,  //��ֱ���������ؼ���ļ������
		
	}; 

	

	static UINT GetMetrics(PASK_UI_METRICS metrics);
	static CFont* GetDlgFont();
};

#endif // !defined(AFX_PANAMETRICS_H__C30BEB69_A4FD_4137_A604_A5CF078E457E__INCLUDED_)
