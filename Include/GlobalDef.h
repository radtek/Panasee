
#ifndef _PANA_GLOBALDEF_H_
#define _PANA_GLOBALDEF_H_

//自定义消息
#define WM_TRACKING				WM_USER + 1   
//WM_USER +2已经被使用
#define WM_MRI_PLAY				WM_USER + 3
#define WM_SCAN_PROGRESS		WM_USER + 4
#define WM_COLLECTION_DONE		WM_USER + 5
#define WM_SIMULATE_SPHERE		WM_USER + 6 
#define WM_STOPTRACKING_FROMTHREAD		WM_USER + 7 

#define WM_WIZARD_STEP			WM_USER + 10

#define WM_XBDICOMMSG     WM_USER + 26

#define WM_DCMSERVICE_MSG  WM_USER+30

// WM_USER +32 WM_USER+33已经被使用

//// WM_USER +100 WM_USER+150已经被使用


#define WM_SOCKET				WM_USER + 200
#define WM_HARDWARE_READY		WM_USER + 201  // indicate all hardware have connected and ready to use
#define WM_START_WIZARD			WM_USER + 203
#define WM_MRI_PATIENT_INFO		WM_USER	+ 204

#define WM_TIME_UPDATE			WM_USER + 205
#define WM_SERIES_CRASH_UPDATE  WM_USER + 206

#define WM_NEW_SERIES			WM_USER + 210
#define WM_MRI_STATUS			WM_USER + 211

#define WM_SYS_MENU_POPUP		WM_USER + 220
#define WM_SYS_DB_OK			WM_USER + 221
#define WM_CHANGETRACEGROUP     WM_USER + 222

#define WM_SHOW_SERIES          WM_USER+223
#define WM_CALL_COMMAND         WM_USER+224
#define WM_IMAGEPROCESS_END     WM_USER+225
#define	WM_INTIME_SEGMENTED	    WM_USER+226
#define WM_LINECHART_NOTIFY     WM_USER+227
#define WM_CONTOUR_UPDATE	    WM_USER+228
#define WM_VOI_UPDATE           WM_USER+229
#define WM_VOI_SLICE_UPDATE     WM_USER+230
#define WM_SEGMENT_UPDATE       WM_USER+231
#define WM_VOLUMERENDER_UPDATE  WM_USER+232

#define	WM_TPS_GET_SERIES       WM_USER+233
#define	WM_TPS_RECEIVE_SERIES   WM_USER+234
#define WM_TPS_PROBE_MOVING     WM_USER+235
#define WM_VIMAGE_SEG_BTN_COLOR WM_USER+236
#define WM_VIMAGE_SEG_EDIT_NAME WM_USER+237
#define WM_VIMAGE_SEG_SLIDER_OPACITY WM_USER+238 
#define WM_VIMAGE_SEG_CHECK		WM_USER+239
#define WM_DICOM_IMAGECOUNT     WM_USER+240

#define WM_SYS_UPDATE_WL        WM_USER+243
#define WM_SYS_UPDATE_TARGET    WM_USER+244
#define WM_SYS_UPDATE_DEPTH     WM_USER+245
#define WM_SYS_UPDATE_ANGLE     WM_USER+246
#define WM_FUSION_UPDATECROSS   WM_USER+247
#define WM_FUSION_GETSERIESLIST WM_USER+248
#define WM_FUSION_GETSERIES     WM_USER+249
#define WM_FUSION_SYNCPOINT     WM_USER+250
#define WM_FUSION_CURPOINT      WM_USER+251
#define WM_FUSION_POINT_POS     WM_USER+252
#define WM_FUSION_TRACKDATA     WM_USER+253
#define WM_SYS_CHILDWINDOW_LBTNUP WM_USER+254
#define WM_SYS_DEPTHSLIDER_SHOW WM_USER+255
#define WM_SYS_DEPTHSLIDER_HIDE WM_USER+256



#define WM_DIALOG_PATINFO_MSG		WM_USER + 455

#define WM_PANA_MOVECTRL            WM_USER+1001
#define WM_PANA_GETPARENTBK         WM_USER+1002
#define WM_PANA_MOVECTRL_LBTN_UP    WM_USER+1003
#define WM_PANA_MOVECTRL_LBTN_DOWN  WM_USER+1004

// WM_USER +1976 ~ WM_USER+1980已经被使用

#define WM_IMRISOCK_MSG     WM_USER + 3001
#define WM_IMRINET_MSG             WM_USER + 3002
#define WM_IMRIUL_MSG             WM_USER + 3003

#define WM_PROTCOL_START		(WM_USER + 7000)
#define WM_PROTCOL_END			(WM_USER + 7500)

#define WM_PROTCOL_FAMILY_START	(WM_USER + 7501)
#define WM_PROTCOL_FAMILY_END	(WM_USER + 7550)

#define WM_PROBE_START			(WM_USER + 8001)
#define WM_PROBE_END			(WM_USER + 8100)

//// WM_USER +9000以上已经被使用

//定时器
#define MY_ELAPSE200 200
#define MY_ELAPSE100 100
#define STATUSBAR_UPDATE_TIMER   1000
#define INDICATOR_UPDATE_TIMER1  1001
#define INDICATOR_UPDATE_TIMER2  1002
#define INDICATOR_UPDATE_TIMER3  1003
#define INDICATOR_PT_TIMER       1004
#define INDICATOR_UPDATE_TIMER4  1005
#define INDICATOR_TPS_PROBE      1006
#define INDICATOR_FUSION_STATUS  1007
#define INDICATOR_LOAD_TPS       1008

typedef struct _IMAGE_EXCHANGE_INFO
{
	int dim[3];
	double space[3];
	unsigned short *data;
	char  name[256];
	double coord[4][4]; //图像相对于影像扫描设备的坐标
}IMAGE_EXCHANGE_INFO;

typedef struct _IMAGE_CONTOUR_INFO
{
	short contourValue;
	COLORREF color;
	int VOI[6];
	BOOL  bLargest;
}IMAGE_CONTOUR_INFO;

struct TRACKING_DATA
{
	BOOL bProbeVisible;
	BOOL bMarkerVisible;
	BOOL bBaseVisible;
	double dProbe2Tracker[4][4];
	double dMarker2Tracker[4][4];
	double dBase2Tracker[4][4];
	double dScanner2Marker[4][4];

	TRACKING_DATA()
	{
		bProbeVisible=FALSE;
		bMarkerVisible=FALSE;
		bBaseVisible=FALSE;
		for(int i=0;i<4;i++)
		{
			for(int j=0;j<4;j++)
			{
				dProbe2Tracker[i][j]=0;
				dMarker2Tracker[i][j]=0;
				dBase2Tracker[i][j]=0;
				dScanner2Marker[i][j]=0;
				if(i==j)
				{
					dProbe2Tracker[i][j]=1;
					dMarker2Tracker[i][j]=1;
					dBase2Tracker[i][j]=1;
					dScanner2Marker[i][j]=1;
				}
			}
		}
	}

	TRACKING_DATA& operator=(TRACKING_DATA &temp)
	{
		bProbeVisible=temp.bProbeVisible;
		bMarkerVisible=temp.bMarkerVisible;
		bBaseVisible=temp.bBaseVisible;
		for(int i=0;i<4;i++)
		{
			for(int j=0;j<4;j++)
			{
				dProbe2Tracker[i][j]=temp.dProbe2Tracker[i][j];
				dMarker2Tracker[i][j]=temp.dMarker2Tracker[i][j];
				dBase2Tracker[i][j]=temp.dBase2Tracker[i][j];
				dScanner2Marker[i][j]=temp.dScanner2Marker[i][j];
			}
		}

		return *this;
	}
};


struct MESH_OPTIONS
{
	
	// Begin render switches
	BOOL bUseGaussianSmoothing;
	BOOL bUseDecimation;
	BOOL bUseMeshSmoothing;
	// Begin gsmooth params
	float fGaussianStandardDeviation;
	float fGaussianError;
	
	// Begin decimate parameters
	float fDecimateTargetReduction;
	float fDecimateInitialError;
	float fDecimateAspectRatio;
	float fDecimateFeatureAngle;
	float fDecimateErrorIncrement;
	unsigned int iDecimateMaximumIterations;
	BOOL bDecimatePreserveTopology;
	
	// Begin msmooth params
	float fMeshSmoothingRelaxationFactor;
	unsigned int iMeshSmoothingIterations;
	float fMeshSmoothingConvergence;
	float fMeshSmoothingFeatureAngle;
	BOOL bMeshSmoothingFeatureEdgeSmoothing;
	BOOL bMeshSmoothingBoundarySmoothing;

	COLORREF color;
	float opacity;
	short label;
	BOOL  bLargest;
	int VOI[6];
	char name[256];

	MESH_OPTIONS()
	{
		
		bLargest=FALSE;

		color=RGB(255,0,255);
		opacity=1.0;
		label=255;

		bUseGaussianSmoothing = FALSE;
		bUseDecimation = TRUE;
		bUseMeshSmoothing = TRUE;

		// Begin gsmooth params
		fGaussianStandardDeviation = 0.8f;
		fGaussianError = 0.03f;

		// Begin decimate params
		fDecimateTargetReduction = 0.95f;
		fDecimateInitialError = 0.002f;
		fDecimateAspectRatio = 20.0f;
		fDecimateErrorIncrement = 0.002f;
		iDecimateMaximumIterations = 1;
		fDecimateFeatureAngle = 45;
		bDecimatePreserveTopology = TRUE;
   
		// Begin msmooth params
		iMeshSmoothingIterations = 1;
		fMeshSmoothingRelaxationFactor = 0.01f;
		fMeshSmoothingFeatureAngle = 45;
		fMeshSmoothingConvergence = 0;
		bMeshSmoothingFeatureEdgeSmoothing = FALSE;
		bMeshSmoothingBoundarySmoothing = FALSE;
	}

	MESH_OPTIONS& operator=(MESH_OPTIONS &temp)
	{
		bUseGaussianSmoothing =temp.bUseGaussianSmoothing;
		bUseDecimation =temp.bUseDecimation;
		bUseMeshSmoothing =temp.bUseMeshSmoothing;
		fGaussianStandardDeviation =temp.fGaussianStandardDeviation;
		fGaussianError =temp.fGaussianError;
		
		fDecimateTargetReduction =temp.fDecimateTargetReduction;
		fDecimateInitialError =temp.fDecimateInitialError;
		fDecimateAspectRatio =temp.fDecimateAspectRatio;
		fDecimateFeatureAngle =temp.fDecimateFeatureAngle;
		fDecimateErrorIncrement =temp.fDecimateErrorIncrement;
		iDecimateMaximumIterations =temp.iDecimateMaximumIterations;
		bDecimatePreserveTopology =temp.bDecimatePreserveTopology;
		
		fMeshSmoothingRelaxationFactor =temp.fMeshSmoothingRelaxationFactor;
		iMeshSmoothingIterations =temp.iMeshSmoothingIterations;
		fMeshSmoothingConvergence =temp.fMeshSmoothingConvergence;
		fMeshSmoothingFeatureAngle =temp.fMeshSmoothingFeatureAngle;
		bMeshSmoothingFeatureEdgeSmoothing =temp.bMeshSmoothingFeatureEdgeSmoothing;
		bMeshSmoothingBoundarySmoothing =temp.bMeshSmoothingBoundarySmoothing;
		
		color =temp.color;
		opacity =temp.opacity;
		label =temp.label;
		bLargest =temp.bLargest;
		for(int i=0;i<6;i++)
		{
			VOI[i] =temp.VOI[i];
		}
		for(i=0;i<256;i++)
		{
			name[i]=temp.name[i];
		}
		
		return *this;
	}
};


enum ENUM_VOLRENDER_MODE
{
	VOLRENDER_MIP,
	VOLRENDER_MINIP,
	VOLRENDER_VOLUME
};

enum ENUM_VOLRENDER_CROP
{
	ENUM_VOLRENDER_CROP_SUBVOLUME,
	ENUM_VOLRENDER_CROP_FENCE,
	ENUM_VOLRENDER_CROP_INVERTED_FENCE,
	ENUM_VOLRENDER_CROP_CROSS,
	ENUM_VOLRENDER_CROP_INVERTED_CROSS
};

struct IA_PIXEL_ITEM
{
	double pixel;  //像素值
	double opacity; //像素值对应的透明度
	COLORREF color; //像素值对应的颜色
	int count;  //像素值对应的像素个数，画直方图使用
	
};

struct VOLRENDER_CONFIG
{
	BOOL bShade;
	BOOL bGradient;
	ENUM_VOLRENDER_MODE renderMode;

	IA_PIXEL_ITEM opacityColorList[256];
	int opacityListCount;
	IA_PIXEL_ITEM gradList[256];
	int gradListCount;

	float LOD;
	short sampleDistance;
	float lowResLODFactor;
	float lodRange[2];

	double dCropPlane[6];
	BOOL   bCrop;
	ENUM_VOLRENDER_CROP cropMode;
		
	double diffuse;
	double ambient;
	double specular;
	double specularPower;

	VOLRENDER_CONFIG()
	{
		opacityListCount=0;
		gradListCount=0;

		LOD=1;
		sampleDistance=1;
		lowResLODFactor=1.5;

		lodRange[0]=1.0;
		lodRange[1]=4.0;

		renderMode=VOLRENDER_VOLUME;
		bGradient=FALSE;
		bShade=TRUE;

		bCrop=FALSE;
		cropMode=ENUM_VOLRENDER_CROP_SUBVOLUME;
		
		diffuse=1.0;
		ambient=0.20;
		specular=0;
		specularPower=1;

	}

	VOLRENDER_CONFIG& operator=(VOLRENDER_CONFIG &temp)
	{
		opacityListCount=temp.opacityListCount;
		gradListCount=temp.gradListCount;
		
		LOD=temp.LOD;
		sampleDistance=temp.sampleDistance;
		lowResLODFactor=temp.lowResLODFactor;
		
		lodRange[0]=temp.lodRange[0];
		lodRange[1]=temp.lodRange[1];
		
		renderMode=temp.renderMode;
		bGradient=temp.bGradient;
		bShade=temp.bShade;
		
		

		diffuse=temp.diffuse;
		ambient=temp.ambient;
		specular=temp.specular;
		specularPower=temp.specularPower;

		//bCrop=temp.bCrop;
		//cropMode=temp.cropMode;
		for (int k=0;k<6;k++)
		{
			//dCropPlane[k]=temp.dCropPlane[k];
		}
		for (int m=0;m<256;m++)
		{
			gradList[m]=temp.gradList[m];
		}
		for(int i=0;i<2;i++)
		{
			lodRange[i]=temp.lodRange[i];
		}
		for (int j=0;j<256;j++)
		{
			opacityColorList[j]=temp.opacityColorList[j];
		}
		
		return *this;
	}
};

#endif //_PANA_GLOBALDEF_H_
