#ifndef TPSCLOBALCONST_H
#define TPSCLOBALCONST_H

#include "vtkHeader.h"

enum FILE_FORMAT
{
	DICOM=0,
	XINAOMDT,
	VTKVTI
};

typedef enum _TagModalType
{
	ModalDrag = 100,
	ModalSeg  = 200,
	ModalContour = 300,
	ModalVolume = 400
}MODALTYPE;

typedef enum _TagZoneType
{
	TUMOR = 0,
	ORGAN
}ZONETYPE;

typedef enum _TagPolyType
{
	TOOL = 0,
	TRACER,
	MODAL
}POLYTYPE;

typedef struct poly_info
{
	int iIndex;
	int iSliceIndex;
	int textVisible;
	char sDscr[100];
	int imageIndex;
	short grey;
}POLY_INFO;

typedef struct modal_info
{
	char name[100];
	HTREEITEM treeItem;
	ZONETYPE zType;
	COLORREF color;
	double volume;
	int Opacity;
}MODAL_INFO;


struct IMAGE_INFO
{
	BOOL bValid;
	double imageInScanner[16];
	char ImageName[256];
	char ImagePath[256];

	int ResolutionX,ResolutionY;
	int NumOfImage;	
	double FovX,FovY;
	double SliceThickness,SliceGap;
	double spacing[3];
	BOOL bHFlip,bVFlip;
	double fAngle;

	char MRIOriChar[4][10];	

	IMAGE_INFO()
	{
		bValid=FALSE;
		ImageName[0]='\0';
		ImagePath[0]='\0';
	}
};

typedef struct  Actor_info{
	int iSliceIndex;
	vtkActor* p;
	vtkFollower* pFollower;
	int imageIndex;
	COLORREF color;
}ACTOR_INFO;

typedef struct zonegroup_info
{
	HTREEITEM treeItem;
	char groupName[100];
	int groupIndex;
	int iImageIndex;
	
	COLORREF color;
	ZONETYPE zType;
	COLORREF textColor;
	double textScale;
	short grey;
}ZONEGROUP_INFO;

typedef struct Ball_shell {
	float Height;
	float Diameter;
	float doseOffset;
	float TipPos;
	int Opacity;
	float EffectContour;
	int Index;
	COLORREF Color;
	int Visibility;
}BALL_SHELL;

typedef struct NeedleInfo
{
	char name[100];
	double needleLength;
	double needleDiamter;
	
	
	int type; //0:biotype 1:cryotype
	BALL_SHELL shell1,shell2,shell3;
} NEEDLE_INFO;

typedef struct IntersectLine_Property 
{
	float Opacity;
	int Visibility;
	double Color[3];
	int PlaneIndex;
	double Area;
	int ShellIndex;
	int ImageIndex;
}INTERSECTLINE_PROPERTY;

#endif