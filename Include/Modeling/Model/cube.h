#pragma once

#include "../Common/Geometry.h"
#include <list>
class Cube
{
public:
	/////////////////////////////////////// sweeping
	bool flag;                  // 标志Sweeping中是否更新 true:不更新 false:更新
	int numOfUpdate;            // 更新次数, Sweeping中共八次扫描,每次扫描该值增1,完毕之后为8
	Vector3D point;				// 投影点(PolygonArray)
	double dis;					// Cube的中心点到投影点的距离
	std::list<QuadCell3D*> quadCellList; // 二次型的列表
	unsigned int xLocateCode;   // x方向的二进制编码
	unsigned int yLocateCode;   // y方向的二进制编码
	unsigned int zLocateCode;	// z方向的二进制编码
	std::list<Cube*> neighbors;	// 所有的邻居列表
	double MAX_DIS;             // 存储当前二次型列表中计算出的最大平方距离
	/////////////////////////////////////// atribute
	double length;				// Cube的宽度
	Vector3D lllpoint;			// Cube的左下角点
	int level;					// Cube所在的层数
	Vector3D ctpoint;			// Cube的中心点
	Cube * root;				// 指向父节点
	Cube * leaf[2][2][2];		// 指向八个叶子节点的指针
	bool hasLeaf;				// 是否有叶子节点
	bool fSplit;				// 是否应该被继续分(有可能因为层数的限制,即使需要被继续分也没有叶子节点)
public:
	Cube(Vector3D lp, double lg, int lv, Cube * rt);   
	Cube(Vector3D lp, double lg, int lv, Cube * rt, int elv, int x, int y, int z);   // x,y,z 为x,y,z方向上的编码
	virtual ~Cube();
	void Split();
	void Split(int elv);
	bool InCube(Vector3D v);
	//bool IsCrossedByLine( Line2D line ); // 是否和line相交
	//bool InPolygon( Polygon2D poly );   // Cube的四个顶点是否都在Polygon内部
	//bool OutPolygon( Polygon2D poly );  // Cube的四个顶点是否都在Polygon外部
	//bool IsCrossedByPolygon( Polygon2D poly ); // Cube是否与Polygon的边相交
	void AddNeighbor( Cube* pNeighbor );
	void PushQuadCell( QuadCell3D* pqc );
private:
	// 得到Cube四个顶点到二次型的最大和最小距离
	// 返回值true表示计算成功返回值有意义,
	// false表示为线段并且投影点不在线段上,返回值无意义
	//bool GetSquaredDisFromQuadCell( QuadCell2D* pqc, double& minDis, double& maxDis );
};
