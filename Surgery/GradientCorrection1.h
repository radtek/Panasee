// GradientCorrection.h: interface for the CGradientCorrection1 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRADIENTCORRECTION1_H__7D830575_C161_4C66_AB06_EECC9957E918__INCLUDED_)
#define AFX_GRADIENTCORRECTION1_H__7D830575_C161_4C66_AB06_EECC9957E918__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "MRISeries.h"

//class CMRISeries;

class  CGradientCorrection1  
{
public:
	CGradientCorrection1();
	virtual ~CGradientCorrection1();
 
	//int GradientCorrection( int nNumOfImages, CBIImageHeader* pImageHeader, short** ppData );
	int GradientCorrection(CMRISeries* pImage);
	//static CGradientCorrection1 s_GradientCorrection;
	//static CGradientCorrection1& GetHandle();

private:

	int interp3(float* im1vec, short ***in_mvol,float *m_C1,float *m_R1,float *m_S1,CString interp_m, int rsize, int csize, int ssize);

	//Load look-up table
	//sFileNmae[in]: File name of look-up table created before unwarping.
	int LoadMRIUnwarpData(CString sFileName); 
	
	//Load Images data and position and orientation  
	int LoadMRISeries2Vol(int nNumOfImages, MR_ImageHeader* pImageHeader, short** ppData, short** m_mVol,float** m_M);  
	
	//Converts a 0-based vox2ras matrix to 1-based
	int vox2ras_0to1(float** ppM0, float** ppM1);

	int rcs2index(int SizeRCS,int* R,int dr,int* C,int dc,
		int* S, int ds,int nr,int nc,int ns,int* indexvec);
	
	int interp_trilin(int SizeRCS,float* R,float* C,float* S,float* M,
		int nr,int nc,int ns,float* V_out);

	
	int proj(int unwarpdims, float** ppMdc,float** Din,int size, 
		float* dx, float* dy, float* dz);

	int jdproj(float** JDin,int size,float** ppMdc,int unwarpdims,int maxjd,
		float* JDout);

	//Unwarp the images	
	int unwarp_resample(short** imvol,float** ppM1,int volsize[3],/*int nrows_out,int ncols_out,int nslices_out,*/
		float** ppMout,float** ppMdc,int Jacobianflag,int inflag,int thruflag,float** imvol_out
		);


	int vox2ras_xform(float** M,int size_vol[3],float** ppMdc,float* c_ras);	
	
	int ncoords_x; 
	int ncoords_y;
	int ncoords_z;
	float coords_x[2]; // Field of correction in m;
	float coords_y[2]; // Field of correction in m;
	float coords_z[2]; // Field of correction in m;
	
	float* Dx; 
	float* Dy;
	float* Dz;
	float* JacDetx;
	float* JacDety;
	float* JacDetz;

	CString m_sRefDataFileName;
	bool m_bLoadedUnwarpRefData;
	
};

#endif // !defined(AFX_GRADIENTCORRECTION_H__7D830575_C161_4C66_AB06_EECC9957E918__INCLUDED_)
