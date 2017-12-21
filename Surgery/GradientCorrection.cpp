// GradientCorrection.cpp: implementation of the CGradientCorrection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GradientCorrection.h"
#include <fstream>
#include "math.h"
#include "float.h"
#include "ErrorCodes.h"
#include "MV.h" 


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifndef Min
#define Min(a,b)            (((a) < (b)) ? (a) : (b))
#endif
#ifndef Max
#define Max(a,b)            (((a) > (b)) ? (a) : (b))
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CGradientCorrection::CGradientCorrection()
{
	m_Dx = NULL;
	m_Dy = NULL;
	m_Dz = NULL;
	m_JacDetx = NULL;
	m_JacDety = NULL;
	m_JacDetz = NULL;
	
	m_bLoadedUnwarpRefData = FALSE;
		
	m_NumVoxel=0;
	m_R1=NULL;
	m_C1=NULL;
	m_S1=NULL;
	m_im1vec=NULL;
	m_im1vec2=NULL;
	m_R=NULL;
	m_C=NULL;
	m_imindx=NULL;
	m_X=NULL;
	m_Y=NULL;
	m_Z=NULL;
	m_Rgw=NULL;
	m_Cgw=NULL;
	m_Sgw=NULL;
	m_Dx1=NULL;
	m_Dy1=NULL;
	m_Dz1=NULL;
	m_Dx2=NULL;
	m_Dy2=NULL;
	m_Dz2=NULL;
	m_JacDetx1=NULL;
	m_JacDety1=NULL;
	m_JacDetz1=NULL;
	m_JacDet1=NULL;
	m_imjdvec=NULL;
	m_nflag=NULL;
	
}

CGradientCorrection::~CGradientCorrection()
{
	RemoveStorage();

	if ( NULL != m_Dx )
	{
		delete []m_Dx;
		m_Dx = NULL;
	}
	
	if ( NULL != m_Dy )
	{
		delete []m_Dy;
		m_Dy = NULL;
	}
	if ( NULL != m_Dz )
	{
		delete []m_Dz;
		m_Dz = NULL;
	}
	
	if ( NULL != m_JacDetx )
	{
		delete []m_JacDetx;
		m_JacDetx = NULL;
	}
	if ( NULL != m_JacDety )
	{
		delete []m_JacDety;
		m_JacDety = NULL;
	}
	if ( NULL != m_JacDetz )
	{
		delete []m_JacDetz;
		m_JacDetz = NULL;
	}
}

int CGradientCorrection::LoadMRISeries2Vol(int nNumOfImages, MR_ImageHeader* pImageHeader, float** MOut)
{
	
	int j;
	int nResolutionX = pImageHeader[0].seriesField.ResolutionX;
	int nResolutionY = pImageHeader[0].seriesField.ResolutionY;
	int nNumOfPts = nResolutionX*nResolutionY;
	Matrix M(4,4);
		
	float* ImageCentern = new float [3]; 
	float* ImageCenter1 = new float [3];
	float* ImageCenter2 = new float [3];
	
	float* dCenter;
	dCenter = new float[3];
	float* dFreqDirection;
	dFreqDirection = new float[3];
	float dFOV[3];
	float* dPhaseDirection;
	dPhaseDirection = new float[3];
	double x,y,z;
	
	MR_ImageHeader pMRIImage = pImageHeader[0];
	
	dCenter[0] = pMRIImage.imageField.fCenterPointX;
	dCenter[1] = pMRIImage.imageField.fCenterPointY;
	dCenter[2] = pMRIImage.imageField.fCenterPointZ;
	
	dFreqDirection[0] = pMRIImage.imageField.fFreqDirX; 
	dFreqDirection[1] = pMRIImage.imageField.fFreqDirY;
	dFreqDirection[2] = pMRIImage.imageField.fFreqDirZ;
	
	
	dFOV[0] = pMRIImage.seriesField.FovX;
	dFOV[1] = pMRIImage.seriesField.FovY;
	dFOV[2] = pMRIImage.seriesField.FovZ;
	
	dPhaseDirection[0] = pMRIImage.imageField.fPhaseDirX;
	dPhaseDirection[1] = pMRIImage.imageField.fPhaseDirY;
	dPhaseDirection[2] = pMRIImage.imageField.fPhaseDirZ;
	
	x = dCenter[0]-dFOV[1]/2*dFreqDirection[0]-dFOV[0]/2*dPhaseDirection[0];
	y = dCenter[1]-dFOV[1]/2*dFreqDirection[1]-dFOV[0]/2*dPhaseDirection[1];
	z = dCenter[2]-dFOV[1]/2*dFreqDirection[2]-dFOV[0]/2*dPhaseDirection[2];
	double sdc[3];
	double PixelSpacing[2];
	double dslice;
	
	ImageCenter1[0] = x;
	ImageCenter1[1] = y;
	ImageCenter1[2] = z;
	
	if 	(nNumOfImages>1)
	{
		pMRIImage = pImageHeader[1];
		dCenter[0] = pMRIImage.imageField.fCenterPointX;
		dCenter[1] = pMRIImage.imageField.fCenterPointY;
		dCenter[2] = pMRIImage.imageField.fCenterPointZ;
		
		x = dCenter[0]-dFOV[1]/2*dFreqDirection[0]-dFOV[0]/2*dPhaseDirection[0];
		y = dCenter[1]-dFOV[1]/2*dFreqDirection[1]-dFOV[0]/2*dPhaseDirection[1];
		z = dCenter[2]-dFOV[1]/2*dFreqDirection[2]-dFOV[0]/2*dPhaseDirection[2];
		
		ImageCenter2[0] = x;
		ImageCenter2[1] = y;
		ImageCenter2[2] = z;
		
		pMRIImage = pImageHeader[nNumOfImages-1];
		
		dCenter[0] = pMRIImage.imageField.fCenterPointX;
		dCenter[1] = pMRIImage.imageField.fCenterPointY;
		dCenter[2] = pMRIImage.imageField.fCenterPointZ;
		
		x = dCenter[0]-dFOV[1]/2*dFreqDirection[0]-dFOV[0]/2*dPhaseDirection[0];
		y = dCenter[1]-dFOV[1]/2*dFreqDirection[1]-dFOV[0]/2*dPhaseDirection[1];
		z = dCenter[2]-dFOV[1]/2*dFreqDirection[2]-dFOV[0]/2*dPhaseDirection[2];
		
		ImageCentern[0] = x;
		ImageCentern[1] = y;
		ImageCentern[2] = z;
		
		double xyz[3];
		
		xyz[0] = ImageCentern[0]-ImageCenter1[0];
		xyz[1] = ImageCentern[1]-ImageCenter1[1];
		xyz[2] = ImageCentern[2]-ImageCenter1[2];
		sdc[0] = xyz[0]/sqrt(xyz[0]*xyz[0]+xyz[1]*xyz[1]+xyz[2]*xyz[2]);
		sdc[1] = xyz[1]/sqrt(xyz[0]*xyz[0]+xyz[1]*xyz[1]+xyz[2]*xyz[2]);
		sdc[2] = xyz[2]/sqrt(xyz[0]*xyz[0]+xyz[1]*xyz[1]+xyz[2]*xyz[2]);
		xyz[0] = ImageCenter2[0]-ImageCenter1[0];
		xyz[1] = ImageCenter2[1]-ImageCenter1[1];
		xyz[2] = ImageCenter2[2]-ImageCenter1[2];
		dslice = sqrt(xyz[0]*xyz[0]+xyz[1]*xyz[1]+xyz[2]*xyz[2]);
	}
	
	if (nNumOfImages==1) //temp by Dailiang 2007.1.10
	{
		sdc[0] = pMRIImage.scanField.SliceDirX;
		sdc[1] = pMRIImage.scanField.SliceDirY;
		sdc[2] = pMRIImage.scanField.SliceDirZ;
		dslice = 1;
	}
	Matrix Mdc(3,3);
	Mdc.SetCol(0,dFreqDirection[0],dFreqDirection[1],dFreqDirection[2]);
	Mdc.SetCol(1,dPhaseDirection[0],dPhaseDirection[1],dPhaseDirection[2]);
	Mdc.SetCol(2,sdc[0],sdc[1],sdc[2]);
	
	PixelSpacing[0] = dFOV[0]/nResolutionX;
	PixelSpacing[1] = dFOV[1]/nResolutionY;
	
	Matrix D(3,3);
	for (int i=0;i<3;i++)
	{
		for (j=0;j<3;j++)
		{
			D.value[i][j] = 0;
		}
	}
	D.value[0][0] = PixelSpacing[0];
	D.value[1][1] = PixelSpacing[1];
	D.value[2][2] = dslice;
	
	double P0[3];
	P0[0] = ImageCenter1[0];
	P0[1] = ImageCenter1[1];
	P0[2] = ImageCenter1[2];
	
	Mdc.value[0][0] = -Mdc.value[0][0];
	Mdc.value[0][1] = -Mdc.value[0][1];
	Mdc.value[0][2] = -Mdc.value[0][2];
	
	
	//	Mdc.value[1][0] = -Mdc.value[1][0];
	//	Mdc.value[1][1] = -Mdc.value[1][1];
	//	Mdc.value[1][2] = -Mdc.value[1][2];
	
	P0[0] = -P0[0];
	//	P0[1] = -P0[1];
	
	Matrix Mtmp(3,3);
	Mtmp = Mdc*D;
	for (i=0;i<3;i++)
	{
		for (j=0;j<3;j++)
		{
			M.value[i][j] = Mtmp.value[i][j];
		}
	}
	
	M.value[0][3] = P0[0];
	M.value[1][3] = P0[1];
	M.value[2][3] = P0[2];
	M.value[3][0] = 0;
	M.value[3][1] = 0;
	M.value[3][2] = 0;
	M.value[3][3] = 1;
	
	for (i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			MOut[i][j] = M.value[i][j];
		}
	}
	delete []ImageCentern;
	delete []ImageCenter1;
	delete []ImageCenter2;
	delete []dCenter;
	delete []dFreqDirection;
	delete []dPhaseDirection;
	return 1;
	
}

int CGradientCorrection::vox2ras_0to1(float** ppM0, float** ppM1)
{
	int i,j;
	Matrix Q(4,4);
	for (i=0;i<4;i++)
	{
		for (j=0;j<4;j++)
		{
			Q.value[i][j] = 0;
		}
	}
	Q.value[0][3] = 1;
	Q.value[1][3] = 1;
	Q.value[2][3] = 1;
	Matrix M0(4,4);
	for (i=0;i<4;i++)
	{
		for (j=0;j<4;j++)
		{
			M0.value[i][j] = ppM0[i][j];
		}
	}
	Q = M0.Inverse()+Q;
	Matrix qtmp = Q.Inverse();
	for (i=0;i<4;i++)
	{
		for (j=0;j<4;j++)
		{
			ppM1[i][j] = qtmp.value[i][j];
		}
	}

	
	return 1;
}

BOOL CGradientCorrection:: LoadMRIUnwarpData(CString sFileName)
{
	try
	{
		std::ifstream inFile( sFileName, std::ios::in | std::ios::binary );
		if ( !inFile )
		{
			return FALSE;
		}
		inFile.seekg( 0, std::ios::seekdir::beg);
		
		inFile.read((char*)(&m_ncoords_x),4);
		inFile.read((char*)(&m_ncoords_y),4);
		inFile.read((char*)(&m_ncoords_z),4);
		inFile.read((char*)(m_fcoords_x),8);
		inFile.read((char*)(m_fcoords_y),8);
		inFile.read((char*)(m_fcoords_z),8);
		m_Dx = new float[m_ncoords_x*m_ncoords_y*m_ncoords_z];
		m_Dy = new float[m_ncoords_x*m_ncoords_y*m_ncoords_z];
		m_Dz = new float[m_ncoords_x*m_ncoords_y*m_ncoords_z];
		m_JacDetx = new float[m_ncoords_x*m_ncoords_y*m_ncoords_z];
		m_JacDety = new float[m_ncoords_x*m_ncoords_y*m_ncoords_z];
		m_JacDetz = new float[m_ncoords_x*m_ncoords_y*m_ncoords_z];
		inFile.read((char*)(m_Dx),m_ncoords_x*m_ncoords_y*m_ncoords_z*4);
		inFile.read((char*)(m_Dy),m_ncoords_x*m_ncoords_y*m_ncoords_z*4);
		inFile.read((char*)(m_Dz),m_ncoords_x*m_ncoords_y*m_ncoords_z*4);
		inFile.read((char*)(m_JacDetx),m_ncoords_x*m_ncoords_y*m_ncoords_z*4);
		inFile.read((char*)(m_JacDety),m_ncoords_x*m_ncoords_y*m_ncoords_z*4);
		inFile.read((char*)(m_JacDetz),m_ncoords_x*m_ncoords_y*m_ncoords_z*4);
		inFile.close();
		
		m_fcoords_x[0] = m_fcoords_x[0]*1000;
		m_fcoords_x[1] = m_fcoords_x[1]*1000;
		m_fcoords_y[0] = m_fcoords_y[0]*1000;
		m_fcoords_y[1] = m_fcoords_y[1]*1000;
		m_fcoords_z[0] = m_fcoords_z[0]*1000;
		m_fcoords_z[1] = m_fcoords_z[1]*1000;
		
		for (int i=0;i<m_ncoords_x*m_ncoords_y*m_ncoords_z;i++)
		{
			m_Dx[i] = m_Dx[i]*1000;
			m_Dy[i] = m_Dy[i]*1000;
			m_Dz[i] = m_Dz[i]*1000;
			//TRACE("%.2f,%.2f,%.2f,%.2f,%.2f,%.2f\n",Dx[i],Dy[i],Dz[i],JacDetx[i],JacDety[i],JacDetz[i]);
		}
	}catch(...)
	{
		return FALSE;
	}

	return TRUE;
}



int CGradientCorrection::rcs2index(int SizeRCS,int* R,int dr,int* C,int dc,
								   int* S,int ds, int nr,int nc,int ns,int* indexvec)
{
	for(int i=0;i<SizeRCS;i++)
	{
		indexvec[i] = R[i]+dr+(C[i]-1+dc)*nr+(S[i]+ds-1)*nr*nc;
	}
	return 1;
}

int  CGradientCorrection::interp_trilin(int SizeRCS,float* R,float* C,float* S,float* M1,float* M2,float* M3,float* M4,float* M5,float* M6,
										int nr,int nc,int ns, float* V_out1,float* V_out2,float* V_out3,float* V_out4,float* V_out5,float* V_out6)
{
	
	int dr,dc,ds;
	int Rint;
	int Cint;
	int Sint;
	float Wr[2];
	float Wc[2];
	float Ws[2];
	

	int index=0;
	
	
	for (int i=0;i<SizeRCS;i++)
	{
		R[i] = max(1,min(nr-1,R[i]));
		C[i] = max(1,min(nc-1,C[i]));
		S[i] = max(1,min(ns-1,S[i]));
		
		V_out1[i] = 0;
		V_out2[i] = 0;
		V_out3[i] = 0;
		V_out4[i] = 0;
		V_out5[i] = 0;
		V_out6[i] = 0;
		Rint = floor(R[i]);
		Cint = floor(C[i]);
		Sint = floor(S[i]);
		Wr[1] = R[i] - Rint;
		Wr[0] = 1-Wr[1];
		Wc[1] = C[i] - Cint;
		Wc[0] = 1-Wc[1];
		Ws[1] = S[i] - Sint;
		Ws[0] = 1-Ws[1];

		
		for (dr=0;dr<2;dr++)
		{ 
			for(dc=0; dc<2;dc++)
			{
				for(ds=0; ds<2;ds++)
				{			
					index = Rint+dr+(Cint-1+dc)*nr+(Sint+ds-1)*nr*nc;
					V_out1[i] = V_out1[i]+M1[index-1]*Wr[dr]*Wc[dc]*Ws[ds];
					V_out2[i] = V_out2[i]+M2[index-1]*Wr[dr]*Wc[dc]*Ws[ds];
					V_out3[i] = V_out3[i]+M3[index-1]*Wr[dr]*Wc[dc]*Ws[ds];
					V_out4[i] = V_out4[i]+M4[index-1]*Wr[dr]*Wc[dc]*Ws[ds];
					V_out5[i] = V_out5[i]+M5[index-1]*Wr[dr]*Wc[dc]*Ws[ds];
					V_out6[i] = V_out6[i]+M6[index-1]*Wr[dr]*Wc[dc]*Ws[ds];
					
				}
				
			}
			
		}
		
	}
	
	

	/*for (int i=0;i<SizeRCS;i++)
	{
		R[i] = max(1,min(nr-1,R[i]));
		C[i] = max(1,min(nc-1,C[i]));
		S[i] = max(1,min(ns-1,S[i]));
		
		V_out1[i] = 0;
		V_out2[i] = 0;
		V_out3[i] = 0;
		V_out4[i] = 0;
		V_out5[i] = 0;
		V_out6[i] = 0;
		Rint[i] = floor(R[i]);
		Cint[i] = floor(C[i]);
		Sint[i] = floor(S[i]);
		Wr[1][i] = R[i] - Rint[i];
		Wr[0][i] = 1-Wr[1][i];
		Wc[1][i] = C[i] - Cint[i];
		Wc[0][i] = 1-Wc[1][i];
		Ws[1][i] = S[i] - Sint[i];
		Ws[0][i] = 1-Ws[1][i];
		
	}
	
	
	for (dr=0;dr<2;dr++)
	{ 
		for(dc=0; dc<2;dc++)
		{
			for(ds=0; ds<2;ds++)
			{			
				//rcs2index(SizeRCS, Rint,dr,Cint,dc,Sint,ds,nr,nc,ns,indexvec[index]);
				for(i=0;i<SizeRCS;i++)
				{
					indexvec[index][i] = Rint[i]+dr+(Cint[i]-1+dc)*nr+(Sint[i]+ds-1)*nr*nc;
				}

				index = index+1;
			}
			
		}
		
	}
	index = 0;
	
	for (i=0;i<SizeRCS;i++)
	{
		for (dr=0;dr<2;dr++)
		{ 
			for(dc=0; dc<2;dc++)
			{
				for(ds=0; ds<2;ds++)
				{			
					V_out1[i] = V_out1[i]+M1[indexvec[index][i]-1]*Wr[dr][i]*Wc[dc][i]*Ws[ds][i];
					V_out2[i] = V_out2[i]+M2[indexvec[index][i]-1]*Wr[dr][i]*Wc[dc][i]*Ws[ds][i];
					V_out3[i] = V_out3[i]+M3[indexvec[index][i]-1]*Wr[dr][i]*Wc[dc][i]*Ws[ds][i];
					V_out4[i] = V_out4[i]+M4[indexvec[index][i]-1]*Wr[dr][i]*Wc[dc][i]*Ws[ds][i];
					V_out5[i] = V_out5[i]+M5[indexvec[index][i]-1]*Wr[dr][i]*Wc[dc][i]*Ws[ds][i];
					V_out6[i] = V_out6[i]+M6[indexvec[index][i]-1]*Wr[dr][i]*Wc[dc][i]*Ws[ds][i];
					index = index+1;
				}
				
			}
			
		}
		index = 0;
	}*/
	
	
	
	return 1;
	
}

int CGradientCorrection::proj(int unwarpdims, float** ppMdc,float *Dx1,float *Dy1,float *Dz1,int size,float* dx, float* dy, float* dz)
{
	Matrix Mdci(3,3);
	Matrix Mdc(3,3);
	
	Matrix Mtmp(3,3);
	Matrix Mdiag1(3,3);
	Matrix Mdiag2(3,3);
	
	int i,j;
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			Mdc.value[i][j] = ppMdc[i][j];
			Mdiag1.value[i][j] = 0;
			Mdiag2.value[i][j] = 0;
		}
	}
	Mdiag1.value[0][0] = 1;
	Mdiag1.value[1][1] = 1;
	Mdiag2.value[2][2] = 1;
	Mdci = Mdc.Inverse();
	
		
	if (unwarpdims==1)
	{
		Mtmp = Mdc*Mdiag2*Mdci;
		for (i=0;i<size;i++)
		{
			dx[i] = Mtmp.value[0][0]*Dx1[i]+Mtmp.value[0][1]*Dy1[i]+Mtmp.value[0][2]*Dz1[i];
			dy[i] = Mtmp.value[1][0]*Dx1[i]+Mtmp.value[1][1]*Dy1[i]+Mtmp.value[1][2]*Dz1[i];
			dz[i] = Mtmp.value[2][0]*Dx1[i]+Mtmp.value[2][1]*Dy1[i]+Mtmp.value[2][2]*Dz1[i];
		}
	}
	else if (unwarpdims==2)
	{
		Mtmp = Mdc*Mdiag1*Mdci;
		for (i=0;i<size;i++)
		{
			dx[i] = Mtmp.value[0][0]*Dx1[i]+Mtmp.value[0][1]*Dy1[i]+Mtmp.value[0][2]*Dz1[i];
			dy[i] = Mtmp.value[1][0]*Dx1[i]+Mtmp.value[1][1]*Dy1[i]+Mtmp.value[1][2]*Dz1[i];
			dz[i] = Mtmp.value[2][0]*Dx1[i]+Mtmp.value[2][1]*Dy1[i]+Mtmp.value[2][2]*Dz1[i];
		}
	}
	else
	{
		memcpy(dx,Dx1,sizeof(float)*size);
		memcpy(dy,Dy1,sizeof(float)*size);
		memcpy(dz,Dz1,sizeof(float)*size);
		
	}
	
	return 1;
}

int CGradientCorrection::jdproj(float* JDinx,float* JDiny,float* JDinz,int size,float** ppMdc,int unwarpdims,int maxjd,float* JDout)
{
	Matrix Mdci(3,3);
	Matrix Mdc(3,3);
	Matrix Mtmp1(3,3);
	Matrix Mtmp2(3,3);
	Matrix Mdiag1(3,3);
	Matrix Mdiag2(3,3);
	
	int i,j;
	for(i=0;i<3;i++)
	{
		for(j=0;j<3;j++)
		{
			Mdc.value[i][j] = ppMdc[i][j];
			Mdiag1.value[i][j] = 0;
			Mdiag2.value[i][j] = 0;
		}
	}
	
	Mdiag1.value[0][0] = 1;
	Mdiag1.value[1][1] = 1;
	Mdiag2.value[2][2] = 1;
	Mdci = Mdc.Inverse();
	
	float JDtmp[3];
	
	if (unwarpdims==1)
	{
		Mtmp1 = Mdc*Mdiag2*Mdci;
		Mtmp2 = Mdc*Mdiag1*Mdci;
		for (i=0;i<size;i++)
		{
			JDtmp[0] = Mtmp1.value[0][0]*JDinx[i]+Mtmp1.value[0][1]*JDiny[i]+Mtmp1.value[0][2]*JDinz[i]+Mtmp2.value[0][0]+Mtmp2.value[0][1]+Mtmp2.value[0][2];
			JDtmp[1] = Mtmp1.value[1][0]*JDinx[i]+Mtmp1.value[1][1]*JDiny[i]+Mtmp1.value[1][2]*JDinz[i]+Mtmp2.value[1][0]+Mtmp2.value[1][1]+Mtmp2.value[1][2];
			JDtmp[2] = Mtmp1.value[2][0]*JDinx[i]+Mtmp1.value[2][1]*JDiny[i]+Mtmp1.value[2][2]*JDinz[i]+Mtmp2.value[2][0]+Mtmp2.value[2][1]+Mtmp2.value[2][2];
			JDout[i] = JDtmp[0]*JDtmp[1]*JDtmp[2];
			JDout[i] = Max(JDout[i],0);
			JDout[i] = Min(JDout[i],maxjd);
		}
	}
	else if (unwarpdims==2)
	{
		Mtmp2 = Mdc*Mdiag2*Mdci;
		Mtmp1 = Mdc*Mdiag1*Mdci;
		for (i=0;i<size;i++)
		{
			JDtmp[0] = Mtmp1.value[0][0]*JDinx[i]+Mtmp1.value[0][1]*JDiny[i]+Mtmp1.value[0][2]*JDinz[i]+Mtmp2.value[0][0]+Mtmp2.value[0][1]+Mtmp2.value[0][2];
			JDtmp[1] = Mtmp1.value[1][0]*JDinx[i]+Mtmp1.value[1][1]*JDiny[i]+Mtmp1.value[1][2]*JDinz[i]+Mtmp2.value[1][0]+Mtmp2.value[1][1]+Mtmp2.value[1][2];
			JDtmp[2] = Mtmp1.value[2][0]*JDinx[i]+Mtmp1.value[2][1]*JDiny[i]+Mtmp1.value[2][2]*JDinz[i]+Mtmp2.value[2][0]+Mtmp2.value[2][1]+Mtmp2.value[2][2];
			JDout[i] = JDtmp[0]*JDtmp[1]*JDtmp[2];
			JDout[i] = Max(JDout[i],0);
			JDout[i] = Min(JDout[i],maxjd);
		}
	}
	else
	{
		for (i=0;i<size;i++)
		{
			JDout[i] = JDinx[i]*JDiny[i]*JDinz[i];
			JDout[i] = Max(JDout[i],0);
			JDout[i] = Min(JDout[i],maxjd);
		}
	}
	
	return 1;
}

int CGradientCorrection::unwarp_resample(short** invol,float** ppM1,int volsize[3],
										 float** ppMout,float** ppMdc,int Jacobianflag,int inflag,int thruflag,float** invol_out)
										 
{
	CString interp_m = "linear";
	int i,j;
	
	Matrix M(4,4);
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			M.value[i][j] = ppM1[i][j];
		}
	}
	Matrix Mi = M.Inverse();

	int SGflag = 0;
	int unwarpdims = 0;
	
	if (inflag)
    {
		unwarpdims = 2;
        SGflag = 0;
    }
	
    else if (thruflag)
	{
		unwarpdims=1;
        SGflag=0;
	}
	
    else
	{
        unwarpdims=3;
        SGflag=1; // Dai Liang
    }
	SGflag = 0;
    unwarpdims = 2; //Add by Dai Liang
	//	SGflag = 1;
	//  unwarpdims = 3; //Add by Dai Liang
	
	int nrows_out = volsize[0];
	int ncols_out = volsize[1];
	int nslices_out = volsize[2];
	int NumVoxel = nrows_out*ncols_out;
	Matrix Mgw(4,4);
	Mgw.value[0][0] = 0;
	Mgw.value[0][1] = (m_fcoords_x[1]-m_fcoords_x[0])/(m_ncoords_x-1);
	Mgw.value[0][2] = 0;
	Mgw.value[0][3] = m_fcoords_x[0]-(m_fcoords_x[1]-m_fcoords_x[0])/(m_ncoords_x-1);
	
	Mgw.value[1][0] = (m_fcoords_y[1]-m_fcoords_y[0])/(m_ncoords_y-1);
	Mgw.value[1][1] = 0;
	Mgw.value[1][2] = 0;
	Mgw.value[1][3] = m_fcoords_y[0]-(m_fcoords_y[1]-m_fcoords_y[0])/(m_ncoords_y-1);
	
	Mgw.value[2][0] = 0;
	Mgw.value[2][1] = 0;
	Mgw.value[2][2] = (m_fcoords_z[1]-m_fcoords_z[0])/(m_ncoords_z-1);
	Mgw.value[2][3] = m_fcoords_z[0]-(m_fcoords_z[1]-m_fcoords_z[0])/(m_ncoords_z-1);
	
	Mgw.value[3][0] = 0;
	Mgw.value[3][1] = 0;
	Mgw.value[3][2] = 0;
	Mgw.value[3][3] = 1;
	Matrix Mgwi = Mgw.Inverse();
	
	
	float  S;
	float tmpR,tmpC,tmpS;
	float tmpX,tmpY,tmpZ;
	
	for (j=0;j<nslices_out;j++)
	{
		
		memset(m_nflag,0,sizeof(float)*NumVoxel);

		S = j+1; 
			
		for (i=0;i<NumVoxel;i++)
		{
			m_X[i] = ppMout[0][0]*m_R[i]+ppMout[0][1]*m_C[i]+ppMout[0][2]*S+ppMout[0][3];
			m_Y[i] = ppMout[1][0]*m_R[i]+ppMout[1][1]*m_C[i]+ppMout[1][2]*S+ppMout[1][3];
			m_Z[i] = ppMout[2][0]*m_R[i]+ppMout[2][1]*m_C[i]+ppMout[2][2]*S+ppMout[2][3];
			m_Rgw[i] = Mgwi.value[0][0]*m_X[i]+Mgwi.value[0][1]*m_Y[i]+Mgwi.value[0][2]*m_Z[i]+Mgwi.value[0][3];
			m_Cgw[i] = Mgwi.value[1][0]*m_X[i]+Mgwi.value[1][1]*m_Y[i]+Mgwi.value[1][2]*m_Z[i]+Mgwi.value[1][3];
			m_Sgw[i] = Mgwi.value[2][0]*m_X[i]+Mgwi.value[2][1]*m_Y[i]+Mgwi.value[2][2]*m_Z[i]+Mgwi.value[2][3];
		}		
		
		interp_trilin(NumVoxel,m_Rgw,m_Cgw,m_Sgw,m_Dx,m_Dy,m_Dz,m_JacDetx,m_JacDety,m_JacDetz,
			m_ncoords_y,m_ncoords_x,m_ncoords_z,m_Dx1,m_Dy1,m_Dz1,m_JacDetx1,m_JacDety1,m_JacDetz1);
		
		proj(unwarpdims,ppMdc,m_Dx1,m_Dy1,m_Dz1,NumVoxel,m_Dx2,m_Dy2,m_Dz2);			
		if (Jacobianflag)
		{		
			if (!SGflag)
			{
				
				jdproj(m_JacDetx1,m_JacDety1,m_JacDetz1,NumVoxel,ppMdc,unwarpdims,3,m_JacDet1);
				
			}
		}
		for (i=0;i<NumVoxel;i++)
		{
			tmpX = m_X[i]+m_Dx2[i];
			tmpY = m_Y[i]+m_Dy2[i];
			tmpZ = m_Z[i]+m_Dz2[i];
			tmpR = Mi.value[0][0]*tmpX+Mi.value[0][1]*tmpY+Mi.value[0][2]*tmpZ+Mi.value[0][3];
			tmpC = Mi.value[1][0]*tmpX+Mi.value[1][1]*tmpY+Mi.value[1][2]*tmpZ+Mi.value[1][3];
			tmpS = Mi.value[2][0]*tmpX+Mi.value[2][1]*tmpY+Mi.value[2][2]*tmpZ+Mi.value[2][3];

			//if (R1[i]<1 || C1[i]<1 || S1[i]<1 || R1[i]>nrows_out || C1[i]>ncols_out || S1[i]>nslices_out+0.5)
			if (tmpR<1 || tmpC<1 || tmpR>nrows_out || tmpC>ncols_out )
			{
				m_nflag[i] = 1;
				
			}
						
			m_R1[i] = Min(Max(tmpR,1),nrows_out)-1;
			m_C1[i] = Min(Max(tmpC,1),ncols_out)-1;
			m_S1[i] = Min(Max(tmpS,1),nslices_out)-1;
		}
		
				
		interp3(m_im1vec, invol,m_C1,m_R1,m_S1,interp_m, ncols_out,nrows_out,nslices_out);
		for (i=0;i<NumVoxel;i++)
		{
			m_im1vec[i] = m_im1vec[i]*(1-m_nflag[i]);
			m_im1vec2[(int)m_imindx[i]] = m_im1vec[i];
		}
				
		
		if (Jacobianflag)
		{
			
			for (i=0;i<NumVoxel;i++)
			{
				m_imjdvec[(int)m_imindx[i]] = m_JacDet1[i];	
			}
			for(i=0;i<NumVoxel;i++)
			{
				m_im1vec2[i] = Min(Max(m_im1vec2[i]*m_imjdvec[i],0),4095);
			}
			
			
		}
		
		memcpy(invol_out[j],m_im1vec2,sizeof(float)*NumVoxel);
		
	}
		
		
	//	fclose(fp);
	return 1;
}


int CGradientCorrection::GradientCorrection(CMRISeries* pImage)
{
	if ( m_bLoadedUnwarpRefData==FALSE )
	{
		return 0;
	}

	int i;	
	int nNumOfImages=pImage->GetNumOfImages();
	int nResolutionX = pImage->GetImageAt(0)->GetImageHeader()->seriesField.ResolutionX;
	int nResolutionY = pImage->GetImageAt(0)->GetImageHeader()->seriesField.ResolutionY;
	int nNumOfPts = nResolutionX*nResolutionY;
	m_size_vol[0] = nResolutionY;
	m_size_vol[1] = nResolutionX;
	m_size_vol[2] = nNumOfImages;
	
	MR_ImageHeader* pImageHeader;
	short** ppData;
	pImageHeader=new MR_ImageHeader[pImage->GetNumOfImages()];
	ppData=new short*[pImage->GetNumOfImages()];
	for(i=0;i<pImage->GetNumOfImages();i++)
	{
		pImageHeader[i]=*pImage->GetImageAt(i)->GetImageHeader();
		//ppData[i]=new short[nNumOfPts];
		//memcpy((char*)(ppData[i]),(char*)(pImage->GetImageAt(i)->GetColorOfRawData()),sizeof(short)*nNumOfPts);
		ppData[i]=(short *)pImage->GetImageAt(i)->GetColorOfRawData();
	}

	
	DWORD t1=GetTickCount();//程序段开始前取得系统运行时间(ms)
	DWORD t2;

	InitStorage();
	
	
	//Read the ini file for flag value
	UINT inflag = 1, thruflag = 0, jacflag = 1;
		
	
	
	float** ppM = new float* [4];
	for (i=0;i<4;i++)
	{
		ppM[i] = new float[4];
	}
	LoadMRISeries2Vol(nNumOfImages, pImageHeader, ppM);
	
	float** ppMdc;
	ppMdc =new float*[3];
	for(i=0;i<3;i++)
	{
		ppMdc[i]=new float[3];
	}
	vox2ras_xform(ppM,ppMdc);

	float** ppM1 = new float*[4];
	for (i=0;i<4;i++)
	{	
		ppM1[i] = new float[4];
	}
	vox2ras_0to1(ppM,ppM1);
		
	float** imvol_out =new float* [nNumOfImages];
	for (i=0;i<nNumOfImages;i++)
	{
		imvol_out[i] = new float[nResolutionX*nResolutionY];
	} 
	
	unwarp_resample(ppData,ppM1,m_size_vol,ppM1,
		ppMdc,jacflag,inflag,thruflag,imvol_out);
	
	/*FILE *file;
	file = fopen ("d:\\21.txt", "w+b");
	fseek(file,0,SEEK_SET);
	for(i=0;i<nNumOfImages;i++)
	{
		fwrite((char*)ppData[i],sizeof(char),sizeof(short)*nNumOfPts,file);
	}
	fclose(file);*/

	for (i=0;i<m_size_vol[2];i++)
	{
		for ( int j=0; j<nResolutionX*nResolutionY; j++ )
		{
			ppData[i][j] = imvol_out[i][j];
		}
	}

	
	/*file = fopen ("d:\\22.txt", "w+b");
	fseek(file,0,SEEK_SET);
	for(i=0;i<nNumOfImages;i++)
	{
		fwrite((char*)ppData[i],sizeof(char),sizeof(short)*nNumOfPts,file);
	}
	fclose(file);*/
	
	delete []pImageHeader;
	
	for (i=0;i<nNumOfImages;i++)
	{
		delete []imvol_out[i];
		
	}
	delete ppData;
	delete []imvol_out;

	for(i=0;i<3;i++)
	{
		delete []ppMdc[i];
		
	}
	delete []ppMdc;
	
	
	for (i=0;i<4;i++)
	{
		delete []ppM1[i];
		delete []ppM[i];
		
	}
	delete []ppM1;
	delete []ppM;
	
	t2=GetTickCount();//程序段开始前取得系统运行时间(ms)
	//	str.Format("Before stop:%dms",t2-t1);//前后之差即 程序运行时间
	return t2-t1;
}

int CGradientCorrection::vox2ras_xform(float** M,float** ppMdc)
{
	Matrix MdcD(3,3);
	int i,j;
	for(i=0;i<3;i++)
	{
		for (j=0;j<3;j++)
		{
			MdcD.value[i][j] = M[i][j];
		}
	}
	float delta[3];
	float sum=0;
	int p,q;
	for ( p=0;p<3;p++)
	{
		for (q=0;q<3;q++)
		{
			sum = sum+MdcD.value[q][p]*MdcD.value[q][p];
			
		}
		delta[p] = sqrt(sum);
		sum = 0;
	}
	for(p=0;p<3;p++)
	{
		for(q=0;q<3;q++)
		{
			ppMdc[p][q] = MdcD.value[p][q]/delta[q];
		}
	}
			
	
	return 1;
	
}



int CGradientCorrection::interp3(float* im1vec, short **in_mvol,float *C1,float *R1,float *S1,CString interp_m, int rsize, int csize, int ssize)
{
    int i;//Used to control lengths of each array
	int x0,x1,y0,y1,z0,z1;  //Used to get the extension of a cube
	int csize_1=csize-1;
	int rsize_1=rsize-1;
	int ssize_1=ssize-1;
	BOOL bZChange=FALSE;
	if(interp_m=="linear") 
	{
		for(i=0;i<csize*rsize;i++)
		{
			if(C1[i]==csize_1)
			{
				x0=C1[i]-1;
				x1=C1[i];
			}
			else
			{
				x0=floor(C1[i]);
				x1=x0+1;
			}
			
			if(R1[i]==rsize_1)
			{
				y0=R1[i]-1;
				y1=R1[i];
			}
			else
			{
				y0=floor(R1[i]);
				y1=y0+1;
			}
			bZChange=FALSE;
			if(S1[i]==ssize_1)
			{
				z0=S1[i]-1;
				z1=S1[i];
				if(z0<0)
				{
					bZChange=TRUE;
					z0=0;
					z1=1;
				}
			}
			else
			{
				z0=floor(S1[i]);
				z1=z0+1;
			}
			
			float A=(y1 - R1[i])*(z1 - S1[i]);
			float B=(y1 - R1[i])*(S1[i]-z0);
			float C=(R1[i]-y0)*(S1[i]-z0);
			float D=(R1[i]-y0)*(z1 - S1[i]);
			float E=(x1 - C1[i]);
			float F=(C1[i]-x0);
			
			if(bZChange)
			{
				z0=0;
				z1=0;
			}
			

			im1vec[i]= E * (in_mvol[z0][x0*rsize+y0] * A + in_mvol[z0][x0*rsize+y1] * D + 
				            in_mvol[z1][x0*rsize+y0] * B + in_mvol[z1][x0*rsize+y1] * C ) +
					   F * (in_mvol[z0][x1*rsize+y0] * A + in_mvol[z1][x1*rsize+y0] * B +
					        in_mvol[z0][x1*rsize+y1] * D + in_mvol[z1][x1*rsize+y1] * C );
			/*im1vec[i]= E * (in_mvol[x0][y0][z0] * A + in_mvol[x0][y1][z0] * D + in_mvol[x0][y0][z1] * B + in_mvol[x0][y1][z1] * C) +
					   F * (in_mvol[x1][y0][z0] * A + in_mvol[x1][y0][z1] * B + in_mvol[x1][y1][z0] * D + in_mvol[x1][y1][z1] * C );*/
			
			/*im1vec[i]= (in_mvol[x0][y0][z0])* E * A +
					   (in_mvol[x0][y1][z0])* E * D +
					   (in_mvol[x0][y0][z1])* E * B +
					   (in_mvol[x0][y1][z1])* E * C +
					   (in_mvol[x1][y0][z0])* F * A +
					   (in_mvol[x1][y0][z1])* F * B +
					   (in_mvol[x1][y1][z0])* F * D +
					   (in_mvol[x1][y1][z1])* F * C ;*/

			/*im1vec[i]= (in_mvol[x0][y0][z0])* (x1 - C1[i]) * (y1 - R1[i])*(z1 - S1[i])+
					   (in_mvol[x1][y0][z0])* (C1[i]-x0)   * (y1 - R1[i])*(z1 - S1[i])+
					   (in_mvol[x0][y1][z0])* (x1 - C1[i]) * (R1[i]-y0)*(z1 - S1[i])+
					   (in_mvol[x0][y0][z1])* (x1 - C1[i]) * (y1 - R1[i])*(S1[i]-z0)+
					   (in_mvol[x1][y0][z1])* (C1[i]-x0)   * (y1 - R1[i])*(S1[i]-z0)+
					   (in_mvol[x0][y1][z1])* (x1 - C1[i]) * (R1[i]-y0)*(S1[i]-z0)+
					   (in_mvol[x1][y1][z0])* (C1[i]-x0)   * (R1[i]-y0)*(z1 - S1[i])+
					   (in_mvol[x1][y1][z1])* (C1[i]-x0)   * (R1[i]-y0)*(S1[i]-z0);*/
		
		}
	}

	return 1;
}

/////////////////////////////////////////////////////////////////



void CGradientCorrection::InitStorage()
{
	if(m_NumVoxel==m_size_vol[0]*m_size_vol[1])
	{
		return;
	}
	RemoveStorage();
	m_NumVoxel=m_size_vol[0]*m_size_vol[1];
	int nrows_out = m_size_vol[0];
	int ncols_out = m_size_vol[1];
	int nslices_out = m_size_vol[2];
	int NumVoxel=m_NumVoxel;

	m_R1 = new float[NumVoxel];
	m_C1 = new float[NumVoxel];
	m_S1 = new float[NumVoxel];
	m_im1vec = new float[NumVoxel];
	m_im1vec2 = new float[NumVoxel];
	m_R = new float[NumVoxel];
	m_C = new float[NumVoxel];
	m_imindx = new float[NumVoxel];
	m_X = new float[NumVoxel];
	m_Y = new float[NumVoxel];
	m_Z = new float[NumVoxel];
	m_Rgw = new float[NumVoxel];
	m_Cgw = new float[NumVoxel];
	m_Sgw = new float[NumVoxel];
	m_Dx1 = new float[NumVoxel];
	m_Dy1 = new float[NumVoxel];
	m_Dz1 = new float[NumVoxel];
	m_Dx2 = new float[NumVoxel];
	m_Dy2 = new float[NumVoxel];
	m_Dz2 = new float[NumVoxel];
	m_JacDetx1 = new float[NumVoxel];
	m_JacDety1 = new float[NumVoxel];
	m_JacDetz1 = new float[NumVoxel];
	m_JacDet1 = new float[NumVoxel];
	m_imjdvec = new float[NumVoxel];
	m_nflag = new float[NumVoxel];

	for (int i=0;i<nrows_out;i++)
	{
		for (int j=0;j<ncols_out;j++)
		{
			m_R[i*ncols_out+j] = i+1;
			m_C[i*ncols_out+j] = j+1;
			m_imindx[i*ncols_out+j] = m_R[i*ncols_out+j]-1 + (m_C[i*ncols_out+j]-1)*nrows_out;
		}
		
	}
}

void CGradientCorrection::RemoveStorage()
{
	if(m_R1!=NULL){	delete []m_R1;	m_R1=NULL; }
	if(m_C1!=NULL){	delete []m_C1;	m_C1=NULL; }
	if(m_S1!=NULL){	delete []m_S1;	m_S1=NULL; }
	if(m_im1vec!=NULL){	delete []m_im1vec;	m_im1vec=NULL; }
	if(m_im1vec2!=NULL){	delete []m_im1vec2;	m_im1vec2=NULL; }
	if(m_R!=NULL){	delete []m_R;	m_R=NULL; }
	if(m_C!=NULL){	delete []m_C;	m_C=NULL; }
	if(m_imindx!=NULL){	delete []m_imindx;	m_imindx=NULL; }
	if(m_X!=NULL){	delete []m_X;	m_X=NULL; }
	if(m_Y!=NULL){	delete []m_Y;	m_Y=NULL; }
	if(m_Z!=NULL){	delete []m_Z;	m_Z=NULL; }
	if(m_Rgw!=NULL){	delete []m_Rgw;	m_Rgw=NULL; }
	if(m_Cgw!=NULL){	delete []m_Cgw;	m_Cgw=NULL; }
	if(m_Sgw!=NULL){	delete []m_Sgw;	m_Sgw=NULL; }
	if(m_Dx1!=NULL){	delete []m_Dx1;	m_Dx1=NULL; }
	if(m_Dy1!=NULL){	delete []m_Dy1;	m_Dy1=NULL; }
	if(m_Dz1!=NULL){	delete []m_Dz1;	m_Dz1=NULL; }
	if(m_Dx2!=NULL){	delete []m_Dx2;	m_Dx2=NULL; }
	if(m_Dy2!=NULL){	delete []m_Dy2;	m_Dy2=NULL; }
	if(m_Dz2!=NULL){	delete []m_Dz2;	m_Dz2=NULL; }
	if(m_JacDetx1!=NULL){	delete []m_JacDetx1;	m_JacDetx1=NULL; }
	if(m_JacDety1!=NULL){	delete []m_JacDety1;	m_JacDety1=NULL; }
	if(m_JacDetz1!=NULL){	delete []m_JacDetz1;	m_JacDetz1=NULL; }
	if(m_JacDet1!=NULL){	delete []m_JacDet1;	m_JacDet1=NULL; }
	if(m_imjdvec!=NULL){	delete []m_imjdvec;	m_imjdvec=NULL; }
	if(m_nflag!=NULL){	delete []m_nflag;	m_nflag=NULL; }
	
}

BOOL CGradientCorrection::Init(CString sRefDataFile)
{
	if ( m_bLoadedUnwarpRefData ==FALSE)
	{
		m_bLoadedUnwarpRefData=LoadMRIUnwarpData( sRefDataFile );
	} 
	return m_bLoadedUnwarpRefData;
}
