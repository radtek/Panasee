// GradientCorrection.h: interface for the CGradientCorrection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRADIENTCORRECTION_H__7D830575_C161_4C66_AB06_EECC9957E918__INCLUDED_)
#define AFX_GRADIENTCORRECTION_H__7D830575_C161_4C66_AB06_EECC9957E918__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "MRISeries.h"

//class CMRISeries;

class CGradientCorrection  
{
public:
	BOOL Init(CString sRefDataFile);
	CGradientCorrection();
	virtual ~CGradientCorrection();
 
	int GradientCorrection(CMRISeries* pImage);

private:
	void RemoveStorage();
	void InitStorage();

	int interp3(float* im1vec, short **in_mvol,float *C1,float *R1,float *S1,CString interp_m, int rsize, int csize, int ssize);

	//Load look-up table
	//sFileNmae[in]: File name of look-up table created before unwarping.
	BOOL LoadMRIUnwarpData(CString sFileName); 
	
	//Load Images data and position and orientation  
	int LoadMRISeries2Vol(int nNumOfImages, MR_ImageHeader* pImageHeader, float** MOut);  
	
	//Converts a 0-based vox2ras matrix to 1-based
	int vox2ras_0to1(float** ppM0, float** ppM1);

	int rcs2index(int SizeRCS,int* R,int dr,int* C,int dc,
		int* S, int ds,int nr,int nc,int ns,int* indexvec);
	
	int interp_trilin(int SizeRCS,float* R,float* C,float* S,float* M1,float* M2,float* M3,float* M4,float* M5,float* M6,
						int nr,int nc,int ns, float* V_out1,float* V_out2,float* V_out3,float* V_out4,float* V_out5,float* V_out6);
	
	int proj(int unwarpdims, float** ppMdc,float *Dx1,float *Dy1,float *Dz1,int size, 
		float* dx, float* dy, float* dz);

	int jdproj(float* JDinx,float* JDiny,float* JDinz,int size,float** ppMdc,int unwarpdims,int maxjd,
		float* JDout);

	//Unwarp the images	
	int unwarp_resample(short** imvol,float** ppM1,int volsize[3],/*int nrows_out,int ncols_out,int nslices_out,*/
		float** ppMout,float** ppMdc,int Jacobianflag,int inflag,int thruflag,float** imvol_out
		);


	int vox2ras_xform(float** M,float** ppMdc);	
	
	int m_ncoords_x; 
	int m_ncoords_y;
	int m_ncoords_z;
	float m_fcoords_x[2]; // Field of correction in m;
	float m_fcoords_y[2]; // Field of correction in m;
	float m_fcoords_z[2]; // Field of correction in m;
	
	float* m_Dx; 
	float* m_Dy;
	float* m_Dz;
	float* m_JacDetx;
	float* m_JacDety;
	float* m_JacDetz;

	
	BOOL m_bLoadedUnwarpRefData;
	int m_size_vol[3];

	//for unwarp_resample()
	int m_NumVoxel;
	float *m_R1;
	float *m_C1;
	float *m_S1;
	float *m_im1vec;
	float* m_im1vec2;
	float* m_R;
	float* m_C;
	float* m_imindx;
	float* m_X;
	float* m_Y;
	float* m_Z;
	float* m_Rgw;
	float* m_Cgw;
	float* m_Sgw;
	float* m_Dx1;
	float* m_Dy1;
	float* m_Dz1;
	float* m_Dx2;
	float* m_Dy2;
	float* m_Dz2;
	float* m_JacDetx1;
	float* m_JacDety1;
	float* m_JacDetz1;
	float* m_JacDet1;
	float* m_imjdvec;
	float* m_nflag;
	
};

#endif // !defined(AFX_GRADIENTCORRECTION_H__7D830575_C161_4C66_AB06_EECC9957E918__INCLUDED_)
