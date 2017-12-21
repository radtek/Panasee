#ifndef _MODEL_H_
#define _MODEL_H_

#include "../Common/Geometry.h"
class CModel
{
public:
	CModel() { m_bVisible = TRUE;}

public:
	Vector3D m_vTranslation;//ƫ����
	Vector3D m_vRotation;//��ת
	double m_color[3];//�������ɫ

	BOOL IsVisible() {return m_bVisible;};
	void SetVisible(BOOL bVisible) { m_bVisible = bVisible;};

private:
	BOOL  m_bVisible;

};

#endif