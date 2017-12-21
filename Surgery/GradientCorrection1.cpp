 // GradientCorrection.cpp: implementation of the CGradientCorrection1 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "GradientCorrection1.h"
#include <fstream>
#include "math.h"
#include "float.h"
#include "ErrorCodes.h"
#include "MV.h" 
//#include "..\BiGlobals\ConfigManager.h" 
//class CLang;


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

//AFX_EXT_CLASS CGradientCorrection1 CGradientCorrection1::s_GradientCorrection;

CGradientCorrection1::CGradientCorrection1()
{
	Dx = NULL;
	Dy = NULL;
	Dz = NULL;
	JacDetx = NULL;
	JacDety = NULL;
	JacDetz = NULL;

	m_bLoadedUnwarpRefData = false;

	/*HKEY hKey = NULL;	
	long lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\XinAoMDT\\Prospect", 0, KEY_READ, &hKey);
	if(lResult == ERROR_SUCCESS)
	{
		char szDir[MAX_PATH];
		DWORD dwSize = MAX_PATH;
		lResult = RegQueryValueEx( hKey, "InstallDir", NULL, NULL, (BYTE*) szDir, &dwSize);
		if(lResult == ERROR_SUCCESS)
		{
			CString sIniPath = szDir;
			sIniPath += "\\bin\\xbmri.ini";

			TCHAR CorrectionFileName[MAX_PATH+1]; 
			UINT count = GetPrivateProfileString( "GRADIENT_CORRECTION", "RefDataFile", " ", CorrectionFileName, sizeof(CorrectionFileName), sIniPath );
			m_sRefDataFileName = CString(CorrectionFileName);
		}
	}

	
	RegCloseKey( hKey );*/
	m_sRefDataFileName ="D:\\MED\\ProspectV3.5.4.03I01R01 Setup\\centauri.gwv";

}

CGradientCorrection1::~CGradientCorrection1()
{

	if ( NULL != Dx )
	{
		delete []Dx;
		Dx = NULL;
	}

	if ( NULL != Dy )
	{
		delete []Dy;
		Dy = NULL;
	}
	if ( NULL != Dz )
	{
		delete []Dz;
		Dz = NULL;
	}

	if ( NULL != JacDetx )
	{
		delete []JacDetx;
		JacDetx = NULL;
	}
	if ( NULL != JacDety )
	{
		delete []JacDety;
		JacDety = NULL;
	}
	if ( NULL != JacDetz )
	{
		delete []JacDetz;
		JacDetz = NULL;
	}
}

/*CGradientCorrection1& CGradientCorrection1::GetHandle()
{
	return s_GradientCorrection;
}*/


int CGradientCorrection1::LoadMRISeries2Vol(int nNumOfImages, MR_ImageHeader* pImageHeader, short** ppData,short** m_mVol,float** m_M)
{

	int j;
	int nResolutionX = pImageHeader[0].seriesField.ResolutionX;
	int nResolutionY = pImageHeader[0].seriesField.ResolutionY;
	int m_nNumOfPts = nResolutionX*nResolutionY;
	Matrix M(4,4);
	for (int i=0; i<nNumOfImages; i++)
	{	
		memcpy(m_mVol[i],ppData[i],sizeof(short)*m_nNumOfPts);
	}

	float* ImageCentern = new float [3]; 
	float* ImageCenter1 = new float [3];
	float* ImageCenter2 = new float [3];

	float* m_dCenter;
	m_dCenter = new float[3];
	float* m_dFreqDirection;
	m_dFreqDirection = new float[3];
	float m_dFOV[3];
	float* m_dPhaseDirection;
	m_dPhaseDirection = new float[3];
	double x,y,z;

	MR_ImageHeader m_pMRIImage = pImageHeader[0];

	m_dCenter[0] = m_pMRIImage.imageField.fCenterPointX;
	m_dCenter[1] = m_pMRIImage.imageField.fCenterPointY;
	m_dCenter[2] = m_pMRIImage.imageField.fCenterPointZ;

	m_dFreqDirection[0] = m_pMRIImage.imageField.fFreqDirX; 
	m_dFreqDirection[1] = m_pMRIImage.imageField.fFreqDirY;
	m_dFreqDirection[2] = m_pMRIImage.imageField.fFreqDirZ;


	m_dFOV[0] = m_pMRIImage.seriesField.FovX;
	m_dFOV[1] = m_pMRIImage.seriesField.FovY;
	m_dFOV[2] = m_pMRIImage.seriesField.FovZ;

	m_dPhaseDirection[0] = m_pMRIImage.imageField.fPhaseDirX;
	m_dPhaseDirection[1] = m_pMRIImage.imageField.fPhaseDirY;
	m_dPhaseDirection[2] = m_pMRIImage.imageField.fPhaseDirZ;

	x = m_dCenter[0]-m_dFOV[1]/2*m_dFreqDirection[0]-m_dFOV[0]/2*m_dPhaseDirection[0];
	y = m_dCenter[1]-m_dFOV[1]/2*m_dFreqDirection[1]-m_dFOV[0]/2*m_dPhaseDirection[1];
	z = m_dCenter[2]-m_dFOV[1]/2*m_dFreqDirection[2]-m_dFOV[0]/2*m_dPhaseDirection[2];
	double sdc[3];
	double PixelSpacing[2];
	double dslice;

	ImageCenter1[0] = x;
	ImageCenter1[1] = y;
	ImageCenter1[2] = z;

	if 	(nNumOfImages>1)
	{
		m_pMRIImage = pImageHeader[1];
		m_dCenter[0] = m_pMRIImage.imageField.fCenterPointX;
		m_dCenter[1] = m_pMRIImage.imageField.fCenterPointY;
		m_dCenter[2] = m_pMRIImage.imageField.fCenterPointZ;

		x = m_dCenter[0]-m_dFOV[1]/2*m_dFreqDirection[0]-m_dFOV[0]/2*m_dPhaseDirection[0];
		y = m_dCenter[1]-m_dFOV[1]/2*m_dFreqDirection[1]-m_dFOV[0]/2*m_dPhaseDirection[1];
		z = m_dCenter[2]-m_dFOV[1]/2*m_dFreqDirection[2]-m_dFOV[0]/2*m_dPhaseDirection[2];
		
		ImageCenter2[0] = x;
		ImageCenter2[1] = y;
		ImageCenter2[2] = z;

		m_pMRIImage = pImageHeader[nNumOfImages-1];

		m_dCenter[0] = m_pMRIImage.imageField.fCenterPointX;
		m_dCenter[1] = m_pMRIImage.imageField.fCenterPointY;
		m_dCenter[2] = m_pMRIImage.imageField.fCenterPointZ;

		x = m_dCenter[0]-m_dFOV[1]/2*m_dFreqDirection[0]-m_dFOV[0]/2*m_dPhaseDirection[0];
		y = m_dCenter[1]-m_dFOV[1]/2*m_dFreqDirection[1]-m_dFOV[0]/2*m_dPhaseDirection[1];
		z = m_dCenter[2]-m_dFOV[1]/2*m_dFreqDirection[2]-m_dFOV[0]/2*m_dPhaseDirection[2];
		
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
		sdc[0] = m_pMRIImage.scanField.SliceDirX;
		sdc[1] = m_pMRIImage.scanField.SliceDirY;
		sdc[2] = m_pMRIImage.scanField.SliceDirZ;
		dslice = 1;
	}
	Matrix Mdc(3,3);
	Mdc.SetCol(0,m_dFreqDirection[0],m_dFreqDirection[1],m_dFreqDirection[2]);
	Mdc.SetCol(1,m_dPhaseDirection[0],m_dPhaseDirection[1],m_dPhaseDirection[2]);
	Mdc.SetCol(2,sdc[0],sdc[1],sdc[2]);

	PixelSpacing[0] = m_dFOV[0]/nResolutionX;
	PixelSpacing[1] = m_dFOV[1]/nResolutionY;
	
	Matrix D(3,3);
	for (i=0;i<3;i++)
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
			m_M[i][j] = M.value[i][j];
		}
	}
	delete []ImageCentern;
	delete []ImageCenter1;
	delete []ImageCenter2;
	delete []m_dCenter;
	delete []m_dFreqDirection;
	delete []m_dPhaseDirection;
	return 1;
	
}

int CGradientCorrection1::vox2ras_0to1(float** ppM0, float** ppM1)
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

int CGradientCorrection1:: LoadMRIUnwarpData(CString sFileName)
{

	std::ifstream inFile( sFileName, std::ios::in | std::ios::binary );
	if ( !inFile )
	{
		//CLang::GetHandle().ShowMessage( CLang::GetHandle().GetCharRes("MSG___FAIL_TO_OPEN_GWV_FILE") );
		return ERR_FAIL_TO_OPEN_FILE;
	}
	inFile.seekg( 0, std::ios::seekdir::beg);

	inFile.read((char*)(&ncoords_x),4);
	inFile.read((char*)(&ncoords_y),4);
	inFile.read((char*)(&ncoords_z),4);
	inFile.read((char*)(coords_x),8);
	inFile.read((char*)(coords_y),8);
	inFile.read((char*)(coords_z),8);
	Dx = new float[ncoords_x*ncoords_y*ncoords_z];
	Dy = new float[ncoords_x*ncoords_y*ncoords_z];
	Dz = new float[ncoords_x*ncoords_y*ncoords_z];
	JacDetx = new float[ncoords_x*ncoords_y*ncoords_z];
	JacDety = new float[ncoords_x*ncoords_y*ncoords_z];
	JacDetz = new float[ncoords_x*ncoords_y*ncoords_z];
	inFile.read((char*)(Dx),ncoords_x*ncoords_y*ncoords_z*4);
	inFile.read((char*)(Dy),ncoords_x*ncoords_y*ncoords_z*4);
	inFile.read((char*)(Dz),ncoords_x*ncoords_y*ncoords_z*4);
	inFile.read((char*)(JacDetx),ncoords_x*ncoords_y*ncoords_z*4);
	inFile.read((char*)(JacDety),ncoords_x*ncoords_y*ncoords_z*4);
	inFile.read((char*)(JacDetz),ncoords_x*ncoords_y*ncoords_z*4);
	inFile.close();

	coords_x[0] = coords_x[0]*1000;
	coords_x[1] = coords_x[1]*1000;
	coords_y[0] = coords_y[0]*1000;
	coords_y[1] = coords_y[1]*1000;
	coords_z[0] = coords_z[0]*1000;
	coords_z[1] = coords_z[1]*1000;

	for (int i=0;i<ncoords_x*ncoords_y*ncoords_z;i++)
	{
		Dx[i] = Dx[i]*1000;
		Dy[i] = Dy[i]*1000;
		Dz[i] = Dz[i]*1000;
	}

	return 1;
}



int CGradientCorrection1::rcs2index(int SizeRCS,int* R,int dr,int* C,int dc,
		int* S,int ds, int nr,int nc,int ns,int* indexvec)
{
	for(int i=0;i<SizeRCS;i++)
	{
		indexvec[i] = R[i]+dr+(C[i]-1+dc)*nr+(S[i]+ds-1)*nr*nc;
	}
	return 1;
}

int  CGradientCorrection1::interp_trilin(int SizeRCS,float* R,float* C,float* S,float* M,
										int nr,int nc,int ns, float* V_out)
{

	int dr,dc,ds;
	int* Rint;
	int* Cint;
	int* Sint;
	float** Wr;
	float** Wc;
	float** Ws;
	Wr = new float* [2];
	Wc = new float* [2];
	Ws = new float* [2];
	int** indexvec = new int* [8];
	for (int k=0;k<8;k++)
	{
		indexvec[k] = new int [SizeRCS];
	}

	Rint = new int [SizeRCS];
	Cint = new int [SizeRCS];
	Sint = new int [SizeRCS];

	int index=0;
	for (int j=0;j<2;j++)
	{
		Wr[j] = new float [SizeRCS];
		Wc[j] = new float [SizeRCS];
		Ws[j] = new float [SizeRCS];
	}

	for (int i=0;i<SizeRCS;i++)
	{
		R[i] = max(1,min(nr-1,R[i]));
		C[i] = max(1,min(nc-1,C[i]));
		S[i] = max(1,min(ns-1,S[i]));

		V_out[i] = 0;
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
				rcs2index(SizeRCS, Rint,dr,Cint,dc,Sint,ds,nr,nc,ns,indexvec[index]);
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
					V_out[i] = V_out[i]+M[indexvec[index][i]-1]*Wr[dr][i]*Wc[dc][i]*Ws[ds][i];
					index = index+1;
				}
				
			}
		
		}
		index = 0;
	}
	

	
	delete []Rint;
	delete []Cint;
	delete []Sint;
	for (i=0; i<8;i++)
	{
		delete []indexvec[i];
	}
	delete []indexvec;
	for ( i = 0; i < 2; i++ )
	{
		delete []Wr[i];
		delete []Wc[i];
		delete []Ws[i];
	}
	delete []Wr;
	delete []Wc;
	delete []Ws;

	return 1;
	
}

int CGradientCorrection1::proj(int unwarpdims, float** ppMdc,float** Din,int size,float* dx, float* dy, float* dz)
{
	Matrix Mdci(3,3);
	Matrix Mdc(3,3);
	
	Matrix Mtmp(3,3);
	Matrix Mdiag1(3,3);
	Matrix Mdiag2(3,3);

	Mdiag1.value[0][0] = 1;
	Mdiag1.value[1][1] = 1;
	Mdiag2.value[2][2] = 1;
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
	
	float** Dout = new float*[3];
	Mdci = Mdc.Inverse();
	for (j=0;j<3;j++)
	{
		Dout[j] = new float [size];
	}
	
	if (unwarpdims==1)
	{
		Mtmp = Mdc*Mdiag2*Mdci;
		for (i=0;i<size;i++)
		{
			Dout[0][i] = Mtmp.value[0][0]*Din[0][i]+Mtmp.value[0][1]*Din[1][i]+Mtmp.value[0][2]*Din[2][i];
			Dout[1][i] = Mtmp.value[1][0]*Din[0][i]+Mtmp.value[1][1]*Din[1][i]+Mtmp.value[1][2]*Din[2][i];
			Dout[2][i] = Mtmp.value[2][0]*Din[0][i]+Mtmp.value[2][1]*Din[1][i]+Mtmp.value[2][2]*Din[2][i];
		}
	}
	else if (unwarpdims==2)
	{
		Mtmp = Mdc*Mdiag1*Mdci;
		for (i=0;i<size;i++)
		{
			Dout[0][i] = Mtmp.value[0][0]*Din[0][i]+Mtmp.value[0][1]*Din[1][i]+Mtmp.value[0][2]*Din[2][i];
			Dout[1][i] = Mtmp.value[1][0]*Din[0][i]+Mtmp.value[1][1]*Din[1][i]+Mtmp.value[1][2]*Din[2][i];
			Dout[2][i] = Mtmp.value[2][0]*Din[0][i]+Mtmp.value[2][1]*Din[1][i]+Mtmp.value[2][2]*Din[2][i];
		}
	}
	else
	{
		for(i=0;i<3;i++)
		{
			memcpy(Dout[i],Din[i],sizeof(float)*size);

		}
		
	}
	memcpy(dx,Dout[0],sizeof(float)*size);
	memcpy(dy,Dout[1],sizeof(float)*size);
	memcpy(dz,Dout[2],sizeof(float)*size);

	for (i=0;i<3;i++)
	{
		delete []Dout[i];
	}
	delete []Dout;
	return 1;
}

int CGradientCorrection1::jdproj(float** JDin,int size,float** ppMdc,int unwarpdims,int maxjd,float* JDout)
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
	float** JDtmp = new float*[3];
	Mdci = Mdc.Inverse();
	for (j=0;j<3;j++)
	{
		JDtmp[j] = new float [size];
	}
	
	if (unwarpdims==1)
	{
		Mtmp1 = Mdc*Mdiag2*Mdci;
		Mtmp2 = Mdc*Mdiag1*Mdci;
		for (i=0;i<size;i++)
		{
			JDtmp[0][i] = Mtmp1.value[0][0]*JDin[0][i]+Mtmp1.value[0][1]*JDin[1][i]+Mtmp1.value[0][2]*JDin[2][i]+Mtmp2.value[0][0]+Mtmp2.value[0][1]+Mtmp2.value[0][2];
			JDtmp[1][i] = Mtmp1.value[1][0]*JDin[0][i]+Mtmp1.value[1][1]*JDin[1][i]+Mtmp1.value[1][2]*JDin[2][i]+Mtmp2.value[1][0]+Mtmp2.value[1][1]+Mtmp2.value[1][2];
			JDtmp[2][i] = Mtmp1.value[2][0]*JDin[0][i]+Mtmp1.value[2][1]*JDin[1][i]+Mtmp1.value[2][2]*JDin[2][i]+Mtmp2.value[2][0]+Mtmp2.value[2][1]+Mtmp2.value[2][2];
		}
	}
	else if (unwarpdims==2)
	{
		Mtmp2 = Mdc*Mdiag2*Mdci;
		Mtmp1 = Mdc*Mdiag1*Mdci;
		for (i=0;i<size;i++)
		{
			JDtmp[0][i] = Mtmp1.value[0][0]*JDin[0][i]+Mtmp1.value[0][1]*JDin[1][i]+Mtmp1.value[0][2]*JDin[2][i]+Mtmp2.value[0][0]+Mtmp2.value[0][1]+Mtmp2.value[0][2];
			JDtmp[1][i] = Mtmp1.value[1][0]*JDin[0][i]+Mtmp1.value[1][1]*JDin[1][i]+Mtmp1.value[1][2]*JDin[2][i]+Mtmp2.value[1][0]+Mtmp2.value[1][1]+Mtmp2.value[1][2];
			JDtmp[2][i] = Mtmp1.value[2][0]*JDin[0][i]+Mtmp1.value[2][1]*JDin[1][i]+Mtmp1.value[2][2]*JDin[2][i]+Mtmp2.value[2][0]+Mtmp2.value[2][1]+Mtmp2.value[2][2];
		}
	}
	else
	{
		for(i=0;i<3;i++)
		{
			memcpy(JDtmp[i],JDin[i],sizeof(float)*size);
			
		}
			 
	}
	for (i=0;i<size;i++)
	{
		JDout[i] = JDtmp[0][i]*JDtmp[1][i]*JDtmp[2][i];
		JDout[i] = Max(JDout[i],0);
		JDout[i] = Min(JDout[i],maxjd);
	}
	for (i=0;i<3;i++)
	{
		delete []JDtmp[i];
	}
	delete []JDtmp;
	return 1;
}

int CGradientCorrection1::unwarp_resample(short** imvol,float** ppM1,int volsize[3],
		float** ppMout,float** ppMdc,int Jacobianflag,int inflag,int thruflag,float** imvol_out)

{
	CString interp_m = "linear";
	int i,j;
	float R_max,C_max,R_min,C_min;
	R_max = 0;
	C_max = 0;
	R_min = 512;
	C_min = 512;
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

	int nv = ncoords_x*ncoords_y*ncoords_z;
	int nrows_out = volsize[0];
	int ncols_out = volsize[1];
	int nslices_out = volsize[2];
	int NumVoxel = nrows_out*ncols_out;
	Matrix Mgw(4,4);
	Mgw.value[0][0] = 0;
	Mgw.value[0][1] = (coords_x[1]-coords_x[0])/(ncoords_x-1);
	Mgw.value[0][2] = 0;
	Mgw.value[0][3] = coords_x[0]-(coords_x[1]-coords_x[0])/(ncoords_x-1);

	Mgw.value[1][0] = (coords_y[1]-coords_y[0])/(ncoords_y-1);
	Mgw.value[1][1] = 0;
	Mgw.value[1][2] = 0;
	Mgw.value[1][3] = coords_y[0]-(coords_y[1]-coords_y[0])/(ncoords_y-1);

	Mgw.value[2][0] = 0;
	Mgw.value[2][1] = 0;
	Mgw.value[2][2] = (coords_z[1]-coords_z[0])/(ncoords_z-1);
	Mgw.value[2][3] = coords_z[0]-(coords_z[1]-coords_z[0])/(ncoords_z-1);

	Mgw.value[3][0] = 0;
	Mgw.value[3][1] = 0;
	Mgw.value[3][2] = 0;
	Mgw.value[3][3] = 1;
	Matrix Mgwi = Mgw.Inverse();

	short ***m_imvol = new short**[ncols_out];
	for (int t=0;t<ncols_out;t++)
	{
		m_imvol[t] = new short* [nrows_out];
	}
	for (int w=0;w<ncols_out;w++)
	{
		for(int x=0;x<nrows_out;x++)
		{
			m_imvol[w][x] = new short [nslices_out];
			
		}
		
	}
	
	float *m_R1 = new float[NumVoxel];
	float *m_C1 = new float[NumVoxel];
	float *m_S1 = new float[NumVoxel];
	float *im1vec = new float[NumVoxel];

	int liv_RowNum=0;
	//int iRow = 0;
	for (i=0;i<nslices_out;i++)
	{
		for(int p=1;p<NumVoxel+1;p++)
		{
			if (p%ncols_out==0)
			{
				liv_RowNum = floor((float)(p)/ncols_out)-1;
				m_imvol[liv_RowNum][nrows_out-1][i] = imvol[i][p-1];
			}
			else
			{
				liv_RowNum = floor((float)(p)/ncols_out);
				m_imvol[liv_RowNum][p%ncols_out-1][i] = imvol[i][p-1];
			}
		}
		
	}
	
	float* R = new float[NumVoxel];
	float* C = new float[NumVoxel];
	float* S = new float[NumVoxel];
	float* R1 = new float[NumVoxel];
	float* C1 = new float[NumVoxel];
	float* S1 = new float[NumVoxel];
	float* imindx = new float[NumVoxel];
	float* X = new float[NumVoxel];
	float* Y = new float[NumVoxel];
	float* Z = new float[NumVoxel];
	float* X1 = new float[NumVoxel];
	float* Y1 = new float[NumVoxel];
	float* Z1 = new float[NumVoxel];
	float* Rgw = new float[NumVoxel];
	float* Cgw = new float[NumVoxel];
	float* Sgw = new float[NumVoxel];
	float* Dx1 = new float[NumVoxel];
	float* Dy1 = new float[NumVoxel];
	float* Dz1 = new float[NumVoxel];
	float* Dx2 = new float[NumVoxel];
	float* Dy2 = new float[NumVoxel];
	float* Dz2 = new float[NumVoxel];
	float* JacDetx1 = new float[NumVoxel];
	float* JacDety1 = new float[NumVoxel];
	float* JacDetz1 = new float[NumVoxel];
	float* JacDet1 = new float[NumVoxel];
	float* im1vec2 = new float[NumVoxel];//

	float** Din = new float* [3];
	Din[0] = new float[NumVoxel];
	Din[1] = new float[NumVoxel];
	Din[2] = new float[NumVoxel];
	float* m_nflag = new float[NumVoxel];
	
	for (i=0;i<nrows_out;i++)
	{
		for (j=0;j<ncols_out;j++)
		{
			R[i*ncols_out+j] = i+1;
			C[i*ncols_out+j] = j+1;
			imindx[i*ncols_out+j] = R[i*ncols_out+j]-1 + (C[i*ncols_out+j]-1)*nrows_out;
		}
		
	}


	for (j=0;j<nslices_out;j++)
	{
		
		for(i=0;i<NumVoxel;i++)
		m_nflag[i] = 1;
		for (i=0;i<NumVoxel;i++)
		{
			S[i] = j+1; 
			X[i] = ppMout[0][0]*R[i]+ppMout[0][1]*C[i]+ppMout[0][2]*S[i]+ppMout[0][3];
			Y[i] = ppMout[1][0]*R[i]+ppMout[1][1]*C[i]+ppMout[1][2]*S[i]+ppMout[1][3];
			Z[i] = ppMout[2][0]*R[i]+ppMout[2][1]*C[i]+ppMout[2][2]*S[i]+ppMout[2][3];
			Rgw[i] = Mgwi.value[0][0]*X[i]+Mgwi.value[0][1]*Y[i]+Mgwi.value[0][2]*Z[i]+Mgwi.value[0][3];
			Cgw[i] = Mgwi.value[1][0]*X[i]+Mgwi.value[1][1]*Y[i]+Mgwi.value[1][2]*Z[i]+Mgwi.value[1][3];
			Sgw[i] = Mgwi.value[2][0]*X[i]+Mgwi.value[2][1]*Y[i]+Mgwi.value[2][2]*Z[i]+Mgwi.value[2][3];
		}		

		interp_trilin(NumVoxel,Rgw,Cgw,Sgw,Dx,ncoords_y,ncoords_x,ncoords_z,Dx1);
		interp_trilin(NumVoxel,Rgw,Cgw,Sgw,Dy,ncoords_y,ncoords_x,ncoords_z,Dy1);
		interp_trilin(NumVoxel,Rgw,Cgw,Sgw,Dz,ncoords_y,ncoords_x,ncoords_z,Dz1);
		
		memcpy(Din[0],Dx1,sizeof(float)*NumVoxel);
		memcpy(Din[1],Dy1,sizeof(float)*NumVoxel);
		memcpy(Din[2],Dz1,sizeof(float)*NumVoxel);

		proj(unwarpdims,ppMdc,Din,NumVoxel,Dx2,Dy2,Dz2);			
		if (Jacobianflag)
		{		
			if (!SGflag)
			{
				interp_trilin(NumVoxel,Rgw,Cgw,Sgw,JacDetx,ncoords_y,ncoords_x,ncoords_z,JacDetx1);
				interp_trilin(NumVoxel,Rgw,Cgw,Sgw,JacDety,ncoords_y,ncoords_x,ncoords_z,JacDety1);
				interp_trilin(NumVoxel,Rgw,Cgw,Sgw,JacDetz,ncoords_y,ncoords_x,ncoords_z,JacDetz1);
				float** JDintmp = new float* [3];
				JDintmp[0] = new float[NumVoxel];
				JDintmp[1] = new float[NumVoxel];
				JDintmp[2] = new float[NumVoxel];
				memcpy(JDintmp[0],JacDetx1,sizeof(float)*NumVoxel);
				memcpy(JDintmp[1],JacDety1,sizeof(float)*NumVoxel);
				memcpy(JDintmp[2],JacDetz1,sizeof(float)*NumVoxel);
				
				jdproj(JDintmp,NumVoxel,ppMdc,unwarpdims,3,JacDet1);
				for (i=0;i<3;i++)
				{
					delete []JDintmp[i];
				}
				delete []JDintmp;
			}
		}
		for (i=0;i<NumVoxel;i++)
		{
			X1[i] = X[i]+Dx2[i];
			Y1[i] = Y[i]+Dy2[i];
			Z1[i] = Z[i]+Dz2[i];
			R1[i] = Mi.value[0][0]*X1[i]+Mi.value[0][1]*Y1[i]+Mi.value[0][2]*Z1[i]+Mi.value[0][3];
			C1[i] = Mi.value[1][0]*X1[i]+Mi.value[1][1]*Y1[i]+Mi.value[1][2]*Z1[i]+Mi.value[1][3];
			S1[i] = Mi.value[2][0]*X1[i]+Mi.value[2][1]*Y1[i]+Mi.value[2][2]*Z1[i]+Mi.value[2][3];
		}

		for (i=0;i<NumVoxel;i++)
		{
			//if (R1[i]<1 || C1[i]<1 || S1[i]<1 || R1[i]>nrows_out || C1[i]>ncols_out || S1[i]>nslices_out+0.5)
			if (R1[i]<1 || C1[i]<1 || R1[i]>nrows_out || C1[i]>ncols_out )
			{
				m_nflag[i] = 0;
				
			}
			if (ceil(i/ncols_out)==nrows_out/2)
			{
				C_max = Max(C_max,C1[i]);
				C_min = Min(C_min,C1[i]);
			}
			if (i%ncols_out==ncols_out/2)
			{
				R_min = Min(R_min,R1[i]);
				R_max = Max(R_max,R1[i]);
			}
			

			m_R1[i] = Min(Max(R1[i],1),nrows_out)-1;
			m_C1[i] = Min(Max(C1[i],1),ncols_out)-1;
			m_S1[i] = Min(Max(S1[i],1),nslices_out)-1;
		
		}
	
		
		interp3(im1vec, m_imvol,m_C1,m_R1,m_S1,interp_m, ncols_out,nrows_out,nslices_out);
		for (i=0;i<NumVoxel;i++)
		{
			im1vec[i] = im1vec[i]*m_nflag[i];
		}
		for (i=0;i<NumVoxel;i++)
		{	
			im1vec2[(int)imindx[i]] = im1vec[i];
		}		
		
		if (Jacobianflag)
		{
			float* imjdvec = new float[NumVoxel];
			for (i=0;i<NumVoxel;i++)
			{
				imjdvec[(int)imindx[i]] = JacDet1[i];	
			}
			for(i=0;i<NumVoxel;i++)
			{
				im1vec2[i] = Min(Max(im1vec2[i]*imjdvec[i],0),4095);
			}
			delete imjdvec;
			 
		}
		


		memcpy(imvol_out[j],im1vec2,sizeof(float)*NumVoxel);
		
	}
///	exitM();


	int m_colindex = 0;
	int m_rowindex = 0;
	float m_ijex = 0;

	delete []R;
	delete []C;
	delete []S;
	delete []R1;
	delete []C1;
	delete []S1;
	delete [] imindx ;
	delete [] X ;
	delete [] Y ;
	delete [] Z ;
	delete [] X1 ;
	delete [] Y1 ;
	delete [] Z1 ;
	delete [] Rgw ;
	delete [] Cgw ;
	delete [] Sgw;
	delete [] Dx1;
	delete [] Dy1;
	delete [] Dz1;
	delete [] Dx2;
	delete [] Dy2;
	delete [] Dz2;
	delete [] m_nflag;
	delete [] m_R1;
	delete [] m_C1;
	delete [] m_S1;

	for(i=0;i<3;i++)
	{
		delete []Din[i];
	}
	delete []Din;
	if ( NULL != JacDetx1 )
	{
		delete []JacDetx1;
	}
	if ( NULL != JacDety1 )
	{
		delete []JacDety1;
	}
	if ( NULL != JacDetz1 )
	{
		delete []JacDetz1;
	}

	delete [] JacDet1;
	delete [] im1vec2; //? why can't be deleted
//	delete [] imjdvec;
	delete [] im1vec;
	for (w=0;w<ncols_out;w++)
	{
		for(int x=0;x<nrows_out;x++)
		{
			delete []m_imvol[w][x];
			
		}
		
	}
	for (t=0;t<ncols_out;t++)
	{
		delete []m_imvol[t];
	}
	delete []m_imvol;
	
	
//	fclose(fp);
	return 1;
}


int CGradientCorrection1::GradientCorrection(CMRISeries* pImage)
{
//	if (nNumOfImages < 3)
//	{
//		return 1;
//	}
//	FILE *fp = fopen("D:\\Time.txt", "a");
//	CString str;
	DWORD t1=GetTickCount();//程序段开始前取得系统运行时间(ms)
	DWORD t2;
	if ( !m_bLoadedUnwarpRefData )
	{
		LoadMRIUnwarpData( m_sRefDataFileName );
	//	m_bLoadedUnwarpRefData = true;
	} 
	
	//Read the ini file for flag value
	UINT inflag = 1, thruflag = 0, jacflag = 1;
	/*HKEY hKey;	
	long lResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\XinAoMDT\\Prospect", 0, KEY_READ, &hKey);
	if(lResult == ERROR_SUCCESS)
	{
		char szDir[MAX_PATH];
		DWORD dwSize = MAX_PATH;
		lResult = RegQueryValueEx( hKey, "InstallDir", NULL, NULL, (BYTE*) szDir, &dwSize);
		if(lResult == ERROR_SUCCESS)
		{
			CString sIniPath = szDir;
			sIniPath += "\\bin\\xbmri.ini";

			inflag = GetPrivateProfileInt("GRADIENT_CORRECTION", "InFlag", 1, sIniPath);// Default Inflag = 1;
			thruflag = GetPrivateProfileInt("GRADIENT_CORRECTION", "ThruFlag", 0, sIniPath); //Default thruflag = 0;
			jacflag = GetPrivateProfileInt("GRADIENT_CORRECTION", "JacFlag", 1, sIniPath);   //Default JacFlag = 1;
		}
	}	*/

	

	int i;	
	int nNumOfImages=pImage->GetNumOfImages();
	int nResolutionX = pImage->GetImageAt(0)->GetImageHeader()->seriesField.ResolutionX;
	int nResolutionY = pImage->GetImageAt(0)->GetImageHeader()->seriesField.ResolutionY;
	int m_nNumOfPts = nResolutionX*nResolutionY;
	float* pdparm = new float [4];
	float** ppM = new float* [4];

	for (i=0;i<4;i++)
	{
		ppM[i] = new float[4];
	}

	short** ppinvol= new short* [nNumOfImages];
	for (i=0;i<nNumOfImages;i++)
	{
		ppinvol[i] = new short[m_nNumOfPts];
	}
	
	MR_ImageHeader* pImageHeader;
	short** ppData;
	pImageHeader=new MR_ImageHeader[pImage->GetNumOfImages()];
	ppData=new short*[pImage->GetNumOfImages()];
	for(i=0;i<pImage->GetNumOfImages();i++)
	{
		pImageHeader[i]=*pImage->GetImageAt(i)->GetImageHeader();
		ppData[i]=(short *)pImage->GetImageAt(i)->GetColorOfRawData();
	}


	LoadMRISeries2Vol(nNumOfImages, pImageHeader, ppData,ppinvol,ppM);
	

	float** ppMdc;
	ppMdc =new float*[3];
	ppMdc[0] = new float[3];
	ppMdc[1] = new float[3];
	ppMdc[2] = new float[3];
	int size_vol[3];
	float* c_rcs = new float[3];
	size_vol[0] = nResolutionY;
	size_vol[1] = nResolutionX;
	size_vol[2] = nNumOfImages;

	vox2ras_xform(ppM,size_vol,ppMdc,c_rcs);
	float** ppM1 = new float*[4];
	for (i=0;i<4;i++)
	{	
		ppM1[i] = new float[4];
	}
	vox2ras_0to1(ppM,ppM1);
	float** ppMout = new float* [4];
	for (i=0;i<4;i++)
	{	
		ppMout[i] = new float[4];
	}
	for (i=0;i<4;i++)
	{
		for (int j=0;j<4;j++)
		{
			ppMout[i][j] = ppM1[i][j];
		}
	}

	
	float** imvol_out =new float* [nNumOfImages];
	for (i=0;i<nNumOfImages;i++)
	{
		imvol_out[i] = new float[nResolutionX*nResolutionY];
	} 
	
	unwarp_resample(ppinvol,ppM1,size_vol,ppMout,
		ppMdc,jacflag,inflag,thruflag,imvol_out);
	
	for (i=0;i<size_vol[2];i++)
	{
//		memcpy(ppData[i],imvol_out[i], sizeof(short)* (nResolutionX*nResolutionY));
		for ( int j=0; j<nResolutionX*nResolutionY; j++ )
		{
		ppData[i][j] = imvol_out[i][j];
		}
	}

	/*FILE *file;
	file = fopen ("d:\\1.txt", "w+b");
	fseek(file,0,SEEK_SET);
	

	for(i=0;i<nNumOfImages;i++)
	{
		fwrite((char*)ppData[i],sizeof(char),sizeof(short)*m_nNumOfPts,file);
	}
	fclose(file);*/


	delete []pImageHeader;
	delete ppData;

	delete []c_rcs;
	for (i=0;i<nNumOfImages;i++)
	{
		delete []imvol_out[i];
		
	}
	delete []imvol_out;
	for(i=0;i<3;i++)
	{
		delete []ppMdc[i];

	}

	delete []ppMdc;

	for (i=0;i<nNumOfImages;i++)
	{
		delete []ppinvol[i];
		
	}
	delete []ppinvol;
	for (i=0;i<4;i++)
	{
		delete []ppM1[i];
		delete []ppMout[i];
		delete []ppM[i];
		
	}
	delete []ppM1;
	delete []ppMout;
	delete []ppM;
	delete []pdparm;

	t2=GetTickCount();//程序段开始前取得系统运行时间(ms)
//	str.Format("Before stop:%dms",t2-t1);//前后之差即 程序运行时间
//	fprintf(fp, "%s ",str);
//	t1 = t2;
//	fclose(fp);
	return t2-t1;
}

int CGradientCorrection1::vox2ras_xform(float** M,int size_vol[3],float** ppMdc,float* c_ras)
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
	float m_sum=0;
	int p,q;
	for ( p=0;p<3;p++)
	{
		for (q=0;q<3;q++)
		{
			m_sum = m_sum+MdcD.value[q][p]*MdcD.value[q][p];

		}
		delta[p] = sqrt(m_sum);
		m_sum = 0;
	}
	for(p=0;p<3;p++)
	{
		for(q=0;q<3;q++)
		{
			ppMdc[p][q] = MdcD.value[p][q]/delta[q];
		}
	}
	Matrix Pcrs_c(4,1);
	Pcrs_c.value[0][0] = size_vol[0]/2;
	Pcrs_c.value[1][0] = size_vol[1]/2;
	Pcrs_c.value[2][0] = (float)(size_vol[2])/2;
	Pcrs_c.value[3][0] = 1;
	Matrix Mtmp(4,4);
	for(i=0;i<4;i++)
	{
		for(j=0;j<4;j++)
		{
			Mtmp.value[i][j] = M[i][j];
		}
	}
	
	Matrix Pxyz_c(4,1);
	Pxyz_c = Mtmp*Pcrs_c;
	float spe = 0;
	for (i=0;i<4;i++)
	{
		spe = spe+Mtmp.value[0][i]*Pcrs_c.value[i][0];
	}
	c_ras[0] = Pxyz_c.value[0][0];
	c_ras[1] = Pxyz_c.value[1][0];
	c_ras[2] = Pxyz_c.value[2][0];
	
	return 1;
	
}

int CGradientCorrection1::interp3(float* im1vec, short ***in_mvol,float *m_C1,float *m_R1,float *m_S1,CString interp_m, int rsize, int csize, int ssize)
{
    int i;//Used to control lengths of each array
	int x0,x1,y0,y1,z0,z1;//Used to get the extension of a cube
//	float *op_mvol;//used to get output
//Judge type if type is trilinear contiune
//In this function I define 50 elements in each array
	//op_mvol = new float[csize*rsize];
	if(interp_m=="linear") 
  {
    for(i=0;i<csize*rsize;i++)
	{
		if(m_C1[i]==csize-1)
		{
			x0=m_C1[i]-1;
			x1=m_C1[i];
		}
		else
		{
			x0=floor(m_C1[i]);
			x1=x0+1;
		}

		if(m_R1[i]==rsize-1)
		{
			y0=m_R1[i]-1;
			y1=m_R1[i];
		}
		else
		{
			y0=floor(m_R1[i]);
			y1=y0+1;
		}
		if(m_S1[i]==ssize-1)
		{
			z0=m_S1[i]-1;
			z1=m_S1[i];
		}
		else
		{
			z0=floor(m_S1[i]);
			z1=z0+1;
		}
//			x0=floor(m_C1[i]);
//			x1=x0+1;
//			y0=floor(m_R1[i]);
//			y1=y0+1;
//			z0=floor(m_S1[i]);
//			z1=z0+1;
	//use formula to get output
		im1vec[i]=(in_mvol[x0][y0][z0])*(x1 - m_C1[i])*(y1 - m_R1[i])*(z1 - m_S1[i])+
					   (in_mvol[x1][y0][z0])*(m_C1[i]-x0)*(y1 - m_R1[i])*(z1 - m_S1[i])+
					   (in_mvol[x0][y1][z0])*(x1 - m_C1[i])*(m_R1[i]-y0)*(z1 - m_S1[i])+
					   (in_mvol[x0][y0][z1])*(x1 - m_C1[i])*(y1 - m_R1[i])*(m_S1[i]-z0)+
					   (in_mvol[x1][y0][z1])*(m_C1[i]-x0)*(y1 - m_R1[i])*(m_S1[i]-z0)+
					   (in_mvol[x0][y1][z1])*(x1 - m_C1[i])*(m_R1[i]-y0)*(m_S1[i]-z0)+
					   (in_mvol[x1][y1][z0])*(m_C1[i]-x0)*(m_R1[i]-y0)*(z1 - m_S1[i])+
					   (in_mvol[x1][y1][z1])*(m_C1[i]-x0)*(m_R1[i]-y0)*(m_S1[i]-z0);
		
	}
  }
//return output
	return 1;
}

/////////////////////////////////////////////////////////////////


