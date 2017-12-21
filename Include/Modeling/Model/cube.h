#pragma once

#include "../Common/Geometry.h"
#include <list>
class Cube
{
public:
	/////////////////////////////////////// sweeping
	bool flag;                  // ��־Sweeping���Ƿ���� true:������ false:����
	int numOfUpdate;            // ���´���, Sweeping�й��˴�ɨ��,ÿ��ɨ���ֵ��1,���֮��Ϊ8
	Vector3D point;				// ͶӰ��(PolygonArray)
	double dis;					// Cube�����ĵ㵽ͶӰ��ľ���
	std::list<QuadCell3D*> quadCellList; // �����͵��б�
	unsigned int xLocateCode;   // x����Ķ����Ʊ���
	unsigned int yLocateCode;   // y����Ķ����Ʊ���
	unsigned int zLocateCode;	// z����Ķ����Ʊ���
	std::list<Cube*> neighbors;	// ���е��ھ��б�
	double MAX_DIS;             // �洢��ǰ�������б��м���������ƽ������
	/////////////////////////////////////// atribute
	double length;				// Cube�Ŀ��
	Vector3D lllpoint;			// Cube�����½ǵ�
	int level;					// Cube���ڵĲ���
	Vector3D ctpoint;			// Cube�����ĵ�
	Cube * root;				// ָ�򸸽ڵ�
	Cube * leaf[2][2][2];		// ָ��˸�Ҷ�ӽڵ��ָ��
	bool hasLeaf;				// �Ƿ���Ҷ�ӽڵ�
	bool fSplit;				// �Ƿ�Ӧ�ñ�������(�п�����Ϊ����������,��ʹ��Ҫ��������Ҳû��Ҷ�ӽڵ�)
public:
	Cube(Vector3D lp, double lg, int lv, Cube * rt);   
	Cube(Vector3D lp, double lg, int lv, Cube * rt, int elv, int x, int y, int z);   // x,y,z Ϊx,y,z�����ϵı���
	virtual ~Cube();
	void Split();
	void Split(int elv);
	bool InCube(Vector3D v);
	//bool IsCrossedByLine( Line2D line ); // �Ƿ��line�ཻ
	//bool InPolygon( Polygon2D poly );   // Cube���ĸ������Ƿ���Polygon�ڲ�
	//bool OutPolygon( Polygon2D poly );  // Cube���ĸ������Ƿ���Polygon�ⲿ
	//bool IsCrossedByPolygon( Polygon2D poly ); // Cube�Ƿ���Polygon�ı��ཻ
	void AddNeighbor( Cube* pNeighbor );
	void PushQuadCell( QuadCell3D* pqc );
private:
	// �õ�Cube�ĸ����㵽�����͵�������С����
	// ����ֵtrue��ʾ����ɹ�����ֵ������,
	// false��ʾΪ�߶β���ͶӰ�㲻���߶���,����ֵ������
	//bool GetSquaredDisFromQuadCell( QuadCell2D* pqc, double& minDis, double& maxDis );
};
