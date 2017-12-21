// DICOM2XINAO.cpp: implementation of the CDICOM2XINAO class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DICOM2XINAO.h"

#include "dcmtk/dcmdata/dcdatset.h"
#include "dcmtk/config/osconfig.h"    /* make sure OS specific configuration is included first */
#include "dcmtk/ofstd/ofstdinc.h"
#include "dcmtk/dcmnet/dimse.h"
#include "dcmtk/dcmnet/diutil.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcdebug.h"
#include "dcmtk/dcmdata/dcuid.h"
#include "dcmtk/dcmdata/dcdict.h"
#include "dcmtk/dcmdata/cmdlnarg.h"
#include "dcmtk/ofstd/ofconapp.h"
#include "dcmtk/ofstd/ofstd.h"
#include "dcmtk/ofstd/ofdatime.h"
#include "dcmtk/dcmdata/dcuid.h"         /* for dcmtk version name */
#include "dcmtk/dcmnet/dicom.h"         /* for DICOM_APPLICATION_ACCEPTOR */
#include "dcmtk/dcmdata/dcdeftag.h"      /* for DCM_StudyInstanceUID */
#include "dcmtk/dcmdata/dcostrmz.h"      /* for dcmZlibCompressionLevel */
#include "dcmtk/dcmnet/dcasccfg.h"      /* for class DcmAssociationConfiguration */
#include "dcmtk/dcmnet/dcasccff.h"      /* for class DcmAssociationConfigurationFile */

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDICOM2XINAO::CDICOM2XINAO()
{
	m_bToshibaCTTable=FALSE;
}

CDICOM2XINAO::~CDICOM2XINAO()
{
}

int  CDICOM2XINAO::ConvertMRDcm2XB(char* lpDcm, int nDcmSize, LPMR_STRUCT_IMAGE& pXinAoImg)
{
	int   nRet = 0 ;
	WORD* pBIImageData ;
	if (pXinAoImg != NULL)
	{
		return -10 ;   //should be null pointer.
	}
	pXinAoImg = new MR_STRUCT_IMAGE ;
	nRet = ConvertMRDcm2XB(pXinAoImg->oHeader,lpDcm,nDcmSize, pBIImageData) ;
	if (nRet == 0)
	{
		pXinAoImg->pData = (IMG_DATA_TYPE*)pBIImageData ;
	}


	return nRet ;
}

int CDICOM2XINAO::ConvertMRDcm2XB(MR_ImageHeader &BIImageHeader,char* lpDcm,int nDcmSize, WORD* &pBIImageData)
{
	char *lpTemp = (char*)lpDcm ;
	char szVR[3]=" " ;
	char szTemp[64] ;
	int  nImageSize = 1 ;
	int  nGroupLen  = 0 ;

	unsigned short sPoints ;
	int            count = 0 ;
	int            i     = 0 ;
	int            nVLength;
	double         dVal[20] ;
	double         dImgDirec[6] ;
	double         dXBImgDirec[12] ;
	double         dOffSet[3] ;
	double         dSliceThick = 5.0;

	int            nVM;
	short iGroupNum,iElementNum;

	lpTemp += 132;

	memset(&(BIImageHeader.headInfo), 0 ,sizeof(MR_ImageHeader) );
	// Initialize File Header
	//headInfo
    BIImageHeader.headInfo.BITitle[0] = 'D';
    BIImageHeader.headInfo.BITitle[1] = 'C';
    BIImageHeader.headInfo.BITitle[2] = 'M';
    BIImageHeader.headInfo.BITitle[3] = 'M';
    BIImageHeader.headInfo.BITitle[4] = 'R';
    BIImageHeader.headInfo.Version = int (1) << 16 + 0;
    BIImageHeader.headInfo.Size = sizeof (MR_HeaderInfo);
    BIImageHeader.headInfo.NumHdrFields = 6; 
    BIImageHeader.headInfo.HeaderSize   = sizeof(MR_HeaderInfo) + sizeof(MR_SiteField) +
                                          sizeof(MR_PatientField) + sizeof(MR_StudyField) + 
										  sizeof(MR_SeriesField)  + sizeof(MR_ScanField) + 
										  sizeof (MR_ImageField);

	//SiteField
    BIImageHeader.siteField.Size = sizeof(MR_SiteField);

	//PatientField
	BIImageHeader.patientField.Size = sizeof(MR_PatientField);

	//StudyField
    BIImageHeader.studyField.Size = sizeof(MR_StudyField);

	//SeriesField
	BIImageHeader.seriesField.Size = sizeof(MR_SeriesField);
	BIImageHeader.seriesField.NumSlab = 1 ;
	BIImageHeader.seriesField.RcvCoil = ctSystemCoil;
	BIImageHeader.seriesField.TXCoil  = ctSystemCoil;

	//ScanField
    BIImageHeader.scanField.Size = sizeof(MR_ScanField);

	//ImageField
	BIImageHeader.imageField.Size = sizeof(MR_ImageField);

	while((lpTemp - lpDcm) < nDcmSize)
	{
		iGroupNum   = *((unsigned short*)lpTemp);
		lpTemp += 2;
		iElementNum = *((unsigned short*)lpTemp);
		lpTemp += 2;
		if (iGroupNum == 2)
		{
			szVR[0] = *lpTemp;
			lpTemp++;
			szVR[1] = *lpTemp;
			lpTemp++;
			if(strcmp(szVR,"OB")==0||strcmp(szVR,"OW")==0||
			   strcmp(szVR,"SQ")==0||strcmp(szVR,"UN")==0)
			{
				lpTemp  += 2;
				nVLength = *((int*)lpTemp);
				lpTemp  += 4;
			}
			else
			{
				nVLength = *((short*)lpTemp);
				lpTemp  += 2;
			}
		}
		else
		{
			nVLength = *((int*)lpTemp);
			lpTemp  += 4;
		}

		switch(iGroupNum)
		{
		case 0x0002:
			{
			switch(iElementNum)
			{
				case 0x0002: //Media Storage SOP Class UID
					if (strncmp(lpTemp, "1.2.840.10008.5.1.4.1.1.4", nVLength) != 0)
					{
						return -1 ;    //Invalidate SOP Class UID
					}
					break;
				case 0x0010: //Transfer Syntax UID
					if (strncmp(lpTemp, "1.2.840.10008.1.2", nVLength) != 0)
					{
						return -2 ;    //Not Support the Transfer Syntax
					}
					break;
				case 0x0016:
					
					break;
				default:
					break;
			}
			lpTemp += nVLength;
			break;
			}
		case 0x0008:
			{
			switch(iElementNum)
			{
			case 0x0008://*Image_Type			(0008,0008) Image Type CS 1-n
				if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\MPR", nVLength) == 0)
				{
					BIImageHeader.imageField.Type = itMPR;
				}
				else if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\PROJECTION IMAGE ", nVLength) == 0)
				{
					BIImageHeader.imageField.Type = itProjection;
				}
				else if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\IMAGE",nVLength) == 0)
				{
					BIImageHeader.imageField.Type=itImage;
				}
				else if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\T1 MAP",nVLength) == 0)
				{
					BIImageHeader.imageField.Type = itT1Map ;
				}
				else if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\T2 MAP", nVLength) == 0)
				{
					BIImageHeader.imageField.Type=itT2Map;
				}
				else if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\DIFFUSION MAP", nVLength) == 0)
				{
					BIImageHeader.imageField.Type=itDiffusionMap;
				}
				else if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\DENSITY MAP",nVLength) == 0)
				{
					BIImageHeader.imageField.Type = itDensityMap;
				}
				else if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\PHASE MAP",nVLength) == 0)
				{
					BIImageHeader.imageField.Type = itPhaseMap;
				}
				else if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\VELOCITY MAP",nVLength) == 0)
				{
					BIImageHeader.imageField.Type = itVelocityMap;
				}
				else if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\IMAGE ADDITION",nVLength)==0)
				{
					BIImageHeader.imageField.Type = itImageAddition ;
				}
				else if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\PHASE SUBTRACT",nVLength) == 0)
				{
					BIImageHeader.imageField.Type = itPhaseSubtract ;
				}
				else if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\MODULUS SUBTRACT", nVLength) == 0)
				{
					BIImageHeader.imageField.Type = itModulusSubtract ;
				}
				else if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\OTHER", nVLength) == 0)
				{
					BIImageHeader.imageField.Type = itOther ;
				}
				break;
			case 0x0016:  //SOP Class UID
				if (strncmp(lpTemp, "1.2.840.10008.5.1.4.1.1.4", nVLength) != 0)
				{
					return -4 ;    //Is not a MR image .
				}
				break;
			case 0x0020://*Study_Time			(0008,0020) Study Date DA 1	
				TimeConvert2(BIImageHeader.studyField.Time, lpTemp, 1);
				break;
			case 0x0021://*Series_DateTime			(0008,0021)Series Date DA 1	
				TimeConvert2(BIImageHeader.seriesField.DateTime, lpTemp, 1);
				break;
			case 0x0022://*Image_DateTime			(0008,0022)Acquisition Date DA 1	
				TimeConvert2(BIImageHeader.imageField.DateTime, lpTemp, 1);
				break;
			case 0x0030://*Study_Time			(0008,0030) Study Time TM 1	
				TimeConvert2(BIImageHeader.studyField.Time, lpTemp, 2);
				break;
			case 0x0031://*Series_DateTime			(0008,0031) Series Time TM 1	
				TimeConvert2(BIImageHeader.seriesField.DateTime, lpTemp, 2);
				break;
			case 0x0032://*image_DateTime			(0008,0032) Study Time TM 1	
				TimeConvert2(BIImageHeader.imageField.DateTime, lpTemp, 2);
				break;
			case 0x0080://*Site_InstName		(0008,0080) Institution Name LO 1
				strncpy(BIImageHeader.siteField.InstName, lpTemp, nVLength);
				if(*(lpTemp + nVLength - 1) == ' ')
				{
					BIImageHeader.siteField.InstName[nVLength-1] = 0x00;
				}
				break;
			case 0x0090://*Study_RefPhy			(0008,0090) Referring Physician'sName PN 1
				NameConvert2(BIImageHeader.studyField.RefPhy,lpTemp,nVLength);
				break;
			case 0x1010://*Site_StationName		(0008,1010) Station Name SH 1
				strncpy(BIImageHeader.siteField.StationName,lpTemp,nVLength);
				if(*(lpTemp + nVLength - 1) == ' ')
				{
					BIImageHeader.siteField.StationName[nVLength-1] = 0x00;
				}
				break;
			case 0x1030://*Study_Desc		(0008,1030) Study Description LO 1
				strncpy(BIImageHeader.studyField.Desc, lpTemp, nVLength);
				if(*(lpTemp + nVLength - 1) == ' ')
				{
					BIImageHeader.studyField.Desc[nVLength-1] = 0x00;
				}
				break;
			case 0x103E://*Series_Desc			(0008,103E) Series Description LO 1
				strncpy(BIImageHeader.seriesField.Desc, lpTemp, nVLength);
				if(*(lpTemp + nVLength - 1) == ' ')
				{
					BIImageHeader.seriesField.Desc[nVLength-1] = 0x00;
				}
				break;
			case 0x1040://*Site_InstDeptName		(0008,1040) Institutional Department Name LO 1
				strncpy(BIImageHeader.siteField.InstDeptName, lpTemp, nVLength);
				if(*(lpTemp + nVLength - 1) == ' ')
				{
					BIImageHeader.siteField.InstDeptName[nVLength-1] = 0x00;
				}
				break;
			case 0x1090://*Site_BISerialNum		(0008,1090) Manufacturer's Model Name LO 1
				strncpy(BIImageHeader.siteField.BISerialNum, lpTemp, nVLength);
				if(*(lpTemp + nVLength - 1) == ' ')
				{
					BIImageHeader.siteField.BISerialNum[nVLength-1] = 0x00;
				}
				break;
			case 0x1110: //*Site_BISerialNum		(0008,1090) Manufacturer's Model Name LO 1
				break;
			case 0x1111: //*Site_BISerialNum		(0008,1090) Manufacturer's Model Name LO 1
				break;
			default:
				break;
			}
			if ( nVLength > 0 )
			{
				lpTemp += nVLength;
			}
			break;
			}
		case 0x0009:
			{
				if (nVLength > 0)
				{
					lpTemp += nVLength;
				}
			break;
			}
		case 0x0010:
			{
			switch(iElementNum)
			{
			case 0x0010://*Patient_Name			(0010,0010) Patient's Name PN 1
				NameConvert2(BIImageHeader.patientField.Name, lpTemp, nVLength);
				break;
			case 0x0020://*Patient_Id			(0010,0020) Patient ID LO 1
				strncpy(BIImageHeader.patientField.Id, lpTemp, nVLength);
				if(*(lpTemp + nVLength - 1) == ' ')
				{
					BIImageHeader.patientField.Id[nVLength-1]=0x00;
				}
				break;
			case 0x0030://*Patient_BirthDate	(0010,0030) Patient's Birth Date DA 1
				TimeConvert2(BIImageHeader.patientField.BirthDate, lpTemp, 1);
				break;
			case 0x0032://*Patient_BirthDate	(0010,0032) Patient's Birth Time TM 1
				TimeConvert2(BIImageHeader.patientField.BirthDate, lpTemp, 2);
				break;
			case 0x0040://*Patient_Sex			(0010,0040) Patient's Sex CS 1
				if(*lpTemp == 'O')
				{
					BIImageHeader.patientField.Sex = 'U';
				}
				else
				{
					BIImageHeader.patientField.Sex = *lpTemp;
				}
				break;
			case 0x1010://*Patient_Age			(0010,1010) Patient's Age AS 1
				BIImageHeader.patientField.Age = AgeConvert2(lpTemp);
				break;
			case 0x1030://*Patient_Weight		(0010,1030) Patient's Weight DS 1
				strncpy(szTemp, lpTemp, nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				BIImageHeader.patientField.Weight = atoi(szTemp);
				break;
			case 0x21B0://*Patient_Desc			(0010,21B0) Additional Patient History LT 1
				strncpy(BIImageHeader.patientField.Desc,lpTemp,nVLength);
				if(*(lpTemp + nVLength - 1) == ' ')
				{
					BIImageHeader.patientField.Desc[nVLength-1] = 0x00;
				}
				break;
			default:
				break;
			}//switch iElementNum
			if (nVLength > 0)
			{
				lpTemp += nVLength;
			}
			break;
			}
		case 0x0018:
			{
			switch(iElementNum)
			{
			case 0x0015: //*PatBodyPart		(0018,0015) Body Part Examined CS 1
				if(strncmp(lpTemp, "SKULL ", nVLength) == 0)
				{
					BIImageHeader.studyField.PatBodyPart = bpHead;
				}
				else if(strncmp(lpTemp, "NECK", nVLength) == 0)
				{
					BIImageHeader.studyField.PatBodyPart = bpNeck;
				}
				else if(strncmp(lpTemp, "SHOULDER", nVLength) == 0)
				{
					BIImageHeader.studyField.PatBodyPart = bpShoulder;
				}
				else if(strncmp(lpTemp, "CSPINE", nVLength) == 0)
				{
					BIImageHeader.studyField.PatBodyPart = bpCervicalSpine;
				}
				else if(strncmp(lpTemp,"TSPINE",nVLength) == 0)
				{
					BIImageHeader.studyField.PatBodyPart = bpThoracicSpine;
				}
				else if(strncmp(lpTemp,"LSPINE",nVLength) == 0)
				{
					BIImageHeader.studyField.PatBodyPart = bpLumberSpine;
				}
				else if(strncmp(lpTemp,"CHEST ",nVLength) == 0)
				{
					BIImageHeader.studyField.PatBodyPart = bpChest;
				}
				else if(strncmp(lpTemp,"BREAST",nVLength) == 0)
				{
					BIImageHeader.studyField.PatBodyPart = bpBreast;
				}
				else if(strncmp(lpTemp,"ABODOMEN",nVLength) == 0)
				{
					BIImageHeader.studyField.PatBodyPart = bpAbdomen;
				}
				else if(strncmp(lpTemp,"PELVIS",nVLength) == 0)
				{
					BIImageHeader.studyField.PatBodyPart = bpPelvis;
				}
				else if(strncmp(lpTemp,"HIP ",nVLength) == 0)
				{
					BIImageHeader.studyField.PatBodyPart = bpHip;
				}
				else if(strncmp(lpTemp,"ELBOW ",nVLength) == 0)
				{
					BIImageHeader.studyField.PatBodyPart = bpElbow;
				}
				else if(strncmp(lpTemp,"WRIST ",nVLength) == 0)
					BIImageHeader.studyField.PatBodyPart = bpWrist;
				else if(strncmp(lpTemp,"ARM ",nVLength) == 0)
				{
					BIImageHeader.studyField.PatBodyPart = bpArm;
				}
				else if(strncmp(lpTemp,"HAND",nVLength) == 0)
				{
					BIImageHeader.studyField.PatBodyPart = bpHand;
				}
				else if(strncmp(lpTemp,"KNEE",nVLength) == 0)
				{
					BIImageHeader.studyField.PatBodyPart = bpKnee;
				}
				else if(strncmp(lpTemp,"LEG ",nVLength) == 0)
				{
					BIImageHeader.studyField.PatBodyPart = bpLeg;
				}
				else if(strncmp(lpTemp,"ANKLE ",nVLength)==0)
				{
					BIImageHeader.studyField.PatBodyPart = bpAnkle;
				}
				else if(strncmp(lpTemp,"FOOT",nVLength) == 0)
				{
					BIImageHeader.studyField.PatBodyPart = bpFoot;
				}
				else if(strncmp(lpTemp,"HEART ",nVLength) == 0)
				{
					BIImageHeader.studyField.PatBodyPart = bpHeart;
				}
				else if(strncmp(lpTemp,"VASCULARHEAD",nVLength) == 0)
				{
					BIImageHeader.studyField.PatBodyPart = bpVascularHead;
				}
				else if(strncmp(lpTemp,"VASCULARNECK",nVLength) == 0)
				{
					BIImageHeader.studyField.PatBodyPart = bpVascularNeck;
				}
				else if(strncmp(lpTemp,"VASCULARBODY",nVLength)==0)
				{
					BIImageHeader.studyField.PatBodyPart=bpVascularBody;
				}
				else if(strncmp(lpTemp,"VASCULARPER ",nVLength) == 0)
				{
					BIImageHeader.studyField.PatBodyPart = bpVascularPeripheral;
				}
				break;
			case 0x0020: //*Series_PulseSeqName	 (0018,0020) Scanning Sequence CS 1
				strncpy(BIImageHeader.seriesField.PulseSeqName, lpTemp, nVLength);
				if(*(lpTemp + nVLength - 1) == ' ')
				{
					BIImageHeader.seriesField.PulseSeqName[nVLength-1] = 0x00;
				}
				BIImageHeader.seriesField.SequenceOptions = 
					    GetSeriesOption(BIImageHeader.seriesField.PulseSeqName) ;
				break;
			case 0x0023: //*Series_Mode	 (0018,0023)MR Acquisition Type CS 1
				if (strncmp(lpTemp, "2D", 2) == 0)
				{
					BIImageHeader.seriesField.Mode = im2D ;
				}
				else if (strncmp(lpTemp, "3D", 2) == 0)
				{
					BIImageHeader.seriesField.Mode = im3D ;
				}
				break;
			case 0x0024: //*Series_PulseSeqName	 (0018,0024)Sequence Name SH 1
				strncpy(BIImageHeader.seriesField.PulseSeqName, lpTemp, nVLength);
				if(*(lpTemp+nVLength-1) == ' ')
				{
					BIImageHeader.seriesField.PulseSeqName[nVLength-1] = 0x00;
				}
				break;
			case 0x0050:	//*SliceThickness		(0018,0050) Slice Thickness DS 1
				strncpy(szTemp, lpTemp, nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				BIImageHeader.seriesField.SliceThickness = (float)atof(szTemp);
				dSliceThick = BIImageHeader.seriesField.SliceThickness ;
				break;
			case 0x0080://*Series_RepTime			(0018,0080) Repetition Time DS 1
				strncpy(szTemp, lpTemp, nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				BIImageHeader.seriesField.RepTime = strtoul(szTemp,NULL,10);
				break;
			case 0x0081://*Series_EchoTime			(0018,0081) Echo Time DS 1
				strncpy(szTemp, lpTemp, nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				BIImageHeader.seriesField.EchoTime = strtoul(szTemp,NULL,10);
				break;
			case 0x0082://Series_TI					(0018,0082) Inversion Time DS 1	
				strncpy(szTemp,lpTemp,nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				BIImageHeader.seriesField.TI = strtoul(szTemp,NULL,10);
				break;
			case 0x0083://Series_NumAvg				(0018,0083) Number of Averages DS 1
				strncpy(szTemp,lpTemp,nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				BIImageHeader.seriesField.NumAvg = (short)strtoul(szTemp,NULL,10);
				break;
			case 0x0084://Site_SF				(0018,0084) Imaging Frequency DS 1
				strncpy(szTemp,lpTemp,nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				BIImageHeader.siteField.SF = (float)strtod(szTemp,NULL);
				break;
			case 0x0085://*Series_NucType			(0018,0085) Imaged Nucleus SH 1
				if(strncmp(lpTemp,"1H",nVLength) == 0)
				{
					BIImageHeader.seriesField.NucType = ntH1;
				}
				else if(strncmp(lpTemp,"3He ",nVLength) == 0)
				{
					BIImageHeader.seriesField.NucType = ntHe3;
				}
				else if(strncmp(lpTemp,"13C ",nVLength) == 0)
				{
					BIImageHeader.seriesField.NucType = ntC13;
				}
				else if(strncmp(lpTemp,"19F ",nVLength) == 0)
				{
					BIImageHeader.seriesField.NucType = ntF19;
				}
				else if(strncmp(lpTemp,"23Na",nVLength) == 0)
				{
					BIImageHeader.seriesField.NucType = ntNa23;
				}
				else if(strncmp(lpTemp,"31P ",nVLength) == 0)
				{
					BIImageHeader.seriesField.NucType = ntP31;
				}
				else if(strncmp(lpTemp,"39K ",nVLength) == 0)
				{
					BIImageHeader.seriesField.NucType = ntK39;
				}
				else if(strncmp(lpTemp,"129Xe ",nVLength) == 0)
				{
					BIImageHeader.seriesField.NucType = ntXe129;
				}
				break;
			case 0x0086://*Image_EchoNum			(0018,0086) Echo Number(s) IS 1-n
				strncpy(szTemp,lpTemp,nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				BIImageHeader.imageField.EchoNum = (unsigned short)strtoul(szTemp,NULL,10);
				break;
			case 0x0087://*Site-MagCalibStrength		(0018,0087)Magnetic Field Strength DS 1
				strncpy(szTemp,lpTemp,nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				BIImageHeader.siteField.MagCalibStrength = (float)strtod(szTemp,NULL);
				break;
			case 0x0088://*Series_SliceGap			(0018,0088) Spacing Between Slices DS 1
				strncpy(szTemp, lpTemp ,nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				BIImageHeader.seriesField.SliceGap  = (float)strtod(szTemp,NULL);
				BIImageHeader.seriesField.SliceGap -= (float)dSliceThick ;
				break;
			case 0x0089://*Series_PhasePoint		(0018,0089) Number of Phase Encoding Steps IS 1
				strncpy(szTemp, lpTemp, nVLength) ;
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				BIImageHeader.seriesField.PhasePoints = (unsigned short)strtoul(szTemp,NULL,10);
				break;
			case 0x0091://*Series_EchoTrainLength	(0018,0091) Echo Train Length IS 1
				strncpy(szTemp, lpTemp, nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				BIImageHeader.seriesField.EchoTrainLength = (unsigned short)strtoul(szTemp,NULL,10);
				break;
			case 0x0095://*BISerialNum		(0018,0095) Pixel Bandwidth DS 1
				strncpy(szTemp, lpTemp, nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				BIImageHeader.seriesField.AcqBandwidth = (float)strtod(szTemp,NULL);
				break;
			case 0x1030://*series_Protocol		(0018,1030) Protocol Name LO 1
				strncpy(BIImageHeader.seriesField.Protocol, lpTemp, nVLength);
				if(*(lpTemp + nVLength - 1) == ' ')
				{
					BIImageHeader.seriesField.Protocol[nVLength-1] = 0x00;
				}
				break;
			case 0x1060://*Series_CardiacGating_TDT		(0018,1060) Trigger Time DS 1
				strncpy(szTemp, lpTemp, nVLength);
				break;
			case 0x1086://*Series_CardiacGating_SB		(0018,1086) Skip Beats IS 1
				strncpy(szTemp,lpTemp,nVLength);
				break;
			case 0x1088://*Series_CardiacGating_Rate		(0018,1088) Heart Rate IS 1
				strncpy(szTemp,lpTemp,nVLength);
				break;
			case 0x1090://*Series_CardiacGating_NOI		(0018,1090) Cardiac Number of Images IS 1
//				strncpy(szTemp,lpTemp,nVLength);
				break;
			case 0x1094://*Series_CardiacGating_TW		(0018,1094) Trigger Window IS 1
//				strncpy(szTemp,lpTemp,nVLength);
				break;
			case 0x1200://*LastCalib		(0018,1200) Date of Last Calibration DA 1-n	
				TimeConvert2(BIImageHeader.siteField.LastCalib,lpTemp,1);
				break;
			case 0x1201://*					(0018,1201) Time of Last Calibration TM 1-n
				TimeConvert2(BIImageHeader.siteField.LastCalib,lpTemp,2);
				break;
			case 0x1250://*RcvCoil				(0018,1250) Receiving Coil SH 1
				strncpy(BIImageHeader.seriesField.UserRcvCoilDesc, lpTemp, nVLength);
				if(*(lpTemp + nVLength - 1) == ' ')
				{
					BIImageHeader.seriesField.UserRcvCoilDesc[nVLength-1] = 0x00;
				}
				break;
			case 0x1251://*TXCoil				(0018,1251) Transmitting Coil SH 1
				strncpy(BIImageHeader.seriesField.UserTxCoilDesc, lpTemp, nVLength);
				if(*(lpTemp + nVLength - 1) == ' ')
				{
					BIImageHeader.seriesField.UserTxCoilDesc[nVLength-1] = 0x00;
				}
				break;
			case 0x1310: //*ReadPoints,PhasePoints	(0018,1310) Acquisition Matrix US 4
				sPoints  = *(unsigned short*)lpTemp;
				lpTemp += 2 ;
				if (sPoints > 0)
				{
					BIImageHeader.seriesField.ReadPoints  = sPoints;
				}
				sPoints  = *(unsigned short*)lpTemp ;
				lpTemp += 2 ;
				if (sPoints > 0)
				{
					BIImageHeader.seriesField.ReadPoints  = sPoints;
				}
				sPoints  = *(unsigned short*)lpTemp ;
				lpTemp += 2 ;
				if (sPoints > 0)
				{
					BIImageHeader.seriesField.PhasePoints  = sPoints;
				}
				sPoints  = *(unsigned short*)lpTemp ;
				lpTemp += 2 ;
				if (sPoints > 0)
				{
					BIImageHeader.seriesField.PhasePoints  = sPoints;
				}
				lpTemp -= 8 ;
				break;
			case 0x1312://*Series_OtherOptions	(0018,1314)In-plane Phase Encoding Direction CS 1
				if (strncmp(lpTemp, "ROW", 3) == 0)
				{
					BIImageHeader.seriesField.OtherOptions += 0x400 ;
				}
				break;
			case 0x1314://*Series_FlipAngle		(0018,1314) Flip Angle DS 1
				strncpy(szTemp, lpTemp, nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				BIImageHeader.seriesField.FlipAngle = (float)atof(szTemp);
				break;
			case 0x1316://*Series_SARAvg			(0018,1316) SAR DS 1
				strncpy(szTemp, lpTemp, nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				BIImageHeader.seriesField.SARAvg = (int)strtod(szTemp,NULL);
				break;
			case 0x5100://*Study_PatPosition	(0018,5100) Patient Position CS 1
				if(strncmp(lpTemp,"HFS ",nVLength) == 0)
				{
					BIImageHeader.studyField.PatEntry    = beHeadFirst;
					BIImageHeader.studyField.PatPosition = bpSupine;
				}
				else if(strncmp(lpTemp,"HFP ",nVLength) == 0)
				{
					BIImageHeader.studyField.PatEntry    = beHeadFirst;
					BIImageHeader.studyField.PatPosition = bpProne;
				}
				else if(strncmp(lpTemp,"HFDR",nVLength) == 0)
				{
					BIImageHeader.studyField.PatEntry    = beHeadFirst;
					BIImageHeader.studyField.PatPosition = bpDecubitusRight;
				}
				else if(strncmp(lpTemp,"HFDL",nVLength) == 0)
				{
					BIImageHeader.studyField.PatEntry    = beHeadFirst;
					BIImageHeader.studyField.PatPosition = bpDecubitusLeft;
				}
				else if(strncmp(lpTemp,"FFS ",nVLength) == 0)
				{
					BIImageHeader.studyField.PatEntry    = beFootFirst;
					BIImageHeader.studyField.PatPosition = bpSupine;
				}
				else if(strncmp(lpTemp,"FFP ",nVLength) == 0)
				{
					BIImageHeader.studyField.PatEntry    = beFootFirst;
					BIImageHeader.studyField.PatPosition = bpProne;
				}
				else if(strncmp(lpTemp,"FFDR",nVLength) == 0)
				{
					BIImageHeader.studyField.PatEntry    = beFootFirst;
					BIImageHeader.studyField.PatPosition = bpDecubitusRight;
				}
				else if(strncmp(lpTemp,"FFDL",nVLength) == 0)
				{
					BIImageHeader.studyField.PatEntry    = beFootFirst;
					BIImageHeader.studyField.PatPosition = bpDecubitusLeft;
				}
				break;
			default:
				break;
			}//switch iElementNum
			if (nVLength > 0)
			{
				lpTemp += nVLength;
			}
			break;
			}
		case 0x0019:
			{
				if (nVLength > 0)
				{
					lpTemp += nVLength;
				}
			break;
			}
		case 0x0020:
			{
			switch(iElementNum)
			{
			case 0x0010://*Study_Id				(0020,0010) Study ID SH 1
				strncpy(BIImageHeader.studyField.Id, lpTemp, nVLength);
				if(*(lpTemp+nVLength-1) == ' ')
				{
					BIImageHeader.studyField.Id[nVLength-1]=0x00;
				}
				break;
			case 0x0011://*Series_Id			(0020,0011) Series Number IS 1
				strncpy(szTemp, lpTemp, nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				BIImageHeader.seriesField.Id = (BYTE)strtoul(szTemp,NULL,10);
				break;
			case 0x0013://*Image_Id				(0020,0013) Instance Number IS 1
				strncpy(szTemp, lpTemp, nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				BIImageHeader.imageField.Id = (unsigned short)strtoul(szTemp,NULL,10);
				break;
			case 0x0032: //*(0020,0032) Image Position (Patient) DS 3
				GetDSVM(lpTemp, nVLength, dVal, nVM);
				if (nVM != 3)
				{
					return -6 ;
				}
				dOffSet[0] = dVal[0] ;
				dOffSet[1] = dVal[1] ;
				dOffSet[2] = dVal[2] ;
				BIImageHeader.scanField.SliceOffset = 0 ;
				BIImageHeader.scanField.ReadOffset  = 0 ;
				BIImageHeader.scanField.PhaseOffset = 0 ;
				break;
			case 0x0037://Image Orientation (Patient) DS 6
				GetDSVM(lpTemp, nVLength, dVal, nVM);
				if (nVM != 6)
				{
					return -7 ;
				}
				else
				{
					dImgDirec[0] = dVal[0] ;
					dImgDirec[1] = dVal[1] ;
					dImgDirec[2] = dVal[2] ;
					dImgDirec[3] = dVal[3] ;
					dImgDirec[4] = dVal[4] ;
					dImgDirec[5] = dVal[5] ;
				}
				break;
			case 0x1002://*ResolutionZ		(0020,1002)Images in Acquisition IS 1
				strncpy(szTemp, lpTemp, nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				BIImageHeader.seriesField.ResolutionZ = (unsigned short)strtoul(szTemp,NULL,10);
				break;	
			default:
				break;
			}//switch iElementNum
			if (nVLength > 0)
			{
				lpTemp += nVLength;
			}
			break;
			}
		case 0x0021:
			{
				if (nVLength > 0)
				{
					lpTemp += nVLength;
				}
			break;
			}
		case 0x0028:
			{
			switch(iElementNum)
			{
			case 0x0010: //(0028,0010)Rows  US 1
				BIImageHeader.seriesField.ResolutionX = *(unsigned short*)lpTemp ;
				break;
			case 0x0011: //(0028,0010)Columns US 1
				BIImageHeader.seriesField.ResolutionY = *(unsigned short*)lpTemp ;
				break;
			case 0x0030:  //(0028,0010)Pixel Spacing DS 2
				GetDSVM(lpTemp, nVLength, dVal, nVM);
				if (nVM == 2)
				{
					BIImageHeader.seriesField.FovX = 
						(unsigned short)((double)BIImageHeader.seriesField.ResolutionX*dVal[0]) ;
					BIImageHeader.seriesField.FovY = 
						(unsigned short)((double)BIImageHeader.seriesField.ResolutionY*dVal[1]) ;
					CalcBIImgDirec(dImgDirec, dOffSet ,
								   BIImageHeader.studyField.PatPosition, 
								   BIImageHeader.studyField.PatEntry, 
								   BIImageHeader.seriesField.FovX,
								   BIImageHeader.seriesField.FovY,
								   dXBImgDirec) ;
					BIImageHeader.scanField.FreqDirX  = (float)dXBImgDirec[0] ;
					BIImageHeader.scanField.FreqDirY  = (float)dXBImgDirec[1] ;
					BIImageHeader.scanField.FreqDirZ  = (float)dXBImgDirec[2] ;
					BIImageHeader.scanField.PhaseDirX = (float)dXBImgDirec[3] ;
					BIImageHeader.scanField.PhaseDirY = (float)dXBImgDirec[4] ;
					BIImageHeader.scanField.PhaseDirZ = (float)dXBImgDirec[5] ;
					BIImageHeader.scanField.SliceDirX = (float)dXBImgDirec[6] ;
					BIImageHeader.scanField.SliceDirY = (float)dXBImgDirec[7] ;
					BIImageHeader.scanField.SliceDirZ = (float)dXBImgDirec[8] ;
//					BIImageHeader.scanField.ReadOffset  = (float)dXBImgDirec[9] ;
//					BIImageHeader.scanField.PhaseOffset = (float)dXBImgDirec[10] ;
//					BIImageHeader.scanField.SliceOffset = (float)dXBImgDirec[11] ;

					BIImageHeader.imageField.fFreqDirX     = (float)dXBImgDirec[0] ;
					BIImageHeader.imageField.fFreqDirY     = (float)dXBImgDirec[1] ;
					BIImageHeader.imageField.fFreqDirZ     = (float)dXBImgDirec[2] ;
					BIImageHeader.imageField.fPhaseDirX    = (float)dXBImgDirec[3] ;
					BIImageHeader.imageField.fPhaseDirY    = (float)dXBImgDirec[4] ;
					BIImageHeader.imageField.fPhaseDirZ    = (float)dXBImgDirec[5] ;
					BIImageHeader.imageField.fSliceDirX    = (float)dXBImgDirec[6] ;
					BIImageHeader.imageField.fSliceDirY    = (float)dXBImgDirec[7] ;
					BIImageHeader.imageField.fSliceDirZ    = (float)dXBImgDirec[8] ;
					BIImageHeader.imageField.fCenterPointX = (float)dXBImgDirec[9] ;
					BIImageHeader.imageField.fCenterPointY = (float)dXBImgDirec[10] ;
					BIImageHeader.imageField.fCenterPointZ = (float)dXBImgDirec[11] ;
				}
				else
				{
					return -8 ;
				}
				break;
			case 0x1050: //(0028,1050) Window Center DS 1
				strncpy(szTemp, lpTemp, nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				BIImageHeader.imageField.WindowLevel = (short)strtoul(szTemp,NULL,10);
				break;
			case 0x1051: //(0028,1051) Window Width DS 1
				strncpy(szTemp, lpTemp, nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				BIImageHeader.imageField.WindowWidth = (short)strtoul(szTemp,NULL,10);
				break;
			default:
				break;
			}
			if (nVLength > 0)
			{
				lpTemp += nVLength;
			}
			break;
			}
		case 0x0029:
			{
				if (nVLength > 0)
				{
					lpTemp += nVLength;
				}
			break;
			}
		case 0x0043:
			{
				if (nVLength > 0)
				{
					lpTemp += nVLength;
				}
			break;
			}
		case 0x7FE0:
			{
			switch(iElementNum)
			{
			case 0x0010:
				{
					int nImgSize = 
						2*BIImageHeader.seriesField.ResolutionX*BIImageHeader.seriesField.ResolutionY ;
					BIImageHeader.headInfo.DataMatrixSize = nImgSize ;
					nImageSize   = BIImageHeader.seriesField.ResolutionX*BIImageHeader.seriesField.ResolutionY ;
					
					pBIImageData = new WORD[nImageSize];
					if (pBIImageData != NULL)
					{
						memcpy(pBIImageData, lpTemp, nImageSize*2);
					
						lpTemp += nImageSize*2 ;
						return 0 ;
					}
					else
					{
						return -5 ;
					}
				}
				break;
			default:
				break;
			}
			if (nVLength > 0)
			{
				lpTemp += nVLength;
			}
			break;
			}
		default:
			if (nVLength > 0)
			{
				lpTemp += nVLength;
			}
			break;
		}//swtch groupnum

	}//while end

	return true;
}

int CDICOM2XINAO::ConvertCTDcm2XB(CT_ImageHeader &ct_imageHeader,char* lpDcm,int nDcmSize, WORD* &pBIImageData)
{
	char *lpTemp = (char*)lpDcm ; 
	char szVR[3]=" " ;
	char szTemp[64] ;
	int  nImageSize = 1 ;
	int  nGroupLen  = 0 ;
	short nRescaleIntercept=0;
	short nRescaleSlope=1;

//	unsigned short sPoints ;
	int            count = 0 ;
	int            i     = 0 ;
	int            nVLength;
	double         dVal[20] ;
	double         dImgDirec[6] ;
	double         dXBImgDirec[12] ;
	double         dOffSet[3] ;
	double         dSliceThick = 5.0;
	double         dSliceLocation = 0.0;

	int            nVM;
	short iGroupNum,iElementNum;

	lpTemp += 132;

	memset(&(ct_imageHeader.headInfo), 0 ,sizeof(ct_imageHeader) );
	// Initialize File Header
	//headInfo
    ct_imageHeader.headInfo.BITitle[0] = 'D';
    ct_imageHeader.headInfo.BITitle[1] = 'C';
    ct_imageHeader.headInfo.BITitle[2] = 'M';
    ct_imageHeader.headInfo.BITitle[3] = 'C';
    ct_imageHeader.headInfo.BITitle[4] = 'T';
    ct_imageHeader.headInfo.Version = int (1) << 16 + 0;
 

	while((lpTemp - lpDcm) < nDcmSize)
	{
		iGroupNum   = *((unsigned short*)lpTemp);
		lpTemp += 2;
		iElementNum = *((unsigned short*)lpTemp);
		lpTemp += 2;
		if (iGroupNum == 2)
		{
			szVR[0] = *lpTemp;
			lpTemp++;
			szVR[1] = *lpTemp;
			lpTemp++;
			if(strcmp(szVR,"OB")==0||strcmp(szVR,"OW")==0||
			   strcmp(szVR,"SQ")==0||strcmp(szVR,"UN")==0)
			{
				lpTemp  += 2;
				nVLength = *((int*)lpTemp);
				lpTemp  += 4;
			}
			else
			{
				nVLength = *((short*)lpTemp);
				lpTemp  += 2;
			}
		}
		else
		{
			nVLength = *((int*)lpTemp);
			lpTemp  += 4;
		}

		switch(iGroupNum)
		{
		case 0x0002:
			{
			switch(iElementNum)
			{
				case 0x0002: //Media Storage SOP Class UID
					if (strncmp(lpTemp, "1.2.840.10008.5.1.4.1.1.2", nVLength) != 0)
					{
						return -1 ;    //Invalidate SOP Class UID
					}
					break;
				case 0x0010: //Transfer Syntax UID
					if (strncmp(lpTemp, "1.2.840.10008.1.2", nVLength) != 0)
					{
						return -2 ;    //Not Support the Transfer Syntax
					}
					break;
				case 0x0016:
					
					break;
				default:
					break;
			}
			lpTemp += nVLength;
			break;
			}
		case 0x0008:
			{
			switch(iElementNum)
			{
			case 0x0008://*Image_Type			(0008,0008) Image Type CS 1-n
				/*if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\AXIAL", nVLength) == 0)
				{
					ct_imageHeader.imageField.Type = itMPR;
				}
				else if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\LOCALIZER", nVLength) == 0)
				{
					ct_imageHeader.imageField.Type = itProjection;
				}*/
				break;
			case 0x0016:  //SOP Class UID
				if (strncmp(lpTemp, "1.2.840.10008.5.1.4.1.1.2", nVLength) != 0)
				{
					return -4 ;    //Is not a CT image .
				}
				break;
			case 0x0020://*Study_Time			(0008,0020) Study Date DA 1	
				TimeConvert2(ct_imageHeader.studyField.Time, lpTemp, 1);
				break;
			case 0x0021://*Series_DateTime			(0008,0021)Series Date DA 1	
				TimeConvert2(ct_imageHeader.seriesField.DateTime, lpTemp, 1);
				break;
			case 0x0022://*Image_DateTime			(0008,0022)Acquisition Date DA 1	
				TimeConvert2(ct_imageHeader.imageField.DateTime, lpTemp, 1);
				break;
			case 0x0030://*Study_Time			(0008,0030) Study Time TM 1	
				TimeConvert2(ct_imageHeader.studyField.Time, lpTemp, 2);
				break;
			case 0x0031://*Series_DateTime			(0008,0031) Series Time TM 1	
				TimeConvert2(ct_imageHeader.seriesField.DateTime, lpTemp, 2);
				break;
			case 0x0032://*image_DateTime			(0008,0032) Study Time TM 1	
				TimeConvert2(ct_imageHeader.imageField.DateTime, lpTemp, 2);
				break;
			case 0x0070:	//*		(0008,0070) Manufacturer LO 1
				strncpy(ct_imageHeader.siteField.Manufacturer, lpTemp, nVLength);
				if(*(lpTemp + nVLength - 1) == ' ')
				{
					ct_imageHeader.siteField.Manufacturer[nVLength-1] = 0x00;
				}
			case 0x0080://*Site_InstName		(0008,0080) Institution Name LO 1
				strncpy(ct_imageHeader.siteField.InstName, lpTemp, nVLength);
				if(*(lpTemp + nVLength - 1) == ' ')
				{
					ct_imageHeader.siteField.InstName[nVLength-1] = 0x00;
				}
				break;
			case 0x0090://*Study_RefPhy			(0008,0090) Referring Physician'sName PN 1
				//NameConvert2(ct_imageHeader.studyField.RefPhy,lpTemp,nVLength);
				break;
			case 0x1010://*Site_StationName		(0008,1010) Station Name SH 1
				strncpy(ct_imageHeader.siteField.StationName,lpTemp,nVLength);
				if(*(lpTemp + nVLength - 1) == ' ')
				{
					ct_imageHeader.siteField.StationName[nVLength-1] = 0x00;
				}
				break;
			case 0x1030://*Study_Desc		(0008,1030) Study Description LO 1
				strncpy(ct_imageHeader.studyField.Desc, lpTemp, nVLength);
				if(*(lpTemp + nVLength - 1) == ' ')
				{
					ct_imageHeader.studyField.Desc[nVLength-1] = 0x00;
				}
				break;
			case 0x103E://*Series_Desc			(0008,103E) Series Description LO 1
				strncpy(ct_imageHeader.seriesField.Desc, lpTemp, nVLength);
				if(*(lpTemp + nVLength - 1) == ' ')
				{
					ct_imageHeader.seriesField.Desc[nVLength-1] = 0x00;
				}
				break;
			case 0x1040://*Site_InstDeptName		(0008,1040) Institutional Department Name LO 1
				strncpy(ct_imageHeader.siteField.InstDeptName, lpTemp, nVLength);
				if(*(lpTemp + nVLength - 1) == ' ')
				{
					ct_imageHeader.siteField.InstDeptName[nVLength-1] = 0x00;
				}
				break;
			case 0x1090://*Site_BISerialNum		(0008,1090) Manufacturer's Model Name LO 1
				strncpy(ct_imageHeader.siteField.BISerialNum, lpTemp, nVLength);
				if(*(lpTemp + nVLength - 1) == ' ')
				{
					ct_imageHeader.siteField.BISerialNum[nVLength-1] = 0x00;
				}
				break;
			case 0x1110: //*Site_BISerialNum		(0008,1090) Manufacturer's Model Name LO 1
				break;
			case 0x1111: //*Site_BISerialNum		(0008,1090) Manufacturer's Model Name LO 1
				break;
			default:
				break;
			}
			if ( nVLength > 0 )
			{
				lpTemp += nVLength;
			}
			break;
			}
		case 0x0010:
			{
			switch(iElementNum)
			{
			case 0x0010://*Patient_Name			(0010,0010) Patient's Name PN 1
				NameConvert2(ct_imageHeader.patientField.Name, lpTemp, nVLength);
				break;
			case 0x0020://*Patient_Id			(0010,0020) Patient ID LO 1
				strncpy(ct_imageHeader.patientField.Id, lpTemp, nVLength);
				if(*(lpTemp + nVLength - 1) == ' ')
				{
					ct_imageHeader.patientField.Id[nVLength-1]=0x00;
				}
				break;
			case 0x0030://*Patient_BirthDate	(0010,0030) Patient's Birth Date DA 1
				TimeConvert2(ct_imageHeader.patientField.BirthDate, lpTemp, 1);
				break;
			case 0x0032://*Patient_BirthDate	(0010,0032) Patient's Birth Time TM 1
				TimeConvert2(ct_imageHeader.patientField.BirthDate, lpTemp, 2);
				break;
			case 0x0040://*Patient_Sex			(0010,0040) Patient's Sex CS 1
				if(*lpTemp == 'O')
				{
					ct_imageHeader.patientField.Sex = 'U';
				}
				else
				{
					ct_imageHeader.patientField.Sex = *lpTemp;
				}
				break;
			case 0x1010://*Patient_Age			(0010,1010) Patient's Age AS 1
				ct_imageHeader.patientField.Age = AgeConvert2(lpTemp);
				break;
			case 0x1030://*Patient_Weight		(0010,1030) Patient's Weight DS 1
				strncpy(szTemp, lpTemp, nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				ct_imageHeader.patientField.Weight = atoi(szTemp);
				break;
			case 0x21B0://*Patient_Desc			(0010,21B0) Additional Patient History LT 1
				strncpy(ct_imageHeader.patientField.Desc,lpTemp,nVLength);
				if(*(lpTemp + nVLength - 1) == ' ')
				{
					ct_imageHeader.patientField.Desc[nVLength-1] = 0x00;
				}
				break;
			default:
				break;
			}//switch iElementNum
			if (nVLength > 0)
			{
				lpTemp += nVLength;
			}
			break;
			}
		case 0x0018:
			{
			switch(iElementNum)
			{
			case 0x0015: //*PatBodyPart		(0018,0015) Body Part Examined CS 1
				/*if(strncmp(lpTemp, "SKULL ", nVLength) == 0)
				{
					ct_imageHeader.studyField.PatBodyPart = bpHead;
				}
				else if(strncmp(lpTemp, "NECK", nVLength) == 0)
				{
					ct_imageHeader.studyField.PatBodyPart = bpNeck;
				}
				else if(strncmp(lpTemp, "SHOULDER", nVLength) == 0)
				{
					ct_imageHeader.studyField.PatBodyPart = bpShoulder;
				}
				else if(strncmp(lpTemp, "CSPINE", nVLength) == 0)
				{
					ct_imageHeader.studyField.PatBodyPart = bpCervicalSpine;
				}
				else if(strncmp(lpTemp,"TSPINE",nVLength) == 0)
				{
					ct_imageHeader.studyField.PatBodyPart = bpThoracicSpine;
				}
				else if(strncmp(lpTemp,"LSPINE",nVLength) == 0)
				{
					ct_imageHeader.studyField.PatBodyPart = bpLumberSpine;
				}
				else if(strncmp(lpTemp,"CHEST ",nVLength) == 0)
				{
					ct_imageHeader.studyField.PatBodyPart = bpChest;
				}
				else if(strncmp(lpTemp,"BREAST",nVLength) == 0)
				{
					ct_imageHeader.studyField.PatBodyPart = bpBreast;
				}
				else if(strncmp(lpTemp,"ABODOMEN",nVLength) == 0)
				{
					ct_imageHeader.studyField.PatBodyPart = bpAbdomen;
				}
				else if(strncmp(lpTemp,"PELVIS",nVLength) == 0)
				{
					ct_imageHeader.studyField.PatBodyPart = bpPelvis;
				}
				else if(strncmp(lpTemp,"HIP ",nVLength) == 0)
				{
					ct_imageHeader.studyField.PatBodyPart = bpHip;
				}
				else if(strncmp(lpTemp,"ELBOW ",nVLength) == 0)
				{
					ct_imageHeader.studyField.PatBodyPart = bpElbow;
				}
				else if(strncmp(lpTemp,"WRIST ",nVLength) == 0)
					ct_imageHeader.studyField.PatBodyPart = bpWrist;
				else if(strncmp(lpTemp,"ARM ",nVLength) == 0)
				{
					ct_imageHeader.studyField.PatBodyPart = bpArm;
				}
				else if(strncmp(lpTemp,"HAND",nVLength) == 0)
				{
					ct_imageHeader.studyField.PatBodyPart = bpHand;
				}
				else if(strncmp(lpTemp,"KNEE",nVLength) == 0)
				{
					ct_imageHeader.studyField.PatBodyPart = bpKnee;
				}
				else if(strncmp(lpTemp,"LEG ",nVLength) == 0)
				{
					ct_imageHeader.studyField.PatBodyPart = bpLeg;
				}
				else if(strncmp(lpTemp,"ANKLE ",nVLength)==0)
				{
					ct_imageHeader.studyField.PatBodyPart = bpAnkle;
				}
				else if(strncmp(lpTemp,"FOOT",nVLength) == 0)
				{
					ct_imageHeader.studyField.PatBodyPart = bpFoot;
				}
				else if(strncmp(lpTemp,"HEART ",nVLength) == 0)
				{
					ct_imageHeader.studyField.PatBodyPart = bpHeart;
				}
				else if(strncmp(lpTemp,"VASCULARHEAD",nVLength) == 0)
				{
					ct_imageHeader.studyField.PatBodyPart = bpVascularHead;
				}
				else if(strncmp(lpTemp,"VASCULARNECK",nVLength) == 0)
				{
					ct_imageHeader.studyField.PatBodyPart = bpVascularNeck;
				}
				else if(strncmp(lpTemp,"VASCULARBODY",nVLength)==0)
				{
					ct_imageHeader.studyField.PatBodyPart=bpVascularBody;
				}
				else if(strncmp(lpTemp,"VASCULARPER ",nVLength) == 0)
				{
					ct_imageHeader.studyField.PatBodyPart = bpVascularPeripheral;
				}*/
				break;
			case 0x0020: //*Series_PulseSeqName	 (0018,0020) Scanning Sequence CS 1
				/*
				strncpy(ct_imageHeader.seriesField.PulseSeqName, lpTemp, nVLength);
				if(*(lpTemp + nVLength - 1) == ' ')
				{
					ct_imageHeader.seriesField.PulseSeqName[nVLength-1] = 0x00;
				}
				ct_imageHeader.seriesField.SequenceOptions = 
					    GetSeriesOption(ct_imageHeader.seriesField.PulseSeqName) ;
				*/
				break;
			case 0x0023: //*Series_Mode	 (0018,0023)MR Acquisition Type CS 1
				/*if (strncmp(lpTemp, "2D", 2) == 0)
				{
					ct_imageHeader.seriesField.Mode = im2D ;
				}
				else if (strncmp(lpTemp, "3D", 2) == 0)
				{
					ct_imageHeader.seriesField.Mode = im3D ;
				}*/
				break;
			case 0x0024: //*Series_PulseSeqName	 (0018,0024)Sequence Name SH 1
				/*
				strncpy(ct_imageHeader.seriesField.PulseSeqName, lpTemp, nVLength);
				if(*(lpTemp+nVLength-1) == ' ')
				{
					BIImageHeader.seriesField.PulseSeqName[nVLength-1] = 0x00;
				}*/
				break;
			case 0x0050:	//*SliceThickness		(0018,0050) Slice Thickness DS 1
				strncpy(szTemp, lpTemp, nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				ct_imageHeader.seriesField.SliceThickness = (float)atof(szTemp);
				dSliceThick = ct_imageHeader.seriesField.SliceThickness ;
				break;
			case 0x0060://*seriesField.KVP	   (0018,0060) KVP DS 1
				strncpy(szTemp, lpTemp, nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				ct_imageHeader.seriesField.KVP = (unsigned short)atoi(szTemp);
				break ;
			case 0x0080://*Series_RepTime			(0018,0080) Repetition Time DS 1
/*				strncpy(szTemp, lpTemp, nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				ct_imageHeader.seriesField.RepTime = strtoul(szTemp,NULL,10);*/
				break;
			case 0x0081://*Series_EchoTime			(0018,0081) Echo Time DS 1
/*				strncpy(szTemp, lpTemp, nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				BIImageHeader.seriesField.EchoTime = strtoul(szTemp,NULL,10);*/
				break;
			case 0x0082://Series_TI					(0018,0082) Inversion Time DS 1	
/*				strncpy(szTemp,lpTemp,nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				ct_imageHeader.seriesField.TI = strtoul(szTemp,NULL,10);*/
				break;
			case 0x0083://Series_NumAvg				(0018,0083) Number of Averages DS 1
/*				strncpy(szTemp,lpTemp,nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				BIImageHeader.seriesField.NumAvg = (short)strtoul(szTemp,NULL,10);*/
				break;
			case 0x0084://Site_SF				(0018,0084) Imaging Frequency DS 1
/*				strncpy(szTemp,lpTemp,nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				BIImageHeader.siteField.SF = (float)strtod(szTemp,NULL);*/
				break;
			case 0x0085://*Series_NucType			(0018,0085) Imaged Nucleus SH 1
/*				if(strncmp(lpTemp,"1H",nVLength) == 0)
				{
					BIImageHeader.seriesField.NucType = ntH1;
				}
				else if(strncmp(lpTemp,"3He ",nVLength) == 0)
				{
					BIImageHeader.seriesField.NucType = ntHe3;
				}
				else if(strncmp(lpTemp,"13C ",nVLength) == 0)
				{
					BIImageHeader.seriesField.NucType = ntC13;
				}
				else if(strncmp(lpTemp,"19F ",nVLength) == 0)
				{
					BIImageHeader.seriesField.NucType = ntF19;
				}
				else if(strncmp(lpTemp,"23Na",nVLength) == 0)
				{
					BIImageHeader.seriesField.NucType = ntNa23;
				}
				else if(strncmp(lpTemp,"31P ",nVLength) == 0)
				{
					BIImageHeader.seriesField.NucType = ntP31;
				}
				else if(strncmp(lpTemp,"39K ",nVLength) == 0)
				{
					BIImageHeader.seriesField.NucType = ntK39;
				}
				else if(strncmp(lpTemp,"129Xe ",nVLength) == 0)
				{
					BIImageHeader.seriesField.NucType = ntXe129;
				}*/
				break;
			case 0x0086://*Image_EchoNum			(0018,0086) Echo Number(s) IS 1-n
/*				strncpy(szTemp,lpTemp,nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				BIImageHeader.imageField.EchoNum = (unsigned short)strtoul(szTemp,NULL,10);
				break;*/
			case 0x0087://*Site-MagCalibStrength		(0018,0087)Magnetic Field Strength DS 1
/*				strncpy(szTemp,lpTemp,nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				BIImageHeader.siteField.MagCalibStrength = (float)strtod(szTemp,NULL);
				break;*/
			case 0x0088://*Series_SliceGap			(0018,0088) Spacing Between Slices DS 1
				strncpy(szTemp, lpTemp ,nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				ct_imageHeader.seriesField.SliceGap  = (float)strtod(szTemp,NULL);
				ct_imageHeader.seriesField.SliceGap += (float)dSliceThick ;
				break;
			case 0x0089://*Series_PhasePoint		(0018,0089) Number of Phase Encoding Steps IS 1
/*				strncpy(szTemp, lpTemp, nVLength) ;
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				BIImageHeader.seriesField.PhasePoints = (unsigned short)strtoul(szTemp,NULL,10);*/
				break;
			case 0x0090://*Data Collection Diameter		(0018,0090) Data Collection Diameter DS 1
				strncpy(szTemp, lpTemp, nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				ct_imageHeader.seriesField.CollectDiameter = (unsigned short)atoi(szTemp);
				break;
			case 0x0091://*Series_EchoTrainLength	(0018,0091) Echo Train Length IS 1
/*				strncpy(szTemp, lpTemp, nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				BIImageHeader.seriesField.EchoTrainLength = (unsigned short)strtoul(szTemp,NULL,10);*/
				break;
			case 0x0095://*BISerialNum		(0018,0095) Pixel Bandwidth DS 1
/*				strncpy(szTemp, lpTemp, nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				BIImageHeader.seriesField.AcqBandwidth = (float)strtod(szTemp,NULL);*/
				break;
			case 0x1020://*Software Version		(0018,1020) Software Version(s) LO 1
				strncpy(ct_imageHeader.siteField.SoftVer, lpTemp, nVLength);
				if(*(lpTemp + nVLength - 1) == ' ')
				{
					ct_imageHeader.seriesField.Protocol[nVLength-1] = 0x00;
				}
				break;
			case 0x1030://*series_Protocol		(0018,1030) Protocol Name LO 1
				strncpy(ct_imageHeader.seriesField.Protocol, lpTemp, nVLength);
				if(*(lpTemp + nVLength - 1) == ' ')
				{
					ct_imageHeader.seriesField.Protocol[nVLength-1] = 0x00;
				}
				break;
			case 0x1060://*Series_CardiacGating_TDT		(0018,1060) Trigger Time DS 1
//				strncpy(szTemp, lpTemp, nVLength);
				break;
			case 0x1086://*Series_CardiacGating_SB		(0018,1086) Skip Beats IS 1
//				strncpy(szTemp,lpTemp,nVLength);
				break;
			case 0x1088://*Series_CardiacGating_Rate		(0018,1088) Heart Rate IS 1
//				strncpy(szTemp,lpTemp,nVLength);
				break;
			case 0x1090://*Series_CardiacGating_NOI		(0018,1090) Cardiac Number of Images IS 1
//				strncpy(szTemp,lpTemp,nVLength);
				break;
			case 0x1094://*Series_CardiacGating_TW		(0018,1094) Trigger Window IS 1
//				strncpy(szTemp,lpTemp,nVLength);
				break;
			case 0x1100://*Reconstruction Diameter		(0018,1100) Reconstruction Diameter DS 1
				strncpy(szTemp, lpTemp, nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				ct_imageHeader.seriesField.DataDiameter = (unsigned short)atoi(szTemp);
				break;
			case 0x1120:   //*Gantry/Detector Tilt		(0018,1120) Gantry/Detector Tilt DS 1
				strncpy(szTemp, lpTemp, nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				ct_imageHeader.seriesField.DetectorTilt = (unsigned short)atoi(szTemp);
				break;
			case 0x1130:   //*Table Height		(0018,1130) Table Height DS 1
				strncpy(szTemp, lpTemp, nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				ct_imageHeader.seriesField.TableHeight = (short)atoi(szTemp);
				break;
			case 0x1140:   //*Rotation Direction		(0018,1140) Rotation Direction CS 1	
				if (strncmp(lpTemp, "CW", 2) == 0)
				{
					ct_imageHeader.seriesField.RotateDir = 1 ;
				}
				else if (strncmp(lpTemp, "CC", 2) == 0)
				{
					ct_imageHeader.seriesField.RotateDir = 2 ;
				}
				else
				{
					ct_imageHeader.seriesField.RotateDir = 0 ;
				}
				break;
			case 0x1151:   //*X-Ray Tube Current		(0018,1151) X-Ray Tube Current IS 1	
				strncpy(szTemp,lpTemp,nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				ct_imageHeader.seriesField.TubeCurrent = (unsigned short)atoi(szTemp);
				break;
			case 0x1152:   //*Exposure in uAs		      (0018,1152) Exposure IS 1	
				strncpy(szTemp,lpTemp,nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				ct_imageHeader.seriesField.Exposure = (unsigned short)atoi(szTemp);
				break;
			case 0x1160:   //*Filter Type		  (0018,1160) Filter Type SH 1	
				break;
			case 0x1200:   //*LastCalib		      (0018,1200) Date of Last Calibration DA 1-n	
				TimeConvert2(ct_imageHeader.siteField.LastCalib,lpTemp,1);
				break;
			case 0x1201:   //* (0018,1201) Time of Last Calibration TM 1-n
				TimeConvert2(ct_imageHeader.siteField.LastCalib,lpTemp,2);
				break;
			case 0x1210:   //*Convolution Kernel     (0018,1210) Convolution Kernel SH 1
				break;
			case 0x1250://*RcvCoil				(0018,1250) Receiving Coil SH 1
/*				strncpy(BIImageHeader.seriesField.UserRcvCoilDesc, lpTemp, nVLength);
				if(*(lpTemp + nVLength - 1) == ' ')
				{
					BIImageHeader.seriesField.UserRcvCoilDesc[nVLength-1] = 0x00;
				}*/
				break;
			case 0x1251://*TXCoil				(0018,1251) Transmitting Coil SH 1
/*				strncpy(BIImageHeader.seriesField.UserTxCoilDesc, lpTemp, nVLength);
				if(*(lpTemp + nVLength - 1) == ' ')
				{
					BIImageHeader.seriesField.UserTxCoilDesc[nVLength-1] = 0x00;
				}*/
				break;
			case 0x1310: //*ReadPoints,PhasePoints	(0018,1310) Acquisition Matrix US 4
/*				sPoints  = *(unsigned short*)lpTemp;
				lpTemp += 2 ;
				if (sPoints > 0)
				{
					ct_imageHeader.seriesField.ReadPoints  = sPoints;
				}
				sPoints  = *(unsigned short*)lpTemp ;
				lpTemp += 2 ;
				if (sPoints > 0)
				{
					ct_imageHeader.seriesField.ReadPoints  = sPoints;
				}
				sPoints  = *(unsigned short*)lpTemp ;
				lpTemp += 2 ;
				if (sPoints > 0)
				{
					ct_imageHeader.seriesField.PhasePoints  = sPoints;
				}
				sPoints  = *(unsigned short*)lpTemp ;
				lpTemp += 2 ;
				if (sPoints > 0)
				{
					ct_imageHeader.seriesField.PhasePoints  = sPoints;
				}
				lpTemp -= 8 ;*/
				break;
			case 0x1312://*Series_OtherOptions	(0018,1314)In-plane Phase Encoding Direction CS 1
/*				if (strncmp(lpTemp, "ROW", 3) == 0)
				{
					ct_imageHeader.seriesField.OtherOptions += 0x400 ;
				}*/
				break;
			case 0x1314://*Series_FlipAngle		(0018,1314) Flip Angle DS 1
/*				strncpy(szTemp, lpTemp, nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				BIImageHeader.seriesField.FlipAngle = (float)atof(szTemp);*/
				break;
			case 0x1316://*Series_SARAvg			(0018,1316) SAR DS 1
/*				strncpy(szTemp, lpTemp, nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				BIImageHeader.seriesField.SARAvg = (int)strtod(szTemp,NULL);*/
				break;
			case 0x5100://*Study_PatPosition	(0018,5100) Patient Position CS 1
				if(strncmp(lpTemp,"HFS ",nVLength) == 0)
				{
					ct_imageHeader.studyField.PatEntry    = beHeadFirst;
					ct_imageHeader.studyField.PatPosition = bpSupine;
				}
				else if(strncmp(lpTemp,"HFP ",nVLength) == 0)
				{
					ct_imageHeader.studyField.PatEntry    = beHeadFirst;
					ct_imageHeader.studyField.PatPosition = bpProne;
				}
				else if(strncmp(lpTemp,"HFDR",nVLength) == 0)
				{
					ct_imageHeader.studyField.PatEntry    = beHeadFirst;
					ct_imageHeader.studyField.PatPosition = bpDecubitusRight;
				}
				else if(strncmp(lpTemp,"HFDL",nVLength) == 0)
				{
					ct_imageHeader.studyField.PatEntry    = beHeadFirst;
					ct_imageHeader.studyField.PatPosition = bpDecubitusLeft;
				}
				else if(strncmp(lpTemp,"FFS ",nVLength) == 0)
				{
					ct_imageHeader.studyField.PatEntry    = beFootFirst;
					ct_imageHeader.studyField.PatPosition = bpSupine;
				}
				else if(strncmp(lpTemp,"FFP ",nVLength) == 0)
				{
					ct_imageHeader.studyField.PatEntry    = beFootFirst;
					ct_imageHeader.studyField.PatPosition = bpProne;
				}
				else if(strncmp(lpTemp,"FFDR",nVLength) == 0)
				{
					ct_imageHeader.studyField.PatEntry    = beFootFirst;
					ct_imageHeader.studyField.PatPosition = bpDecubitusRight;
				}
				else if(strncmp(lpTemp,"FFDL",nVLength) == 0)
				{
					ct_imageHeader.studyField.PatEntry    = beFootFirst;
					ct_imageHeader.studyField.PatPosition = bpDecubitusLeft;
				}
				break;
			default:
				break;
			}//switch iElementNum
			if (nVLength > 0)
			{
				lpTemp += nVLength;
			}
			break;
			}
		case 0x0019:
			{
			if (nVLength > 0)
			{
				lpTemp += nVLength;
			}
			break;
			}
		case 0x0020:
			{
			switch(iElementNum)
			{
			case 0x0010://*Study_Id				(0020,0010) Study ID SH 1
				strncpy(ct_imageHeader.studyField.Id, lpTemp, nVLength);
				if(*(lpTemp+nVLength-1) == ' ')
				{
					ct_imageHeader.studyField.Id[nVLength-1]=0x00;
				}
				break;
			case 0x0011://*Series_Id			(0020,0011) Series Number IS 1
				strncpy(szTemp, lpTemp, nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				ct_imageHeader.seriesField.Id = (unsigned short)strtoul(szTemp,NULL,10);
				break;
			case 0x0013://*Image_Id				(0020,0013) Instance Number IS 1
				strncpy(szTemp, lpTemp, nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				ct_imageHeader.imageField.Id = (unsigned short)strtoul(szTemp,NULL,10);
				break;
			case 0x0032: //*(0020,0032) Image Position (Patient) DS 3
				GetDSVM(lpTemp, nVLength, dVal, nVM);
				if (nVM != 3)
				{
					return -6 ;
				}
				dOffSet[0] = dVal[0] ;
				dOffSet[1] = dVal[1] ;
				dOffSet[2] = dVal[2] ;
				
				break;
			case 0x0037://Image Orientation (Patient) DS 6
				GetDSVM(lpTemp, nVLength, dVal, nVM);
				if (nVM != 6)
				{
					return -7 ;
				}
				else
				{
					dImgDirec[0] = dVal[0] ;
					dImgDirec[1] = dVal[1] ;
					dImgDirec[2] = dVal[2] ;
					dImgDirec[3] = dVal[3] ;
					dImgDirec[4] = dVal[4] ;
					dImgDirec[5] = dVal[5] ;
				}
				break;
			case 0x1002://*ResolutionZ		(0020,1002)Images in Acquisition IS 1
				strncpy(szTemp, lpTemp, nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				ct_imageHeader.seriesField.ResolutionZ = (unsigned short)strtoul(szTemp,NULL,10);
				break;	
			case 0x1041: //slice location
				strncpy(szTemp, lpTemp, nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				dSliceLocation = atof(szTemp);
				//ct_imageHeader.seriesField.ResolutionZ = (unsigned short)strtoul(szTemp,NULL,10);
				break;	
			default:
				break;
			}//switch iElementNum
			if (nVLength > 0)
			{
				lpTemp += nVLength;
			}
			break;
			}
		case 0x0021:
			{
				if (nVLength > 0)
				{
					lpTemp += nVLength;
				}
			break;
			}
		case 0x0028:
			{
			switch(iElementNum)
			{
			case 0x0010: //(0028,0010)Rows  US 1
				ct_imageHeader.seriesField.ResolutionX = *(unsigned short*)lpTemp ;
				break;
			case 0x0011: //(0028,0010)Columns US 1
				ct_imageHeader.seriesField.ResolutionY = *(unsigned short*)lpTemp ;
				break;
			case 0x0030:  //(0028,0010)Pixel Spacing DS 2
				GetDSVM(lpTemp, nVLength, dVal, nVM);
				if (nVM == 2)
				{
					ct_imageHeader.seriesField.FovX = 
						(unsigned short)((double)ct_imageHeader.seriesField.ResolutionX*dVal[0]) ;
					ct_imageHeader.seriesField.FovY = 
						(unsigned short)((double)ct_imageHeader.seriesField.ResolutionY*dVal[1]) ;
					CalcBIImgDirec(dImgDirec, dOffSet ,
								   ct_imageHeader.studyField.PatPosition, 
								   ct_imageHeader.studyField.PatEntry, 
								   ct_imageHeader.seriesField.FovX,
								   ct_imageHeader.seriesField.FovY,
								   dXBImgDirec) ;
					ct_imageHeader.scanField.FreqDirX  = (float)dXBImgDirec[0] ;
					ct_imageHeader.scanField.FreqDirY  = (float)dXBImgDirec[1] ;
					ct_imageHeader.scanField.FreqDirZ  = (float)dXBImgDirec[2] ;
					ct_imageHeader.scanField.PhaseDirX = (float)dXBImgDirec[3] ;
					ct_imageHeader.scanField.PhaseDirY = (float)dXBImgDirec[4] ;
					ct_imageHeader.scanField.PhaseDirZ = (float)dXBImgDirec[5] ;
					ct_imageHeader.scanField.SliceDirX = (float)dXBImgDirec[6] ;
					ct_imageHeader.scanField.SliceDirY = (float)dXBImgDirec[7] ;
					ct_imageHeader.scanField.SliceDirZ = (float)dXBImgDirec[8] ;

					
					ct_imageHeader.imageField.fCenterPointX = (float)dXBImgDirec[9] ;
					ct_imageHeader.imageField.fCenterPointY = (float)dXBImgDirec[10] ;
					ct_imageHeader.imageField.fCenterPointZ = (float)dXBImgDirec[11] ;
					if(m_bToshibaCTTable)
					{
						ct_imageHeader.imageField.fCenterPointY = (float)dSliceLocation;
					}
				}
				else
				{
					return -8 ;
				}
				break;
			case 0x1050: //(0028,1050) Window Center DS 1
				strncpy(szTemp, lpTemp, nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				/*CT图像窗位需要加上1024*/
				ct_imageHeader.imageField.WindowLevel = 1024 + (short)strtoul(szTemp,NULL,10);
				break;
			case 0x1051: //(0028,1051) Window Width DS 1
				strncpy(szTemp, lpTemp, nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				ct_imageHeader.imageField.WindowWidth = (short)strtoul(szTemp,NULL,10);
				break;
			case 0x1052: // RescaleIntercept
				strncpy(szTemp, lpTemp, nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				nRescaleIntercept = (short)strtoul(szTemp,NULL,10);
				break;
			case 0x1053: // RescaleSlope
				strncpy(szTemp, lpTemp, nVLength);
				if(szTemp[nVLength - 1] == ' ')
				{
					szTemp[nVLength-1] = 0x00 ;
				}
				else
				{
					szTemp[nVLength] = 0x00 ;
				}
				nRescaleSlope = (short)strtoul(szTemp,NULL,10);
				break;
			default:
				break;
			}
			if (nVLength > 0)
			{
				lpTemp += nVLength;
			}
			break;
			}
		case 0x0029:
			{
			if (nVLength > 0)
			{
				lpTemp += nVLength;
			}
			break;
			}
		case 0x0043:
			{
			if (nVLength > 0)
			{
				lpTemp += nVLength;
			}
			break;
			}
		case 0x7FE0:
			{
			switch(iElementNum)
			{
			case 0x0010:
				{
					int nImgSize = 
						2*ct_imageHeader.seriesField.ResolutionX*ct_imageHeader.seriesField.ResolutionY ;
					
					nImageSize   = ct_imageHeader.seriesField.ResolutionX*ct_imageHeader.seriesField.ResolutionY ;
					
					pBIImageData = new WORD[nImageSize];

					if (pBIImageData != NULL)
					{
						memcpy(pBIImageData, lpTemp, nImageSize*2);
						if(nRescaleIntercept!=-1024)
						{
							for(int ii=0;ii<nImageSize;ii++)
							{
								pBIImageData[ii]+=(1024+nRescaleIntercept);
							}
						}
						
						lpTemp += nImageSize*2 ;
						return 0 ;
					}
					else
					{
						return -5 ;
					}
				}
				break;
			default:
				break;
			}
			if (nVLength > 0)
			{
				lpTemp += nVLength;
			}
			break;
			}
		default:
			if (nVLength > 0)
			{
				lpTemp += nVLength;
			}
			break;
		}//swtch groupnum

	}//while end

	return true;
}

int CDICOM2XINAO::ConvertCTDcm2XB(char* lpDcm, int nDcmSize, LPCT_STRUCT_IMAGE& pXinAoImg)
{
	int   nRet = 0 ;
	WORD* pBIImageData ;
	if (pXinAoImg != NULL)
	{
		return -10 ;   //should be null pointer.
	}
	pXinAoImg = new CT_STRUCT_IMAGE ;
	nRet = ConvertCTDcm2XB(pXinAoImg->oHeader,lpDcm,nDcmSize, pBIImageData) ;
	
	if (nRet == 0)
	{
		pXinAoImg->pData = (IMG_DATA_TYPE*)pBIImageData ;
	}

	


	return nRet ;
}

void CDICOM2XINAO::DCMTKConvertMRDcm2XB(void** dcmDataset,LPMR_STRUCT_IMAGE& pXinAoImg)
{
	DcmDataset **imageDataSet = (DcmDataset**)dcmDataset;
	
	DcmTagKey MediaStorageSOPClassUIDTagKey( DCM_MediaStorageSOPClassUID );				//DcmTagKey(0x0002, 0x0002)
	DcmTagKey TransferSyntaxUIDTagKey( DCM_TransferSyntaxUID );							//DcmTagKey(0x0002, 0x0010)
	DcmTagKey ImageTypeTagKey( DCM_ImageType );											//DcmTagKey(0x0008, 0x0008)
	DcmTagKey SOPClassUIDTagKey( DCM_SOPClassUID );										//DcmTagKey(0x0008, 0x0016)
	DcmTagKey StudyDateTagKey( DCM_StudyDate );											//DcmTagKey(0x0008, 0x0020)
	DcmTagKey SeriesDateTagKey( DCM_SeriesDate );										//DcmTagKey(0x0008, 0x0021)
	DcmTagKey AcquisitionDateTagKey( DCM_AcquisitionDate );								//DcmTagKey(0x0008, 0x0022)
	DcmTagKey StudyTimeTagKey( DCM_StudyTime );											//DcmTagKey(0x0008, 0x0030)
	DcmTagKey SeriesTimeTagKey( DCM_SeriesTime );										//DcmTagKey(0x0008, 0x0031)
	DcmTagKey AcquisitionTimeTagKey( DCM_AcquisitionTime );								//DcmTagKey(0x0008, 0x0032)
	DcmTagKey ModalityTagKey( DCM_Modality );                                           //DcmTagKey(0x0008, 0x0060)
	DcmTagKey ManufacturerTagKey( DCM_Manufacturer);									//DcmTagKey(0x0008, 0x0070)
	DcmTagKey InstitutionNameTagKey( DCM_InstitutionName );								//DcmTagKey(0x0008, 0x0080)
	DcmTagKey ReferringPhysiciansNameTagKey( DCM_ReferringPhysiciansName );				//DcmTagKey(0x0008, 0x0090)
	DcmTagKey StationNameTagKey( DCM_StationName );										//DcmTagKey(0x0008, 0x1010)
	DcmTagKey StudyDescriptionTagKey( DCM_StudyDescription );							//DcmTagKey(0x0008, 0x1030)
	DcmTagKey SeriesDescriptionTagKey( DCM_SeriesDescription );							//DcmTagKey(0x0008, 0x103e)
	DcmTagKey InstitutionalDepartmentNameTagKey( DCM_InstitutionalDepartmentName );     //DcmTagKey(0x0008, 0x1040)
	DcmTagKey ManufacturersModelNameTagKey( DCM_ManufacturersModelName );				//DcmTagKey(0x0008, 0x1090)
	DcmTagKey PatientsNameTagKey( DCM_PatientsName );									//DcmTagKey(0x0010, 0x0010)
	DcmTagKey PatientIDTagKey( DCM_PatientID );											//DcmTagKey(0x0010, 0x0020)
	DcmTagKey PatientsBirthDateTagKey( DCM_PatientsBirthDate );							//DcmTagKey(0x0010, 0x0030)
	DcmTagKey PatientsBirthTimeTagKey( DCM_PatientsBirthTime );							//DcmTagKey(0x0010, 0x0032)
	DcmTagKey PatientsSexTagKey( DCM_PatientsSex );										//DcmTagKey(0x0010, 0x0040)
	DcmTagKey PatientsAgeTagKey( DCM_PatientsAge );										//DcmTagKey(0x0010, 0x1010)
	DcmTagKey PatientsWeightTagKey( DCM_PatientsWeight );								//DcmTagKey(0x0010, 0x1030)
	DcmTagKey AdditionalPatientHistoryTagKey( DCM_AdditionalPatientHistory );			//DcmTagKey(0x0010, 0x21b0)
	DcmTagKey BodyPartExaminedTagKey( DCM_BodyPartExamined );							//DcmTagKey(0x0018, 0x0015)
	DcmTagKey ScanningSequenceTagKey( DCM_ScanningSequence );							//DcmTagKey(0x0018, 0x0020)
	DcmTagKey MRAcquisitionTypeTagKey( DCM_MRAcquisitionType );							//DcmTagKey(0x0018, 0x0023)
	DcmTagKey SequenceNameTagKey( DCM_SequenceName );									//DcmTagKey(0x0018, 0x0024)
	DcmTagKey SliceThicknessTagKey( DCM_SliceThickness );								//DcmTagKey(0x0018, 0x0050)
	DcmTagKey KVPTagKey( DCM_KVP );														//DcmTagKey(0x0018, 0x0060)
	DcmTagKey RepetitionTimeTagKey( DCM_RepetitionTime );								//DcmTagKey(0x0018, 0x0080)
	DcmTagKey EchoTimeTagKey( DCM_EchoTime );											//DcmTagKey(0x0018, 0x0081)
	DcmTagKey InversionTimeTagKey( DCM_InversionTime );									//DcmTagKey(0x0018, 0x0082)
	DcmTagKey NumberOfAveragesTagKey( DCM_NumberOfAverages );							//DcmTagKey(0x0018, 0x0083)
	DcmTagKey ImagingFrequencyTagKey( DCM_ImagingFrequency );							//DcmTagKey(0x0018, 0x0084)
	DcmTagKey ImagedNucleusTagKey( DCM_ImagedNucleus );									//DcmTagKey(0x0018, 0x0085)
	DcmTagKey EchoNumbersTagKey( DCM_EchoNumbers );										//DcmTagKey(0x0018, 0x0086)
	DcmTagKey MagneticFieldStrengthTagKey( DCM_MagneticFieldStrength );					//DcmTagKey(0x0018, 0x0087)
	DcmTagKey SpacingBetweenSlicesTagKey( DCM_SpacingBetweenSlices );					//DcmTagKey(0x0018, 0x0088)
	DcmTagKey NumberOfPhaseEncodingStepsTagKey( DCM_NumberOfPhaseEncodingSteps );       //DcmTagKey(0x0018, 0x0089)
	DcmTagKey DataCollectionDiameter( DCM_DataCollectionDiameter );						//DcmTagKey(0x0018, 0x0090)
	DcmTagKey EchoTrainLengthTagKey( DCM_EchoTrainLength );								//DcmTagKey(0x0018, 0x0091)
	DcmTagKey PixelBandwidthTagKey( DCM_PixelBandwidth );								//DcmTagKey(0x0018, 0x0095)
	DcmTagKey SoftwareVersionsTagKey( DCM_SoftwareVersions );							//DcmTagKey(0x0018, 0x1020)
	DcmTagKey ProtocolNameTagKey( DCM_ProtocolName );									//DcmTagKey(0x0018, 0x1030)
	DcmTagKey TriggerTimeTagKey( DCM_TriggerTime );										//DcmTagKey(0x0018, 0x1060)
	DcmTagKey SkipBeatsTagKey( DCM_SkipBeats );											//DcmTagKey(0x0018, 0x1086)
	DcmTagKey HeartRateTagKey( DCM_HeartRate );											//DcmTagKey(0x0018, 0x1088)
	DcmTagKey CardiacNumberOfImagesTagKey( DCM_CardiacNumberOfImages );					//DcmTagKey(0x0018, 0x1090)
	DcmTagKey ReconstructionDiameterTagKey( DCM_ReconstructionDiameter );				//DcmTagKey(0x0018, 0x1100)
	DcmTagKey GantryDetectorTiltTagKey( DCM_GantryDetectorTilt );						//DcmTagKey(0x0018, 0x1120)
	DcmTagKey TableHeightTagKey( DCM_TableHeight );										//DcmTagKey(0x0018, 0x1130)
	DcmTagKey RotationDirectionTagKey( DCM_RotationDirection );							//DcmTagKey(0x0018, 0x1140)
	DcmTagKey XRayTubeCurrentTagKey( DCM_XRayTubeCurrent );								//DcmTagKey(0x0018, 0x1151)
	DcmTagKey ExposureTagKey( DCM_Exposure );											//DcmTagKey(0x0018, 0x1152)
	DcmTagKey DateOfLastCalibrationTagKey( DCM_DateOfLastCalibration );					//DcmTagKey(0x0018, 0x1200)
	DcmTagKey TimeOfLastCalibrationTagKey( DCM_TimeOfLastCalibration );					//DcmTagKey(0x0018, 0x1201)
	DcmTagKey ReceiveCoilNameTagKey( DCM_ReceiveCoilName );								//DcmTagKey(0x0018, 0x1250)
	DcmTagKey TransmitCoilNameTagKey( DCM_TransmitCoilName );							//DcmTagKey(0x0018, 0x1251)
	DcmTagKey AcquisitionMatrixTagKey( DCM_AcquisitionMatrix );							//DcmTagKey(0x0018, 0x1310)
	DcmTagKey InPlanePhaseEncodingDirectionTagKey( DCM_InPlanePhaseEncodingDirection ); //DcmTagKey(0x0018, 0x1312)
	DcmTagKey FlipAngleTagKey( DCM_FlipAngle );											//DcmTagKey(0x0018, 0x1314)
	DcmTagKey SARTagKey( DCM_SAR );														//DcmTagKey(0x0018, 0x1316)
	DcmTagKey PatientPositionTagKey( DCM_PatientPosition );								//DcmTagKey(0x0018, 0x5100)
	DcmTagKey StudyIDTagKey( DCM_StudyID );												//DcmTagKey(0x0020, 0x0010)
	DcmTagKey SeriesNumberTagKey( DCM_SeriesNumber );									//DcmTagKey(0x0020, 0x0011)
	DcmTagKey InstanceNumberTagKey( DCM_InstanceNumber );								//DcmTagKey(0x0020, 0x0013)
	DcmTagKey ImagePositionPatientTagKey( DCM_ImagePositionPatient );					//DcmTagKey(0x0020, 0x0032)
	DcmTagKey ImageOrientationPatientTagKey( DCM_ImageOrientationPatient );				//DcmTagKey(0x0020, 0x0037)
	DcmTagKey ImagesInAcquisitionTagKey( DCM_ImagesInAcquisition );						//DcmTagKey(0x0020, 0x1002)
	DcmTagKey SliceLocationTagKey( DCM_SliceLocation );									//DcmTagKey(0x0020, 0x1041)
	DcmTagKey RowsTagKey( DCM_Rows );													//DcmTagKey(0x0028, 0x0010)
	DcmTagKey ColumnsTagKey( DCM_Columns );												//DcmTagKey(0x0028, 0x0011)
	DcmTagKey PixelSpacingTagKey( DCM_PixelSpacing );									//DcmTagKey(0x0028, 0x0030)
	DcmTagKey WindowCenterTagKey( DCM_WindowCenter );									//DcmTagKey(0x0028, 0x1050)
	DcmTagKey WindowWidthTagKey( DCM_WindowWidth );										//DcmTagKey(0x0028, 0x1051)
	DcmTagKey RescaleInterceptTagKey( DCM_RescaleIntercept );							//DcmTagKey(0x0028, 0x1052)	
	DcmTagKey RescaleSlopeTagKey( DCM_RescaleSlope );									//DcmTagKey(0x0028, 0x1053)
	DcmTagKey PixelDataTagKey( DCM_PixelData );											//DcmTagKey(0x7fe0, 0x0010)
	
	char *lpTemp = NULL;
	char			szTemp[64] ;
	unsigned short	sPoints ;
	int				count = 0 ;
	int				i     = 0 ;
	double			dVal[20] ;
	double			dImgDirec[6] ;
	double			dXBImgDirec[12] ;
	double			dOffSet[3] ;
	double			dSliceThick = 5.0;
	double          dSliceLocation = 0.0;
	short			nRescaleIntercept=0;
	short			nRescaleSlope=1;
	int				nImageSize = 1 ;
	int				nVM;
	int				nModality = -1;
	
	pXinAoImg = new MR_STRUCT_IMAGE;
	memset(&(pXinAoImg->oHeader.headInfo), 0 ,sizeof(MR_ImageHeader) );
	// Initialize
	pXinAoImg->oHeader.headInfo.BITitle[0] = 'D';
	pXinAoImg->oHeader.headInfo.BITitle[1] = 'C';
	pXinAoImg->oHeader.headInfo.BITitle[2] = 'M';
	pXinAoImg->oHeader.headInfo.BITitle[3] = 'M';
	pXinAoImg->oHeader.headInfo.BITitle[4] = 'R';
	pXinAoImg->oHeader.headInfo.Version = int (1) << 16 + 0;
	pXinAoImg->oHeader.headInfo.Size = sizeof (MR_HeaderInfo);
	pXinAoImg->oHeader.headInfo.NumHdrFields = 6; 
	pXinAoImg->oHeader.headInfo.HeaderSize =  sizeof(MR_HeaderInfo)   + sizeof(MR_SiteField) +
		sizeof(MR_PatientField) + sizeof(MR_StudyField)+ 
		sizeof(MR_SeriesField)  + sizeof(MR_ScanField) + 
		sizeof (MR_ImageField);
	
	//SiteField
	pXinAoImg->oHeader.siteField.Size = sizeof(MR_SiteField);
	
	//PatientField
	pXinAoImg->oHeader.patientField.Size = sizeof(MR_PatientField);
	
	//StudyField
	pXinAoImg->oHeader.studyField.Size = sizeof(MR_StudyField);
	
	//SeriesField
	pXinAoImg->oHeader.seriesField.Size = sizeof(MR_SeriesField);
	pXinAoImg->oHeader.seriesField.NumSlab = 1 ;
	pXinAoImg->oHeader.seriesField.RcvCoil = ctSystemCoil;
	pXinAoImg->oHeader.seriesField.TXCoil  = ctSystemCoil;
	
	//ScanField
	pXinAoImg->oHeader.scanField.Size = sizeof(MR_ScanField);
	
	//ImageField
	pXinAoImg->oHeader.imageField.Size = sizeof(MR_ImageField);
	
	//DcmTagKey(0x0008, 0x0008)
	OFCondition eb = (*imageDataSet)->findAndGetString( ImageTypeTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
	{
		if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\MPR", strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.imageField.Type = itMPR;
		}
		else if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\PROJECTION IMAGE ", strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.imageField.Type = itProjection;
		}
		else if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\IMAGE",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.imageField.Type=itImage;
		}
		else if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\T1 MAP",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.imageField.Type = itT1Map ;
		}
		else if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\T2 MAP", strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.imageField.Type=itT2Map;
		}
		else if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\DIFFUSION MAP", strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.imageField.Type=itDiffusionMap;
		}
		else if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\DENSITY MAP",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.imageField.Type = itDensityMap;
		}
		else if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\PHASE MAP",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.imageField.Type = itPhaseMap;
		}
		else if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\VELOCITY MAP",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.imageField.Type = itVelocityMap;
		}
		else if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\IMAGE ADDITION",strlen(lpTemp))==0)
		{
			pXinAoImg->oHeader.imageField.Type = itImageAddition ;
		}
		else if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\PHASE SUBTRACT",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.imageField.Type = itPhaseSubtract ;
		}
		else if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\MODULUS SUBTRACT", strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.imageField.Type = itModulusSubtract ;
		}
		else if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\OTHER", strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.imageField.Type = itOther ;
		}
	}

	//DcmTagKey(0x0008, 0x0020)
	eb = (*imageDataSet)->findAndGetString( StudyDateTagKey, lpTemp, OFFalse );   
	if(eb == EC_Normal && lpTemp != NULL)
		TimeConvert2(pXinAoImg->oHeader.studyField.Time, lpTemp, 1);
	//DcmTagKey(0x0008, 0x0021)
	eb = (*imageDataSet)->findAndGetString( SeriesDateTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
		TimeConvert2(pXinAoImg->oHeader.seriesField.DateTime, lpTemp, 1);
	//DcmTagKey(0x0008, 0x0022)
	eb = (*imageDataSet)->findAndGetString( AcquisitionDateTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
		TimeConvert2(pXinAoImg->oHeader.imageField.DateTime, lpTemp, 1);
	//DcmTagKey(0x0008, 0x0030)
	eb = (*imageDataSet)->findAndGetString( StudyTimeTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)	
		TimeConvert2(pXinAoImg->oHeader.studyField.Time, lpTemp, 2);
	//DcmTagKey(0x0008, 0x0031)
	eb = (*imageDataSet)->findAndGetString( SeriesTimeTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
		TimeConvert2(pXinAoImg->oHeader.seriesField.DateTime, lpTemp, 2);
	//DcmTagKey(0x0008, 0x0032)
	eb = (*imageDataSet)->findAndGetString( AcquisitionTimeTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
		TimeConvert2(pXinAoImg->oHeader.imageField.DateTime, lpTemp, 2);
	//DcmTagKey(0x0008, 0x0080)
	eb = (*imageDataSet)->findAndGetString( InstitutionNameTagKey, lpTemp, OFFalse );  
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(pXinAoImg->oHeader.siteField.InstName, lpTemp, strlen(lpTemp));
		if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
		{
			pXinAoImg->oHeader.siteField.InstName[strlen(lpTemp)-1] = 0x00;
		}
	}
	//DcmTagKey(0x0008, 0x0090)
	eb = (*imageDataSet)->findAndGetString( ReferringPhysiciansNameTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
		NameConvert2(pXinAoImg->oHeader.studyField.RefPhy,lpTemp,strlen(lpTemp));
	//DcmTagKey(0x0008, 0x1010)
	eb = (*imageDataSet)->findAndGetString( StationNameTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(pXinAoImg->oHeader.siteField.StationName,lpTemp,strlen(lpTemp));
		if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
		{
			pXinAoImg->oHeader.siteField.StationName[strlen(lpTemp)-1] = 0x00;
		}
	}
	//DcmTagKey(0x0008, 0x1030)
	eb = (*imageDataSet)->findAndGetString( StudyDescriptionTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(pXinAoImg->oHeader.studyField.Desc, lpTemp, strlen(lpTemp));
		if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
		{
			pXinAoImg->oHeader.studyField.Desc[strlen(lpTemp)-1] = 0x00;
		}
	}
	//DcmTagKey(0x0008, 0x103e)
	eb = (*imageDataSet)->findAndGetString( SeriesDescriptionTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(pXinAoImg->oHeader.seriesField.Desc, lpTemp, strlen(lpTemp));
		if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
		{
			pXinAoImg->oHeader.seriesField.Desc[strlen(lpTemp)-1] = 0x00;
		}
	}
	//DcmTagKey(0x0008, 0x1040)
	eb = (*imageDataSet)->findAndGetString( InstitutionalDepartmentNameTagKey,lpTemp,OFFalse);
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(pXinAoImg->oHeader.siteField.InstDeptName, lpTemp, strlen(lpTemp));
		if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
		{
			pXinAoImg->oHeader.siteField.InstDeptName[strlen(lpTemp)-1] = 0x00;
		}
	}
	//DcmTagKey(0x0008, 0x1090)
	eb = (*imageDataSet)->findAndGetString( ManufacturersModelNameTagKey,lpTemp,OFFalse);
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(pXinAoImg->oHeader.siteField.BISerialNum, lpTemp, strlen(lpTemp));
		if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
		{
			pXinAoImg->oHeader.siteField.BISerialNum[strlen(lpTemp)-1] = 0x00;
		}
	}
	//DcmTagKey(0x0010, 0x0010)
	eb = (*imageDataSet)->findAndGetString( PatientsNameTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		NameConvert2(pXinAoImg->oHeader.patientField.Name, lpTemp, strlen(lpTemp));
	}		
	//DcmTagKey(0x0010, 0x0020)
	eb = (*imageDataSet)->findAndGetString( PatientIDTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(pXinAoImg->oHeader.patientField.Id, lpTemp, strlen(lpTemp));
		if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
		{
			pXinAoImg->oHeader.patientField.Id[strlen(lpTemp)-1]=0x00;
		}
	}
	//DcmTagKey(0x0010, 0x0030)
	eb = (*imageDataSet)->findAndGetString( PatientsBirthDateTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
		TimeConvert2(pXinAoImg->oHeader.patientField.BirthDate, lpTemp, 1);
	//DcmTagKey(0x0010, 0x0032)
	eb = (*imageDataSet)->findAndGetString( PatientsBirthTimeTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
		TimeConvert2(pXinAoImg->oHeader.patientField.BirthDate, lpTemp, 2);
	//DcmTagKey(0x0010, 0x0040)
	eb = (*imageDataSet)->findAndGetString( PatientsSexTagKey, lpTemp, OFFalse );  
	if(eb == EC_Normal)
	{
		if(lpTemp == NULL)
		{
			pXinAoImg->oHeader.patientField.Sex = 'U';
		}
		else
		{
			pXinAoImg->oHeader.patientField.Sex = *lpTemp;
		}
	}
	//DcmTagKey(0x0010, 0x1010)
	eb = (*imageDataSet)->findAndGetString( PatientsAgeTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
		pXinAoImg->oHeader.patientField.Age = AgeConvert2(lpTemp);
	//DcmTagKey(0x0010, 0x1030)
	eb = (*imageDataSet)->findAndGetString( PatientsWeightTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp, lpTemp, strlen(lpTemp));
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		pXinAoImg->oHeader.patientField.Weight = atoi(szTemp);
	}
	//DcmTagKey(0x0010, 0x21b0)
	eb = (*imageDataSet)->findAndGetString( AdditionalPatientHistoryTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(pXinAoImg->oHeader.patientField.Desc,lpTemp,strlen(lpTemp));
		if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
		{
			pXinAoImg->oHeader.patientField.Desc[strlen(lpTemp)-1] = 0x00;
		}
	}
	//DcmTagKey(0x0018, 0x0015)
	eb = (*imageDataSet)->findAndGetString( BodyPartExaminedTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		if(strncmp(lpTemp, "SKULL ", strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.studyField.PatBodyPart = bpHead;
		}
		else if(strncmp(lpTemp, "NECK", strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.studyField.PatBodyPart = bpNeck;
		}
		else if(strncmp(lpTemp, "SHOULDER", strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.studyField.PatBodyPart = bpShoulder;
		}
		else if(strncmp(lpTemp, "CSPINE", strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.studyField.PatBodyPart = bpCervicalSpine;
		}
		else if(strncmp(lpTemp,"TSPINE",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.studyField.PatBodyPart = bpThoracicSpine;
		}
		else if(strncmp(lpTemp,"LSPINE",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.studyField.PatBodyPart = bpLumberSpine;
		}
		else if(strncmp(lpTemp,"CHEST ",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.studyField.PatBodyPart = bpChest;
		}
		else if(strncmp(lpTemp,"BREAST",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.studyField.PatBodyPart = bpBreast;
		}
		else if(strncmp(lpTemp,"ABODOMEN",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.studyField.PatBodyPart = bpAbdomen;
		}
		else if(strncmp(lpTemp,"PELVIS",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.studyField.PatBodyPart = bpPelvis;
		}
		else if(strncmp(lpTemp,"HIP ",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.studyField.PatBodyPart = bpHip;
		}
		else if(strncmp(lpTemp,"ELBOW ",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.studyField.PatBodyPart = bpElbow;
		}
		else if(strncmp(lpTemp,"WRIST ",strlen(lpTemp)) == 0)
			pXinAoImg->oHeader.studyField.PatBodyPart = bpWrist;
		else if(strncmp(lpTemp,"ARM ",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.studyField.PatBodyPart = bpArm;
		}
		else if(strncmp(lpTemp,"HAND",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.studyField.PatBodyPart = bpHand;
		}
		else if(strncmp(lpTemp,"KNEE",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.studyField.PatBodyPart = bpKnee;
		}
		else if(strncmp(lpTemp,"LEG ",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.studyField.PatBodyPart = bpLeg;
		}
		else if(strncmp(lpTemp,"ANKLE ",strlen(lpTemp))==0)
		{
			pXinAoImg->oHeader.studyField.PatBodyPart = bpAnkle;
		}
		else if(strncmp(lpTemp,"FOOT",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.studyField.PatBodyPart = bpFoot;
		}
		else if(strncmp(lpTemp,"HEART ",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.studyField.PatBodyPart = bpHeart;
		}
		else if(strncmp(lpTemp,"VASCULARHEAD",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.studyField.PatBodyPart = bpVascularHead;
		}
		else if(strncmp(lpTemp,"VASCULARNECK",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.studyField.PatBodyPart = bpVascularNeck;
		}
		else if(strncmp(lpTemp,"VASCULARBODY",strlen(lpTemp))==0)
		{
			pXinAoImg->oHeader.studyField.PatBodyPart=bpVascularBody;
		}
		else if(strncmp(lpTemp,"VASCULARPER ",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.studyField.PatBodyPart = bpVascularPeripheral;
		}
	}
	//DcmTagKey(0x0018, 0x0020)
	eb = (*imageDataSet)->findAndGetString( ScanningSequenceTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(pXinAoImg->oHeader.seriesField.PulseSeqName, lpTemp, strlen(lpTemp));
		if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
		{
			pXinAoImg->oHeader.seriesField.PulseSeqName[strlen(lpTemp)-1] = 0x00;
		}
		pXinAoImg->oHeader.seriesField.SequenceOptions = GetSeriesOption(pXinAoImg->oHeader.seriesField.PulseSeqName) ;
	}
	//DcmTagKey(0x0018, 0x0023)
	eb = (*imageDataSet)->findAndGetString( MRAcquisitionTypeTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		if (strncmp(lpTemp, "2D", 2) == 0)
		{
			pXinAoImg->oHeader.seriesField.Mode = im2D ;
		}
		else if (strncmp(lpTemp, "3D", 2) == 0)
		{
			pXinAoImg->oHeader.seriesField.Mode = im3D ;
		}
	}
	//DcmTagKey(0x0018, 0x0024)
	eb = (*imageDataSet)->findAndGetString( SequenceNameTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(pXinAoImg->oHeader.seriesField.PulseSeqName, lpTemp, strlen(lpTemp));
		if(*(lpTemp+strlen(lpTemp)-1) == ' ')
		{
			pXinAoImg->oHeader.seriesField.PulseSeqName[strlen(lpTemp)-1] = 0x00;
		}
	}
	//DcmTagKey(0x0018, 0x0050)			
	eb = (*imageDataSet)->findAndGetString( SliceThicknessTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp, lpTemp, strlen(lpTemp));
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		pXinAoImg->oHeader.seriesField.SliceThickness = (float)atof(szTemp);
		dSliceThick = pXinAoImg->oHeader.seriesField.SliceThickness ;
	}
	//DcmTagKey(0x0018, 0x0080)
	eb = (*imageDataSet)->findAndGetString( RepetitionTimeTagKey, lpTemp, OFFalse );  
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp, lpTemp, strlen(lpTemp));
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		pXinAoImg->oHeader.seriesField.RepTime = strtoul(szTemp,NULL,10);
	}
	//DcmTagKey(0x0018, 0x0081)
	eb = (*imageDataSet)->findAndGetString( EchoTimeTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp, lpTemp, strlen(lpTemp));
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		pXinAoImg->oHeader.seriesField.EchoTime = strtoul(szTemp,NULL,10);
	}
	//DcmTagKey(0x0018, 0x0082)
	eb = (*imageDataSet)->findAndGetString( InversionTimeTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp,lpTemp,strlen(lpTemp));
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		pXinAoImg->oHeader.seriesField.TI = strtoul(szTemp,NULL,10);
	}
	//DcmTagKey(0x0018, 0x0083)
	eb = (*imageDataSet)->findAndGetString( NumberOfAveragesTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp,lpTemp,strlen(lpTemp));
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		pXinAoImg->oHeader.seriesField.NumAvg = (short)strtoul(szTemp,NULL,10);
	}
	//DcmTagKey(0x0018, 0x0084)
	eb = (*imageDataSet)->findAndGetString( ImagingFrequencyTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp,lpTemp,strlen(lpTemp));
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		pXinAoImg->oHeader.siteField.SF = (float)strtod(szTemp,NULL);
	}
	//DcmTagKey(0x0018, 0x0085)			
	eb = (*imageDataSet)->findAndGetString( ImagedNucleusTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
	{
		if(strncmp(lpTemp,"1H",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.seriesField.NucType = ntH1;
		}
		else if(strncmp(lpTemp,"3He ",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.seriesField.NucType = ntHe3;
		}
		else if(strncmp(lpTemp,"13C ",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.seriesField.NucType = ntC13;
		}
		else if(strncmp(lpTemp,"19F ",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.seriesField.NucType = ntF19;
		}
		else if(strncmp(lpTemp,"23Na",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.seriesField.NucType = ntNa23;
		}
		else if(strncmp(lpTemp,"31P ",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.seriesField.NucType = ntP31;
		}
		else if(strncmp(lpTemp,"39K ",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.seriesField.NucType = ntK39;
		}
		else if(strncmp(lpTemp,"129Xe ",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.seriesField.NucType = ntXe129;
		}
	}
	//DcmTagKey(0x0018, 0x0086)
	eb = (*imageDataSet)->findAndGetString( EchoNumbersTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp,lpTemp,strlen(lpTemp));
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		pXinAoImg->oHeader.imageField.EchoNum = (unsigned short)strtoul(szTemp,NULL,10);
	}
	//DcmTagKey(0x0018, 0x0087)
	eb = (*imageDataSet)->findAndGetString( MagneticFieldStrengthTagKey,lpTemp,OFFalse);
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp,lpTemp,strlen(lpTemp));
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		pXinAoImg->oHeader.siteField.MagCalibStrength = (float)strtod(szTemp,NULL);
	}
	//DcmTagKey(0x0018, 0x0088)
	eb = (*imageDataSet)->findAndGetString( SpacingBetweenSlicesTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp, lpTemp ,strlen(lpTemp));
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		pXinAoImg->oHeader.seriesField.SliceGap  = (float)strtod(szTemp,NULL);
		pXinAoImg->oHeader.seriesField.SliceGap -= (float)dSliceThick ;
	}
	//DcmTagKey(0x0018, 0x0089)
	eb = (*imageDataSet)->findAndGetString( NumberOfPhaseEncodingStepsTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp, lpTemp, strlen(lpTemp)) ;
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		pXinAoImg->oHeader.seriesField.PhasePoints = (unsigned short)strtoul(szTemp,NULL,10);
	}
	//DcmTagKey(0x0018, 0x0091)
	eb = (*imageDataSet)->findAndGetString( EchoTrainLengthTagKey, lpTemp, OFFalse );  
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp, lpTemp, strlen(lpTemp));
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		pXinAoImg->oHeader.seriesField.EchoTrainLength = (unsigned short)strtoul(szTemp,NULL,10);
	}
	//DcmTagKey(0x0018, 0x0095)
	eb = (*imageDataSet)->findAndGetString( PixelBandwidthTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp, lpTemp, strlen(lpTemp));
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		pXinAoImg->oHeader.seriesField.AcqBandwidth = (float)strtod(szTemp,NULL);
	}
	//DcmTagKey(0x0018, 0x1030)
	eb = (*imageDataSet)->findAndGetString( ProtocolNameTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(pXinAoImg->oHeader.seriesField.Protocol, lpTemp, strlen(lpTemp));
		if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
		{
			pXinAoImg->oHeader.seriesField.Protocol[strlen(lpTemp)-1] = 0x00;
		}
	}
	//DcmTagKey(0x0018, 0x1060)
	eb = (*imageDataSet)->findAndGetString( TriggerTimeTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
		strncpy(szTemp, lpTemp, strlen(lpTemp));
	//DcmTagKey(0x0018, 0x1086)
	eb = (*imageDataSet)->findAndGetString( SkipBeatsTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
		strncpy(szTemp,lpTemp,strlen(lpTemp));
	//DcmTagKey(0x0018, 0x1088)
	eb = (*imageDataSet)->findAndGetString( HeartRateTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
		strncpy(szTemp,lpTemp,strlen(lpTemp));
	
	eb = (*imageDataSet)->findAndGetString( CardiacNumberOfImagesTagKey, lpTemp, OFFalse );
	//DcmTagKey(0x0018, 0x1200)
	eb = (*imageDataSet)->findAndGetString( DateOfLastCalibrationTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
		TimeConvert2(pXinAoImg->oHeader.siteField.LastCalib,lpTemp,1);
	//DcmTagKey(0x0018, 0x1201)
	eb = (*imageDataSet)->findAndGetString( TimeOfLastCalibrationTagKey, lpTemp, OFFalse );  
	if(eb == EC_Normal && lpTemp != NULL)
		TimeConvert2(pXinAoImg->oHeader.siteField.LastCalib,lpTemp,2);
	//DcmTagKey(0x0018, 0x1250)
	eb = (*imageDataSet)->findAndGetString( ReceiveCoilNameTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(pXinAoImg->oHeader.seriesField.UserRcvCoilDesc, lpTemp, strlen(lpTemp));
		if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
		{
			pXinAoImg->oHeader.seriesField.UserRcvCoilDesc[strlen(lpTemp)-1] = 0x00;
		}
	}
	//DcmTagKey(0x0018, 0x1251)
	eb = (*imageDataSet)->findAndGetString( TransmitCoilNameTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(pXinAoImg->oHeader.seriesField.UserTxCoilDesc, lpTemp, strlen(lpTemp));
		if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
		{
			pXinAoImg->oHeader.seriesField.UserTxCoilDesc[strlen(lpTemp)-1] = 0x00;
		}
	}
	//DcmTagKey(0x0018, 0x1310)
	eb = (*imageDataSet)->findAndGetString( AcquisitionMatrixTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		sPoints  = *(unsigned short*)lpTemp;
		lpTemp += 2 ;
		if (sPoints > 0)
		{
			pXinAoImg->oHeader.seriesField.ReadPoints  = sPoints;
		}
		sPoints  = *(unsigned short*)lpTemp ;
		lpTemp += 2 ;
		if (sPoints > 0)
		{
			pXinAoImg->oHeader.seriesField.ReadPoints  = sPoints;
		}
		sPoints  = *(unsigned short*)lpTemp ;
		lpTemp += 2 ;
		if (sPoints > 0)
		{
			pXinAoImg->oHeader.seriesField.PhasePoints  = sPoints;
		}
		sPoints  = *(unsigned short*)lpTemp ;
		lpTemp += 2 ;
		if (sPoints > 0)
		{
			pXinAoImg->oHeader.seriesField.PhasePoints  = sPoints;
		}
		lpTemp -= 8 ;
	}
	//DcmTagKey(0x0018, 0x1312)
	eb = (*imageDataSet)->findAndGetString( InPlanePhaseEncodingDirectionTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		if (strncmp(lpTemp, "ROW", 3) == 0)
		{
			pXinAoImg->oHeader.seriesField.OtherOptions += 0x400 ;
		}
	}
	//DcmTagKey(0x0018, 0x1314)
	eb = (*imageDataSet)->findAndGetString( FlipAngleTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp, lpTemp, strlen(lpTemp));
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		pXinAoImg->oHeader.seriesField.FlipAngle = (float)atof(szTemp);
	}
	//DcmTagKey(0x0018, 0x1316)
	eb = (*imageDataSet)->findAndGetString( SARTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp, lpTemp, strlen(lpTemp));
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		pXinAoImg->oHeader.seriesField.SARAvg = (int)strtod(szTemp,NULL);
	}
	//DcmTagKey(0x0018, 0x5100)
	eb = (*imageDataSet)->findAndGetString( PatientPositionTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		if(strncmp(lpTemp,"HFS ",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.studyField.PatEntry    = beHeadFirst;
			pXinAoImg->oHeader.studyField.PatPosition = bpSupine;
		}
		else if(strncmp(lpTemp,"HFP ",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.studyField.PatEntry    = beHeadFirst;
			pXinAoImg->oHeader.studyField.PatPosition = bpProne;
		}
		else if(strncmp(lpTemp,"HFDR",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.studyField.PatEntry    = beHeadFirst;
			pXinAoImg->oHeader.studyField.PatPosition = bpDecubitusRight;
		}
		else if(strncmp(lpTemp,"HFDL",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.studyField.PatEntry    = beHeadFirst;
			pXinAoImg->oHeader.studyField.PatPosition = bpDecubitusLeft;
		}
		else if(strncmp(lpTemp,"FFS ",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.studyField.PatEntry    = beFootFirst;
			pXinAoImg->oHeader.studyField.PatPosition = bpSupine;
		}
		else if(strncmp(lpTemp,"FFP ",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.studyField.PatEntry    = beFootFirst;
			pXinAoImg->oHeader.studyField.PatPosition = bpProne;
		}
		else if(strncmp(lpTemp,"FFDR",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.studyField.PatEntry    = beFootFirst;
			pXinAoImg->oHeader.studyField.PatPosition = bpDecubitusRight;
		}
		else if(strncmp(lpTemp,"FFDL",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.studyField.PatEntry    = beFootFirst;
			pXinAoImg->oHeader.studyField.PatPosition = bpDecubitusLeft;
		}
	}
	//DcmTagKey(0x0020, 0x0010)
	eb = (*imageDataSet)->findAndGetString( StudyIDTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(pXinAoImg->oHeader.studyField.Id, lpTemp, strlen(lpTemp));
		if(*(lpTemp+strlen(lpTemp)-1) == ' ')
		{
			pXinAoImg->oHeader.studyField.Id[strlen(lpTemp)-1]=0x00;
		}
	}
	//DcmTagKey(0x0020, 0x0011)
	eb = (*imageDataSet)->findAndGetString( SeriesNumberTagKey, lpTemp, OFFalse );  
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp, lpTemp, strlen(lpTemp));
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		pXinAoImg->oHeader.seriesField.Id = (BYTE)strtoul(szTemp,NULL,10);
	}
	//DcmTagKey(0x0020, 0x0013)
	eb = (*imageDataSet)->findAndGetString( InstanceNumberTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp, lpTemp, strlen(lpTemp));
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		pXinAoImg->oHeader.imageField.Id = (unsigned short)strtoul(szTemp,NULL,10);
	}
	//DcmTagKey(0x0020, 0x0032)
	eb = (*imageDataSet)->findAndGetString( ImagePositionPatientTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
	{
		GetDSVM(lpTemp, strlen(lpTemp), dVal, nVM);
		if (nVM != 3)
		{
			return ;
		}
		dOffSet[0] = dVal[0] ;
		dOffSet[1] = dVal[1] ;
		dOffSet[2] = dVal[2] ;
		pXinAoImg->oHeader.scanField.SliceOffset = 0 ;
		pXinAoImg->oHeader.scanField.ReadOffset  = 0 ;
		pXinAoImg->oHeader.scanField.PhaseOffset = 0 ;
	}
	//DcmTagKey(0x0020, 0x0037)
	eb = (*imageDataSet)->findAndGetString( ImageOrientationPatientTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		GetDSVM(lpTemp, strlen(lpTemp), dVal, nVM);
		if (nVM != 6)
		{
			return ;
		}
		else
		{
			dImgDirec[0] = dVal[0] ;
			dImgDirec[1] = dVal[1] ;
			dImgDirec[2] = dVal[2] ;
			dImgDirec[3] = dVal[3] ;
			dImgDirec[4] = dVal[4] ;
			dImgDirec[5] = dVal[5] ;
		}
	}
	//DcmTagKey(0x0020, 0x1002)
	eb = (*imageDataSet)->findAndGetString( ImagesInAcquisitionTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp, lpTemp, strlen(lpTemp));
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		pXinAoImg->oHeader.seriesField.ResolutionZ = (unsigned short)strtoul(szTemp,NULL,10);
	}
	//DcmTagKey(0x0028, 0x0010)
	Uint16 RowsValue;
	UINT16 ColumnsValue;
	eb = (*imageDataSet)->findAndGetUint16( RowsTagKey, RowsValue, OFFalse );
	if(eb == EC_Normal)
		pXinAoImg->oHeader.seriesField.ResolutionX = RowsValue;
	//DcmTagKey(0x0028, 0x0011)
	eb = (*imageDataSet)->findAndGetUint16( ColumnsTagKey, ColumnsValue, OFFalse );
	if(eb == EC_Normal)
		pXinAoImg->oHeader.seriesField.ResolutionY = ColumnsValue ;
	//DcmTagKey(0x0028, 0x0030)
	eb = (*imageDataSet)->findAndGetString( PixelSpacingTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
	{
		GetDSVM(lpTemp, strlen(lpTemp), dVal, nVM);
		if (nVM == 2)
		{
			pXinAoImg->oHeader.seriesField.FovX = 
				(unsigned short)((double)pXinAoImg->oHeader.seriesField.ResolutionX*dVal[0]) ;
			pXinAoImg->oHeader.seriesField.FovY = 
				(unsigned short)((double)pXinAoImg->oHeader.seriesField.ResolutionY*dVal[1]) ;
			CalcBIImgDirec(dImgDirec, dOffSet ,
				pXinAoImg->oHeader.studyField.PatPosition, 
				pXinAoImg->oHeader.studyField.PatEntry, 
				pXinAoImg->oHeader.seriesField.FovX,
				pXinAoImg->oHeader.seriesField.FovY,
				dXBImgDirec) ;
			pXinAoImg->oHeader.scanField.FreqDirX  = (float)dXBImgDirec[0] ;
			pXinAoImg->oHeader.scanField.FreqDirY  = (float)dXBImgDirec[1] ;
			pXinAoImg->oHeader.scanField.FreqDirZ  = (float)dXBImgDirec[2] ;
			pXinAoImg->oHeader.scanField.PhaseDirX = (float)dXBImgDirec[3] ;
			pXinAoImg->oHeader.scanField.PhaseDirY = (float)dXBImgDirec[4] ;
			pXinAoImg->oHeader.scanField.PhaseDirZ = (float)dXBImgDirec[5] ;
			pXinAoImg->oHeader.scanField.SliceDirX = (float)dXBImgDirec[6] ;
			pXinAoImg->oHeader.scanField.SliceDirY = (float)dXBImgDirec[7] ;
			pXinAoImg->oHeader.scanField.SliceDirZ = (float)dXBImgDirec[8] ;
			
			pXinAoImg->oHeader.imageField.fFreqDirX     = (float)dXBImgDirec[0] ;
			pXinAoImg->oHeader.imageField.fFreqDirY     = (float)dXBImgDirec[1] ;
			pXinAoImg->oHeader.imageField.fFreqDirZ     = (float)dXBImgDirec[2] ;
			pXinAoImg->oHeader.imageField.fPhaseDirX    = (float)dXBImgDirec[3] ;
			pXinAoImg->oHeader.imageField.fPhaseDirY    = (float)dXBImgDirec[4] ;
			pXinAoImg->oHeader.imageField.fPhaseDirZ    = (float)dXBImgDirec[5] ;
			pXinAoImg->oHeader.imageField.fSliceDirX    = (float)dXBImgDirec[6] ;
			pXinAoImg->oHeader.imageField.fSliceDirY    = (float)dXBImgDirec[7] ;
			pXinAoImg->oHeader.imageField.fSliceDirZ    = (float)dXBImgDirec[8] ;
			pXinAoImg->oHeader.imageField.fCenterPointX = (float)dXBImgDirec[9] ;
			pXinAoImg->oHeader.imageField.fCenterPointY = (float)dXBImgDirec[10] ;
			pXinAoImg->oHeader.imageField.fCenterPointZ = (float)dXBImgDirec[11] ;
		}
		else
		{
			return;
		}
	}
	//DcmTagKey(0x0028, 0x1050)
	eb = (*imageDataSet)->findAndGetString( WindowCenterTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp, lpTemp, strlen(lpTemp));
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		pXinAoImg->oHeader.imageField.WindowLevel = (short)strtoul(szTemp,NULL,10);
	}
	//DcmTagKey(0x0028, 0x1051)
	eb = (*imageDataSet)->findAndGetString( WindowWidthTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp, lpTemp, strlen(lpTemp));
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		pXinAoImg->oHeader.imageField.WindowWidth = (short)strtoul(szTemp,NULL,10);
	}
	
	const Uint16 *pixelData = NULL;
	unsigned long cnt = 0; 
	//DcmTagKey(0x7fe0, 0x0010)
	if ((*imageDataSet)->findAndGetUint16Array(PixelDataTagKey, pixelData, &cnt).good())
	{ 
		int nImgSize = 2*pXinAoImg->oHeader.seriesField.ResolutionX*pXinAoImg->oHeader.seriesField.ResolutionY ;
		pXinAoImg->oHeader.headInfo.DataMatrixSize = nImgSize ;
		nImageSize   = pXinAoImg->oHeader.seriesField.ResolutionX*pXinAoImg->oHeader.seriesField.ResolutionY ;
		
		pXinAoImg->pData = new WORD[nImageSize];
		if (pXinAoImg->pData != NULL)
		{
			memcpy(pXinAoImg->pData, pixelData, nImageSize*2);					
		}
		else
		{
			return;
		}
	} 
	
	pXinAoImg = pXinAoImg;
}

void CDICOM2XINAO::DCMTKConvertCTDcm2XB(void** dcmDataset,LPCT_STRUCT_IMAGE& pXinAoImg)
{
	DcmDataset **imageDataSet = (DcmDataset**)dcmDataset;
	
	DcmTagKey MediaStorageSOPClassUIDTagKey( DCM_MediaStorageSOPClassUID );				//DcmTagKey(0x0002, 0x0002)
	DcmTagKey TransferSyntaxUIDTagKey( DCM_TransferSyntaxUID );							//DcmTagKey(0x0002, 0x0010)
	DcmTagKey ImageTypeTagKey( DCM_ImageType );											//DcmTagKey(0x0008, 0x0008)
	DcmTagKey SOPClassUIDTagKey( DCM_SOPClassUID );										//DcmTagKey(0x0008, 0x0016)
	DcmTagKey StudyDateTagKey( DCM_StudyDate );											//DcmTagKey(0x0008, 0x0020)
	DcmTagKey SeriesDateTagKey( DCM_SeriesDate );										//DcmTagKey(0x0008, 0x0021)
	DcmTagKey AcquisitionDateTagKey( DCM_AcquisitionDate );								//DcmTagKey(0x0008, 0x0022)
	DcmTagKey StudyTimeTagKey( DCM_StudyTime );											//DcmTagKey(0x0008, 0x0030)
	DcmTagKey SeriesTimeTagKey( DCM_SeriesTime );										//DcmTagKey(0x0008, 0x0031)
	DcmTagKey AcquisitionTimeTagKey( DCM_AcquisitionTime );								//DcmTagKey(0x0008, 0x0032)
	DcmTagKey ModalityTagKey( DCM_Modality );                                           //DcmTagKey(0x0008, 0x0060)
	DcmTagKey ManufacturerTagKey( DCM_Manufacturer);									//DcmTagKey(0x0008, 0x0070)
	DcmTagKey InstitutionNameTagKey( DCM_InstitutionName );								//DcmTagKey(0x0008, 0x0080)
	DcmTagKey ReferringPhysiciansNameTagKey( DCM_ReferringPhysiciansName );				//DcmTagKey(0x0008, 0x0090)
	DcmTagKey StationNameTagKey( DCM_StationName );										//DcmTagKey(0x0008, 0x1010)
	DcmTagKey StudyDescriptionTagKey( DCM_StudyDescription );							//DcmTagKey(0x0008, 0x1030)
	DcmTagKey SeriesDescriptionTagKey( DCM_SeriesDescription );							//DcmTagKey(0x0008, 0x103e)
	DcmTagKey InstitutionalDepartmentNameTagKey( DCM_InstitutionalDepartmentName );     //DcmTagKey(0x0008, 0x1040)
	DcmTagKey ManufacturersModelNameTagKey( DCM_ManufacturersModelName );				//DcmTagKey(0x0008, 0x1090)
	DcmTagKey PatientsNameTagKey( DCM_PatientsName );									//DcmTagKey(0x0010, 0x0010)
	DcmTagKey PatientIDTagKey( DCM_PatientID );											//DcmTagKey(0x0010, 0x0020)
	DcmTagKey PatientsBirthDateTagKey( DCM_PatientsBirthDate );							//DcmTagKey(0x0010, 0x0030)
	DcmTagKey PatientsBirthTimeTagKey( DCM_PatientsBirthTime );							//DcmTagKey(0x0010, 0x0032)
	DcmTagKey PatientsSexTagKey( DCM_PatientsSex );										//DcmTagKey(0x0010, 0x0040)
	DcmTagKey PatientsAgeTagKey( DCM_PatientsAge );										//DcmTagKey(0x0010, 0x1010)
	DcmTagKey PatientsWeightTagKey( DCM_PatientsWeight );								//DcmTagKey(0x0010, 0x1030)
	DcmTagKey AdditionalPatientHistoryTagKey( DCM_AdditionalPatientHistory );			//DcmTagKey(0x0010, 0x21b0)
	DcmTagKey BodyPartExaminedTagKey( DCM_BodyPartExamined );							//DcmTagKey(0x0018, 0x0015)
	DcmTagKey ScanningSequenceTagKey( DCM_ScanningSequence );							//DcmTagKey(0x0018, 0x0020)
	DcmTagKey MRAcquisitionTypeTagKey( DCM_MRAcquisitionType );							//DcmTagKey(0x0018, 0x0023)
	DcmTagKey SequenceNameTagKey( DCM_SequenceName );									//DcmTagKey(0x0018, 0x0024)
	DcmTagKey SliceThicknessTagKey( DCM_SliceThickness );								//DcmTagKey(0x0018, 0x0050)
	DcmTagKey KVPTagKey( DCM_KVP );														//DcmTagKey(0x0018, 0x0060)
	DcmTagKey RepetitionTimeTagKey( DCM_RepetitionTime );								//DcmTagKey(0x0018, 0x0080)
	DcmTagKey EchoTimeTagKey( DCM_EchoTime );											//DcmTagKey(0x0018, 0x0081)
	DcmTagKey InversionTimeTagKey( DCM_InversionTime );									//DcmTagKey(0x0018, 0x0082)
	DcmTagKey NumberOfAveragesTagKey( DCM_NumberOfAverages );							//DcmTagKey(0x0018, 0x0083)
	DcmTagKey ImagingFrequencyTagKey( DCM_ImagingFrequency );							//DcmTagKey(0x0018, 0x0084)
	DcmTagKey ImagedNucleusTagKey( DCM_ImagedNucleus );									//DcmTagKey(0x0018, 0x0085)
	DcmTagKey EchoNumbersTagKey( DCM_EchoNumbers );										//DcmTagKey(0x0018, 0x0086)
	DcmTagKey MagneticFieldStrengthTagKey( DCM_MagneticFieldStrength );					//DcmTagKey(0x0018, 0x0087)
	DcmTagKey SpacingBetweenSlicesTagKey( DCM_SpacingBetweenSlices );					//DcmTagKey(0x0018, 0x0088)
	DcmTagKey NumberOfPhaseEncodingStepsTagKey( DCM_NumberOfPhaseEncodingSteps );       //DcmTagKey(0x0018, 0x0089)
	DcmTagKey DataCollectionDiameter( DCM_DataCollectionDiameter );						//DcmTagKey(0x0018, 0x0090)
	DcmTagKey EchoTrainLengthTagKey( DCM_EchoTrainLength );								//DcmTagKey(0x0018, 0x0091)
	DcmTagKey PixelBandwidthTagKey( DCM_PixelBandwidth );								//DcmTagKey(0x0018, 0x0095)
	DcmTagKey SoftwareVersionsTagKey( DCM_SoftwareVersions );							//DcmTagKey(0x0018, 0x1020)
	DcmTagKey ProtocolNameTagKey( DCM_ProtocolName );									//DcmTagKey(0x0018, 0x1030)
	DcmTagKey TriggerTimeTagKey( DCM_TriggerTime );										//DcmTagKey(0x0018, 0x1060)
	DcmTagKey SkipBeatsTagKey( DCM_SkipBeats );											//DcmTagKey(0x0018, 0x1086)
	DcmTagKey HeartRateTagKey( DCM_HeartRate );											//DcmTagKey(0x0018, 0x1088)
	DcmTagKey CardiacNumberOfImagesTagKey( DCM_CardiacNumberOfImages );					//DcmTagKey(0x0018, 0x1090)
	DcmTagKey ReconstructionDiameterTagKey( DCM_ReconstructionDiameter );				//DcmTagKey(0x0018, 0x1100)
	DcmTagKey GantryDetectorTiltTagKey( DCM_GantryDetectorTilt );						//DcmTagKey(0x0018, 0x1120)
	DcmTagKey TableHeightTagKey( DCM_TableHeight );										//DcmTagKey(0x0018, 0x1130)
	DcmTagKey RotationDirectionTagKey( DCM_RotationDirection );							//DcmTagKey(0x0018, 0x1140)
	DcmTagKey XRayTubeCurrentTagKey( DCM_XRayTubeCurrent );								//DcmTagKey(0x0018, 0x1151)
	DcmTagKey ExposureTagKey( DCM_Exposure );											//DcmTagKey(0x0018, 0x1152)
	DcmTagKey DateOfLastCalibrationTagKey( DCM_DateOfLastCalibration );					//DcmTagKey(0x0018, 0x1200)
	DcmTagKey TimeOfLastCalibrationTagKey( DCM_TimeOfLastCalibration );					//DcmTagKey(0x0018, 0x1201)
	DcmTagKey ReceiveCoilNameTagKey( DCM_ReceiveCoilName );								//DcmTagKey(0x0018, 0x1250)
	DcmTagKey TransmitCoilNameTagKey( DCM_TransmitCoilName );							//DcmTagKey(0x0018, 0x1251)
	DcmTagKey AcquisitionMatrixTagKey( DCM_AcquisitionMatrix );							//DcmTagKey(0x0018, 0x1310)
	DcmTagKey InPlanePhaseEncodingDirectionTagKey( DCM_InPlanePhaseEncodingDirection ); //DcmTagKey(0x0018, 0x1312)
	DcmTagKey FlipAngleTagKey( DCM_FlipAngle );											//DcmTagKey(0x0018, 0x1314)
	DcmTagKey SARTagKey( DCM_SAR );														//DcmTagKey(0x0018, 0x1316)
	DcmTagKey PatientPositionTagKey( DCM_PatientPosition );								//DcmTagKey(0x0018, 0x5100)
	DcmTagKey StudyIDTagKey( DCM_StudyID );												//DcmTagKey(0x0020, 0x0010)
	DcmTagKey SeriesNumberTagKey( DCM_SeriesNumber );									//DcmTagKey(0x0020, 0x0011)
	DcmTagKey InstanceNumberTagKey( DCM_InstanceNumber );								//DcmTagKey(0x0020, 0x0013)
	DcmTagKey ImagePositionPatientTagKey( DCM_ImagePositionPatient );					//DcmTagKey(0x0020, 0x0032)
	DcmTagKey ImageOrientationPatientTagKey( DCM_ImageOrientationPatient );				//DcmTagKey(0x0020, 0x0037)
	DcmTagKey ImagesInAcquisitionTagKey( DCM_ImagesInAcquisition );						//DcmTagKey(0x0020, 0x1002)
	DcmTagKey SliceLocationTagKey( DCM_SliceLocation );									//DcmTagKey(0x0020, 0x1041)
	DcmTagKey RowsTagKey( DCM_Rows );													//DcmTagKey(0x0028, 0x0010)
	DcmTagKey ColumnsTagKey( DCM_Columns );												//DcmTagKey(0x0028, 0x0011)
	DcmTagKey PixelSpacingTagKey( DCM_PixelSpacing );									//DcmTagKey(0x0028, 0x0030)
	DcmTagKey WindowCenterTagKey( DCM_WindowCenter );									//DcmTagKey(0x0028, 0x1050)
	DcmTagKey WindowWidthTagKey( DCM_WindowWidth );										//DcmTagKey(0x0028, 0x1051)
	DcmTagKey RescaleInterceptTagKey( DCM_RescaleIntercept );							//DcmTagKey(0x0028, 0x1052)	
	DcmTagKey RescaleSlopeTagKey( DCM_RescaleSlope );									//DcmTagKey(0x0028, 0x1053)
	DcmTagKey PixelDataTagKey( DCM_PixelData );											//DcmTagKey(0x7fe0, 0x0010)
	
	char *lpTemp = NULL;
	char			szTemp[64] ;
	int				count = 0 ;
	int				i     = 0 ;
	double			dVal[20] ;
	double			dImgDirec[6] ;
	double			dXBImgDirec[12] ;
	double			dOffSet[3] ;
	double			dSliceThick = 5.0;
	double          dSliceLocation = 0.0;
	short			nRescaleIntercept=0;
	short			nRescaleSlope=1;
	int				nImageSize = 1 ;
	int				nVM;
	int				nModality = -1;
	
	pXinAoImg = new CT_STRUCT_IMAGE;				
	memset(&(pXinAoImg->oHeader.headInfo), 0 ,sizeof(pXinAoImg->oHeader) );
	// Initialize
	pXinAoImg->oHeader.headInfo.BITitle[0] = 'D';
	pXinAoImg->oHeader.headInfo.BITitle[1] = 'C';
	pXinAoImg->oHeader.headInfo.BITitle[2] = 'M';
	pXinAoImg->oHeader.headInfo.BITitle[3] = 'C';
	pXinAoImg->oHeader.headInfo.BITitle[4] = 'T';
	pXinAoImg->oHeader.headInfo.Version = int (1) << 16 + 0;
	
	//DcmTagKey(0x0008, 0x0020)
	OFCondition eb = (*imageDataSet)->findAndGetString( StudyDateTagKey, lpTemp, OFFalse );   
	if(eb == EC_Normal && lpTemp != NULL)
		TimeConvert2(pXinAoImg->oHeader.studyField.Time, lpTemp, 1);
	//DcmTagKey(0x0008, 0x0021)
	eb = (*imageDataSet)->findAndGetString( SeriesDateTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
		TimeConvert2(pXinAoImg->oHeader.seriesField.DateTime, lpTemp, 1);
	//DcmTagKey(0x0008, 0x0022)
	eb = (*imageDataSet)->findAndGetString( AcquisitionDateTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
		TimeConvert2(pXinAoImg->oHeader.imageField.DateTime, lpTemp, 1);
	//DcmTagKey(0x0008, 0x0030)
	eb = (*imageDataSet)->findAndGetString( StudyTimeTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)	
		TimeConvert2(pXinAoImg->oHeader.studyField.Time, lpTemp, 2);
	//DcmTagKey(0x0008, 0x0031)
	eb = (*imageDataSet)->findAndGetString( SeriesTimeTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
		TimeConvert2(pXinAoImg->oHeader.seriesField.DateTime, lpTemp, 2);
	//DcmTagKey(0x0008, 0x0032)
	eb = (*imageDataSet)->findAndGetString( AcquisitionTimeTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
		TimeConvert2(pXinAoImg->oHeader.imageField.DateTime, lpTemp, 2);
	//DcmTagKey(0x0008, 0x0070)
	eb = (*imageDataSet)->findAndGetString( ManufacturerTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(pXinAoImg->oHeader.siteField.Manufacturer, lpTemp, strlen(lpTemp));
		if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
		{
			pXinAoImg->oHeader.siteField.Manufacturer[strlen(lpTemp)-1] = 0x00;
		}
	}
	//DcmTagKey(0x0008, 0x0080)
	eb = (*imageDataSet)->findAndGetString( InstitutionNameTagKey, lpTemp, OFFalse );  
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(pXinAoImg->oHeader.siteField.InstName, lpTemp, strlen(lpTemp));
		if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
		{
			pXinAoImg->oHeader.siteField.InstName[strlen(lpTemp)-1] = 0x00;
		}
	}
	//DcmTagKey(0x0008, 0x1010)
	eb = (*imageDataSet)->findAndGetString( StationNameTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(pXinAoImg->oHeader.siteField.StationName,lpTemp,strlen(lpTemp));
		if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
		{
			pXinAoImg->oHeader.siteField.StationName[strlen(lpTemp)-1] = 0x00;
		}
	}
	//DcmTagKey(0x0008, 0x1030)
	eb = (*imageDataSet)->findAndGetString( StudyDescriptionTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(pXinAoImg->oHeader.studyField.Desc, lpTemp, strlen(lpTemp));
		if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
		{
			pXinAoImg->oHeader.studyField.Desc[strlen(lpTemp)-1] = 0x00;
		}
	}
	//DcmTagKey(0x0008, 0x103e)
	eb = (*imageDataSet)->findAndGetString( SeriesDescriptionTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(pXinAoImg->oHeader.seriesField.Desc, lpTemp, strlen(lpTemp));
		if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
		{
			pXinAoImg->oHeader.seriesField.Desc[strlen(lpTemp)-1] = 0x00;
		}
	}
	//DcmTagKey(0x0008, 0x1040)
	eb = (*imageDataSet)->findAndGetString( InstitutionalDepartmentNameTagKey,lpTemp,OFFalse);
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(pXinAoImg->oHeader.siteField.InstDeptName, lpTemp, strlen(lpTemp));
		if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
		{
			pXinAoImg->oHeader.siteField.InstDeptName[strlen(lpTemp)-1] = 0x00;
		}
	}
	//DcmTagKey(0x0008, 0x1090)
	eb = (*imageDataSet)->findAndGetString( ManufacturersModelNameTagKey,lpTemp,OFFalse);
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(pXinAoImg->oHeader.siteField.BISerialNum, lpTemp, strlen(lpTemp));
		if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
		{
			pXinAoImg->oHeader.siteField.BISerialNum[strlen(lpTemp)-1] = 0x00;
		}
	}
	//DcmTagKey(0x0010, 0x0010)
	eb = (*imageDataSet)->findAndGetString( PatientsNameTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		NameConvert2(pXinAoImg->oHeader.patientField.Name, lpTemp, strlen(lpTemp));
	}		
	//DcmTagKey(0x0010, 0x0020)
	eb = (*imageDataSet)->findAndGetString( PatientIDTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(pXinAoImg->oHeader.patientField.Id, lpTemp, strlen(lpTemp));
		if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
		{
			pXinAoImg->oHeader.patientField.Id[strlen(lpTemp)-1]=0x00;
		}
	}
	//DcmTagKey(0x0010, 0x0030)
	eb = (*imageDataSet)->findAndGetString( PatientsBirthDateTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
		TimeConvert2(pXinAoImg->oHeader.patientField.BirthDate, lpTemp, 1);
	
	//DcmTagKey(0x0010, 0x0032)
	eb = (*imageDataSet)->findAndGetString( PatientsBirthTimeTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
		TimeConvert2(pXinAoImg->oHeader.patientField.BirthDate, lpTemp, 2);
	//DcmTagKey(0x0010, 0x0040)
	eb = (*imageDataSet)->findAndGetString( PatientsSexTagKey, lpTemp, OFFalse );  
	if(eb == EC_Normal)
	{
		if(lpTemp == NULL)
		{
			pXinAoImg->oHeader.patientField.Sex = 'U';
		}
		else
		{
			pXinAoImg->oHeader.patientField.Sex = *lpTemp;
		}
	}
	//DcmTagKey(0x0010, 0x1010)
	eb = (*imageDataSet)->findAndGetString( PatientsAgeTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
		pXinAoImg->oHeader.patientField.Age = AgeConvert2(lpTemp);
	//DcmTagKey(0x0010, 0x1030)
	eb = (*imageDataSet)->findAndGetString( PatientsWeightTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp, lpTemp, strlen(lpTemp));
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		pXinAoImg->oHeader.patientField.Weight = atoi(szTemp);
	}
	//DcmTagKey(0x0010, 0x21b0)
	eb = (*imageDataSet)->findAndGetString( AdditionalPatientHistoryTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(pXinAoImg->oHeader.patientField.Desc,lpTemp,strlen(lpTemp));
		if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
		{
			pXinAoImg->oHeader.patientField.Desc[strlen(lpTemp)-1] = 0x00;
		}
	}
	//DcmTagKey(0x0018, 0x0050)			
	eb = (*imageDataSet)->findAndGetString( SliceThicknessTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp, lpTemp, strlen(lpTemp));
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		pXinAoImg->oHeader.seriesField.SliceThickness = (float)atof(szTemp);
		dSliceThick = pXinAoImg->oHeader.seriesField.SliceThickness ;
	}
	//DcmTagKey(0x0018, 0x0060)			
	eb = (*imageDataSet)->findAndGetString( KVPTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp, lpTemp, strlen(lpTemp));
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		pXinAoImg->oHeader.seriesField.KVP = (unsigned short)atoi(szTemp);
	}
	//DcmTagKey(0x0018, 0x0088)
	eb = (*imageDataSet)->findAndGetString( SpacingBetweenSlicesTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp, lpTemp ,strlen(lpTemp));
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		pXinAoImg->oHeader.seriesField.SliceGap  = (float)strtod(szTemp,NULL);
		pXinAoImg->oHeader.seriesField.SliceGap -= (float)dSliceThick ;
	}
	//DcmTagKey(0x0018, 0x0090)
	eb = (*imageDataSet)->findAndGetString( DataCollectionDiameter, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp, lpTemp, strlen(lpTemp));
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		pXinAoImg->oHeader.seriesField.CollectDiameter = (unsigned short)atoi(szTemp);
	}
	//DcmTagKey(0x0018, 0x1020)
	eb = (*imageDataSet)->findAndGetString( SoftwareVersionsTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(pXinAoImg->oHeader.siteField.SoftVer, lpTemp, strlen(lpTemp));
		if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
		{
			pXinAoImg->oHeader.seriesField.Protocol[strlen(lpTemp)-1] = 0x00;
		}
	}
	//DcmTagKey(0x0018, 0x1030)
	eb = (*imageDataSet)->findAndGetString( ProtocolNameTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(pXinAoImg->oHeader.seriesField.Protocol, lpTemp, strlen(lpTemp));
		if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
		{
			pXinAoImg->oHeader.seriesField.Protocol[strlen(lpTemp)-1] = 0x00;
		}
	}
	
	eb = (*imageDataSet)->findAndGetString( CardiacNumberOfImagesTagKey, lpTemp, OFFalse );
	
	//DcmTagKey(0x0018, 0x1100)
	eb = (*imageDataSet)->findAndGetString( ReconstructionDiameterTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp, lpTemp, strlen(lpTemp));
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		pXinAoImg->oHeader.seriesField.DataDiameter = (unsigned short)atoi(szTemp);
	}
	//DcmTagKey(0x0018, 0x1120)
	eb = (*imageDataSet)->findAndGetString( GantryDetectorTiltTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp, lpTemp, strlen(lpTemp));
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		pXinAoImg->oHeader.seriesField.DetectorTilt = (unsigned short)atoi(szTemp);
	}
	//DcmTagKey(0x0018, 0x1130)
	eb = (*imageDataSet)->findAndGetString( TableHeightTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp, lpTemp, strlen(lpTemp));
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		pXinAoImg->oHeader.seriesField.TableHeight = (short)atoi(szTemp);
	}
	//DcmTagKey(0x0018, 0x1140)
	eb = (*imageDataSet)->findAndGetString( RotationDirectionTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
	{
		if (strncmp(lpTemp, "CW", 2) == 0)
		{
			pXinAoImg->oHeader.seriesField.RotateDir = 1 ;
		}
		else if (strncmp(lpTemp, "CC", 2) == 0)
		{
			pXinAoImg->oHeader.seriesField.RotateDir = 2 ;
		}
		else
		{
			pXinAoImg->oHeader.seriesField.RotateDir = 0 ;
		}
	}
	//DcmTagKey(0x0018, 0x1151)
	eb = (*imageDataSet)->findAndGetString( XRayTubeCurrentTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp,lpTemp,strlen(lpTemp));
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		pXinAoImg->oHeader.seriesField.TubeCurrent = (unsigned short)atoi(szTemp);
	}
	//DcmTagKey(0x0018, 0x1152)
	eb = (*imageDataSet)->findAndGetString( ExposureTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp,lpTemp,strlen(lpTemp));
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		pXinAoImg->oHeader.seriesField.Exposure = (unsigned short)atoi(szTemp);
	}
	//DcmTagKey(0x0018, 0x1200)
	eb = (*imageDataSet)->findAndGetString( DateOfLastCalibrationTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
		TimeConvert2(pXinAoImg->oHeader.siteField.LastCalib,lpTemp,1);
	//DcmTagKey(0x0018, 0x1201)
	eb = (*imageDataSet)->findAndGetString( TimeOfLastCalibrationTagKey, lpTemp, OFFalse );  
	if(eb == EC_Normal && lpTemp != NULL)
		TimeConvert2(pXinAoImg->oHeader.siteField.LastCalib,lpTemp,2);
	//DcmTagKey(0x0018, 0x5100)
	eb = (*imageDataSet)->findAndGetString( PatientPositionTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		if(strncmp(lpTemp,"HFS ",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.studyField.PatEntry    = beHeadFirst;
			pXinAoImg->oHeader.studyField.PatPosition = bpSupine;
		}
		else if(strncmp(lpTemp,"HFP ",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.studyField.PatEntry    = beHeadFirst;
			pXinAoImg->oHeader.studyField.PatPosition = bpProne;
		}
		else if(strncmp(lpTemp,"HFDR",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.studyField.PatEntry    = beHeadFirst;
			pXinAoImg->oHeader.studyField.PatPosition = bpDecubitusRight;
		}
		else if(strncmp(lpTemp,"HFDL",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.studyField.PatEntry    = beHeadFirst;
			pXinAoImg->oHeader.studyField.PatPosition = bpDecubitusLeft;
		}
		else if(strncmp(lpTemp,"FFS ",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.studyField.PatEntry    = beFootFirst;
			pXinAoImg->oHeader.studyField.PatPosition = bpSupine;
		}
		else if(strncmp(lpTemp,"FFP ",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.studyField.PatEntry    = beFootFirst;
			pXinAoImg->oHeader.studyField.PatPosition = bpProne;
		}
		else if(strncmp(lpTemp,"FFDR",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.studyField.PatEntry    = beFootFirst;
			pXinAoImg->oHeader.studyField.PatPosition = bpDecubitusRight;
		}
		else if(strncmp(lpTemp,"FFDL",strlen(lpTemp)) == 0)
		{
			pXinAoImg->oHeader.studyField.PatEntry    = beFootFirst;
			pXinAoImg->oHeader.studyField.PatPosition = bpDecubitusLeft;
		}
	}
	//DcmTagKey(0x0020, 0x0010)
	eb = (*imageDataSet)->findAndGetString( StudyIDTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(pXinAoImg->oHeader.studyField.Id, lpTemp, strlen(lpTemp));
		if(*(lpTemp+strlen(lpTemp)-1) == ' ')
		{
			pXinAoImg->oHeader.studyField.Id[strlen(lpTemp)-1]=0x00;
		}
	}
	//DcmTagKey(0x0020, 0x0011)
	eb = (*imageDataSet)->findAndGetString( SeriesNumberTagKey, lpTemp, OFFalse );  
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp, lpTemp, strlen(lpTemp));
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		pXinAoImg->oHeader.seriesField.Id = (BYTE)strtoul(szTemp,NULL,10);
	}
	//DcmTagKey(0x0020, 0x0013)
	eb = (*imageDataSet)->findAndGetString( InstanceNumberTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp, lpTemp, strlen(lpTemp));
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		pXinAoImg->oHeader.imageField.Id = (unsigned short)strtoul(szTemp,NULL,10);
	}
	//DcmTagKey(0x0020, 0x0032)
	eb = (*imageDataSet)->findAndGetString( ImagePositionPatientTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
	{
		GetDSVM(lpTemp, strlen(lpTemp), dVal, nVM);
		if (nVM != 3)
		{
			return ;
		}
		dOffSet[0] = dVal[0] ;
		dOffSet[1] = dVal[1] ;
		dOffSet[2] = dVal[2] ;
	}
	//DcmTagKey(0x0020, 0x0037)
	eb = (*imageDataSet)->findAndGetString( ImageOrientationPatientTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		GetDSVM(lpTemp, strlen(lpTemp), dVal, nVM);
		if (nVM != 6)
		{
			return ;
		}
		else
		{
			dImgDirec[0] = dVal[0] ;
			dImgDirec[1] = dVal[1] ;
			dImgDirec[2] = dVal[2] ;
			dImgDirec[3] = dVal[3] ;
			dImgDirec[4] = dVal[4] ;
			dImgDirec[5] = dVal[5] ;
		}
	}
	//DcmTagKey(0x0020, 0x1002)
	eb = (*imageDataSet)->findAndGetString( ImagesInAcquisitionTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp, lpTemp, strlen(lpTemp));
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		pXinAoImg->oHeader.seriesField.ResolutionZ = (unsigned short)strtoul(szTemp,NULL,10);
	}
	//DcmTagKey(0x0020, 0x1041)
	eb = (*imageDataSet)->findAndGetString( SliceLocationTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp, lpTemp, strlen(lpTemp));
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		dSliceLocation = atof(szTemp);
		//pXinAoImg->oHeader.seriesField.ResolutionZ = (unsigned short)strtoul(szTemp,NULL,10);
	}
	//DcmTagKey(0x0028, 0x0010)
	Uint16 RowsValue;
	UINT16 ColumnsValue;
	eb = (*imageDataSet)->findAndGetUint16( RowsTagKey, RowsValue, OFFalse );
	if(eb == EC_Normal)
		pXinAoImg->oHeader.seriesField.ResolutionX = RowsValue;
	//DcmTagKey(0x0028, 0x0011)
	eb = (*imageDataSet)->findAndGetUint16( ColumnsTagKey, ColumnsValue, OFFalse );
	if(eb == EC_Normal)
		pXinAoImg->oHeader.seriesField.ResolutionY = ColumnsValue ;
	//DcmTagKey(0x0028, 0x0030)
	eb = (*imageDataSet)->findAndGetString( PixelSpacingTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
	{
		GetDSVM(lpTemp, strlen(lpTemp), dVal, nVM);
		if (nVM == 2)
		{
			pXinAoImg->oHeader.seriesField.FovX = 
				(unsigned short)((double)pXinAoImg->oHeader.seriesField.ResolutionX*dVal[0]) ;
			pXinAoImg->oHeader.seriesField.FovY = 
				(unsigned short)((double)pXinAoImg->oHeader.seriesField.ResolutionY*dVal[1]) ;
			CalcBIImgDirec(dImgDirec, dOffSet ,
				pXinAoImg->oHeader.studyField.PatPosition, 
				pXinAoImg->oHeader.studyField.PatEntry, 
				pXinAoImg->oHeader.seriesField.FovX,
				pXinAoImg->oHeader.seriesField.FovY,
				dXBImgDirec) ;
			pXinAoImg->oHeader.scanField.FreqDirX  = (float)dXBImgDirec[0] ;
			pXinAoImg->oHeader.scanField.FreqDirY  = (float)dXBImgDirec[1] ;
			pXinAoImg->oHeader.scanField.FreqDirZ  = (float)dXBImgDirec[2] ;
			pXinAoImg->oHeader.scanField.PhaseDirX = (float)dXBImgDirec[3] ;
			pXinAoImg->oHeader.scanField.PhaseDirY = (float)dXBImgDirec[4] ;
			pXinAoImg->oHeader.scanField.PhaseDirZ = (float)dXBImgDirec[5] ;
			pXinAoImg->oHeader.scanField.SliceDirX = (float)dXBImgDirec[6] ;
			pXinAoImg->oHeader.scanField.SliceDirY = (float)dXBImgDirec[7] ;
			pXinAoImg->oHeader.scanField.SliceDirZ = (float)dXBImgDirec[8] ;
			
			pXinAoImg->oHeader.imageField.fCenterPointX = (float)dXBImgDirec[9] ;
			pXinAoImg->oHeader.imageField.fCenterPointY = (float)dXBImgDirec[10] ;
			pXinAoImg->oHeader.imageField.fCenterPointZ = (float)dXBImgDirec[11] ;
			
			if(0)//pDICOMSrv->m_bToshibaCTTable
			{
				pXinAoImg->oHeader.imageField.fCenterPointY = (float)dSliceLocation;
			}
		}
		else
		{
			return;
		}
	}
	//DcmTagKey(0x0028, 0x1050)
	eb = (*imageDataSet)->findAndGetString( WindowCenterTagKey, lpTemp, OFFalse ); 
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp, lpTemp, strlen(lpTemp));
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		//CT图像窗位需要加上1024
		pXinAoImg->oHeader.imageField.WindowLevel = 1024 + (short)strtoul(szTemp,NULL,10);
	}
	//DcmTagKey(0x0028, 0x1051)
	eb = (*imageDataSet)->findAndGetString( WindowWidthTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp, lpTemp, strlen(lpTemp));
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		pXinAoImg->oHeader.imageField.WindowWidth = (short)strtoul(szTemp,NULL,10);
	}
	//DcmTagKey(0x0028, 0x1052)
	eb = (*imageDataSet)->findAndGetString( RescaleInterceptTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp, lpTemp, strlen(lpTemp));
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		nRescaleIntercept = (short)strtoul(szTemp,NULL,10);
	}
	//DcmTagKey(0x0028, 0x1053)
	eb = (*imageDataSet)->findAndGetString( RescaleSlopeTagKey, lpTemp, OFFalse );
	if(eb == EC_Normal && lpTemp != NULL)
	{
		strncpy(szTemp, lpTemp, strlen(lpTemp));
		if(szTemp[strlen(lpTemp) - 1] == ' ')
		{
			szTemp[strlen(lpTemp)-1] = 0x00 ;
		}
		else
		{
			szTemp[strlen(lpTemp)] = 0x00 ;
		}
		nRescaleSlope = (short)strtoul(szTemp,NULL,10);
	}
	
	const Uint16 *pixelData = NULL;
	unsigned long cnt = 0; 
	//DcmTagKey(0x7fe0, 0x0010)
	if ((*imageDataSet)->findAndGetUint16Array(PixelDataTagKey, pixelData, &cnt).good())
	{ 
		int nImgSize = 2*pXinAoImg->oHeader.seriesField.ResolutionX*pXinAoImg->oHeader.seriesField.ResolutionY ;
		nImageSize   = pXinAoImg->oHeader.seriesField.ResolutionX*pXinAoImg->oHeader.seriesField.ResolutionY ;
		
		pXinAoImg->pData = new WORD[nImageSize];
		if (pXinAoImg->pData != NULL)
		{
			memcpy(pXinAoImg->pData, pixelData, nImageSize*2);
			if(nRescaleIntercept!=-1024)
			{
				for(int ii=0;ii<nImageSize;ii++)
				{
					pXinAoImg->pData[ii]+=(1024+nRescaleIntercept);
				}
			}
		}
		else
		{
			return;
		}				
	} 
	
	pXinAoImg = pXinAoImg;
}

void CDICOM2XINAO::TimeConvert(MR_BIDateTime datetime,char date[8],char time[6])
{
	char year[4],mon[2],day[2],hour[2],min[2],sec[2];
	_ultoa((int)datetime.year,year,10);
	_ultoa((int)datetime.month,mon,10);
	if(datetime.month<10)
	{
		mon[1]=mon[0];
		mon[0]='0';
	}
	_ultoa((int)datetime.day,day,10);
	if(datetime.day<10)
	{
		day[1]=day[0];
		day[0]='0';
	}
	_ultoa((int)datetime.hour,hour,10);
	if(datetime.hour<10)
	{
		hour[1]=hour[0];
		hour[0]='0';
	}
	_ultoa((int)datetime.minute,min,10);
	if(datetime.minute<10)
	{
		min[1]=min[0];
		min[0]='0';
	}
	_ultoa((int)datetime.second,sec,10);
	if(datetime.second<10)
	{
		sec[1]=sec[0];
		sec[0]='0';
	}
	for(int i=0;i<8;i++)
	{
		if(i<4)
		{
			date[i]=year[i];
		}
		else if(i<6&&i>=4)
		{
			date[i]=mon[i-4];
		}
		else if(i<8&&i>=6)
		{
			date[i]=day[i-6];
		}
	}
	for(i=0;i<6;i++)
	{
		if(i<2)
		{
			time[i]=hour[i];
		}
		else if(i<4&&i>=2)
		{
			time[i]=min[i-2];
		}
		else if(i<6&&i>=4)
		{
			time[i]=sec[i-4];
		}
	}
	
}

void CDICOM2XINAO::TimeConvert2(MR_BIDateTime &datetime,const char *p,int flag)
{
	if(flag==2)//TM
	{
		char hour[2],min[2],sec[2];
		strncpy(hour,p,2);
		datetime.hour=atoi(hour);
		strncpy(min,p+2,2);
		datetime.minute=atoi(min);
		strncpy(sec,p+4,2);
		datetime.second=atoi(sec);
	}
	else if(flag==1)//DA
	{
		char year[4],mon[2],day[2];
		strncpy(year,p,4);
		datetime.year=atoi(year);
		strncpy(mon,p+4,2);
		datetime.month=atoi(mon);
		strncpy(day,p+6,2);
		datetime.day=atoi(day);
	}
}

void CDICOM2XINAO::TimeConvert2(CT_BIDateTime &datetime,const char *p,int flag)
{
	if(flag==2)//TM
	{
		char hour[2],min[2],sec[2];
		strncpy(hour,p,2);
		datetime.hour=atoi(hour);
		strncpy(min,p+2,2);
		datetime.minute=atoi(min);
		strncpy(sec,p+4,2);
		datetime.second=atoi(sec);
	}
	else if(flag==1)//DA
	{
		char year[4],mon[2],day[2];
		strncpy(year,p,4);
		datetime.year=atoi(year);
		strncpy(mon,p+4,2);
		datetime.month=atoi(mon);
		strncpy(day,p+6,2);
		datetime.day=atoi(day);
	}
}

void CDICOM2XINAO::NameConvert2(char BIname[], char DIname[], int vlength)
{
	int i=0;
	while(i<vlength&&i<31)
	{
		while(DIname[i]!='^'&&i<vlength&&i<31)
		{
			BIname[i]=DIname[i];
			i++;
		}
		if(i<vlength&&i<32)
		{
			BIname[i]=' ';
			i++;
		}
	}
	BIname[i]=0;
	if(BIname[vlength-1]==' ')
		BIname[vlength-1]=0x00;


}

unsigned char CDICOM2XINAO::AgeConvert2(char DIage[])
{
	char age[3];
	strncpy(age,DIage,3);
	return atoi(age);
}

unsigned short CDICOM2XINAO::SwapByte(unsigned short sPoint)
{
	unsigned short sPointUs ;
	BYTE           iP1, iP2 ;
    iP1 = sPoint&0x00FF ;
	iP2 = sPoint>>8 ;
	sPointUs = iP2 + (iP1<<8) ;
	return  sPointUs ;
}

int  CDICOM2XINAO::GetSeriesOption(char szSeriesName[])
{
	int nSeriesOption = 0 ;
	if (strncmp(szSeriesName, "SE", 2) == 0)
	{
		nSeriesOption = cnstSE ;
	}
	else if (strncmp(szSeriesName, "SEDWI", 5) == 0)
	{
		nSeriesOption = cnstSEDWI ;
	}
	else if (strncmp(szSeriesName, "IRSE", 4) == 0)
	{
		nSeriesOption = cnstIRSE ;
	}
	else if (strncmp(szSeriesName, "SE\\IR", 5) == 0)
	{
		nSeriesOption = cnstSTIR ;
	}
	else if (strncmp(szSeriesName, "IR", 2) == 0)
	{
		nSeriesOption = cnstFLAIR ;
	}
	else if (strncmp(szSeriesName, "GR", 2) == 0)
	{
		nSeriesOption = cnstGRE ;
	}
	else if (strncmp(szSeriesName, "LSDI", 4) == 0)
	{
		nSeriesOption = cnstLSDI ;
	}
	else
	{
		nSeriesOption = cnstTOF3D ;
	}
	return nSeriesOption ;
}

void CDICOM2XINAO::GetDSVM(char* lpTemp, int nVLength, double dVal[], int& nVM)
{
	char   *pszTemp ; 
	CString csT1, csT2 ;
	int     nPos = 0;
	int     nLen ;

	pszTemp = new char[nVLength + 1] ;
	memcpy(pszTemp, lpTemp, nVLength);
	nVM = 0;
	if (pszTemp[nVLength-1] == ' ')
	{
		pszTemp[nVLength-1] = 0 ;
	}
	else
	{
		pszTemp[nVLength] = 0 ;
	}

	csT1.Format("%s",pszTemp);
	nLen = csT1.GetLength();
	nPos = csT1.Find("\\" ,0);
	while (nPos > 0)
	{
		csT2 = csT1.Left(nPos);
		dVal[nVM] = atof(csT2.GetBuffer(30));
		csT1 = csT1.Right(nLen - nPos -1);
		nLen = csT1.GetLength();
		nPos = csT1.Find("\\" ,0);
		nVM++;
	}
	dVal[nVM++] = atof(csT1.GetBuffer(30));
	delete[] pszTemp ;
}

void CDICOM2XINAO::CalcBIImgDirec(double dDirCos[6], double dTopLeft[3] ,
									 BYTE iPatPos, BYTE iPatEntry,
									 unsigned short sFovX, unsigned short sFovY,
									 double dBIDirec[])
{
	double  dImgCenterX , dImgCenterY , dImgCenterZ ;
	dImgCenterX = dTopLeft[0] + ((double)sFovX*dDirCos[0] + (double)sFovY*dDirCos[3])/2.0 ;
	dImgCenterY = dTopLeft[1] + ((double)sFovX*dDirCos[1] + (double)sFovY*dDirCos[4])/2.0 ;
	dImgCenterZ = dTopLeft[2] + ((double)sFovX*dDirCos[2] + (double)sFovY*dDirCos[5])/2.0 ;

	if (iPatEntry == beHeadFirst)
	{
		switch (iPatPos)
		{
		case bpProne:
			dBIDirec[0] = -dDirCos[0] ;   //Read-x
			dBIDirec[1] = dDirCos[2] ;    //Read-y
			dBIDirec[2] = dDirCos[1] ;    //Read-z
			dBIDirec[3] = -dDirCos[3] ;   //phase-x
			dBIDirec[4] = dDirCos[5] ;    //phase-y
			dBIDirec[5] = dDirCos[4] ;    //phase-z

			dBIDirec[9]  = -dImgCenterX ;
			dBIDirec[10] =  dImgCenterZ ;
			dBIDirec[11] =  dImgCenterY ;
			break ;
		case bpDecubitusRight:
			dBIDirec[0] = dDirCos[1] ;
			dBIDirec[1] = dDirCos[2] ;
			dBIDirec[2] = dDirCos[0] ;
			dBIDirec[3] = dDirCos[4] ;
			dBIDirec[4] = dDirCos[5] ;
			dBIDirec[5] = dDirCos[3] ;

			dBIDirec[9]  = dImgCenterY ;
			dBIDirec[10] = dImgCenterZ ;
			dBIDirec[11] = dImgCenterX ;
			break ;
		case bpDecubitusLeft:
			dBIDirec[0] = -dDirCos[1] ;
			dBIDirec[1] = dDirCos[2] ;
			dBIDirec[2] = -dDirCos[0] ;
			dBIDirec[3] = -dDirCos[4] ;
			dBIDirec[4] = dDirCos[5] ;
			dBIDirec[5] = -dDirCos[3] ;

			dBIDirec[9]  = -dImgCenterY ;
			dBIDirec[10] =  dImgCenterZ ;
			dBIDirec[11] = -dImgCenterX ;
			break ;
		case bpSupine:
		default:
			dBIDirec[0] = dDirCos[0] ;
			dBIDirec[1] = dDirCos[2] ;
			dBIDirec[2] = -dDirCos[1] ;
			dBIDirec[3] = dDirCos[3] ;
			dBIDirec[4] = dDirCos[5] ;
			dBIDirec[5] = -dDirCos[4] ;

			dBIDirec[9]  =  dImgCenterX ;
			dBIDirec[10] =  dImgCenterZ ;
			dBIDirec[11] = -dImgCenterY ;
			break ;
		}
	}
	else
	{
		switch (iPatPos)
		{
		case bpProne:
			dBIDirec[0] =  dDirCos[0] ;     //Read-x
			dBIDirec[1] = -dDirCos[2] ;     //Read-y
			dBIDirec[2] =  dDirCos[1] ;     //Read-z
			dBIDirec[3] =  dDirCos[3] ;     //phase-x
			dBIDirec[4] = -dDirCos[5] ;     //phase-y
			dBIDirec[5] =  dDirCos[4] ;     //phase-z

			dBIDirec[9]  =  dImgCenterX ;
			dBIDirec[10] = -dImgCenterZ ;
			dBIDirec[11] =  dImgCenterY ;
			break ;
		case bpDecubitusRight:
			dBIDirec[0] = -dDirCos[1] ;
			dBIDirec[1] = -dDirCos[2] ;
			dBIDirec[2] =  dDirCos[0] ;
			dBIDirec[3] = -dDirCos[4] ;
			dBIDirec[4] = -dDirCos[5] ;
			dBIDirec[5] =  dDirCos[3] ;

			dBIDirec[9]  = -dImgCenterY ;
			dBIDirec[10] = -dImgCenterZ ;
			dBIDirec[11] =  dImgCenterX ;
			break ;
		case bpDecubitusLeft:
			dBIDirec[0] =  dDirCos[1] ;
			dBIDirec[1] = -dDirCos[2] ;
			dBIDirec[2] = -dDirCos[0] ;
			dBIDirec[3] =  dDirCos[4] ;
			dBIDirec[4] = -dDirCos[5] ;
			dBIDirec[5] = -dDirCos[3] ;

			dBIDirec[9]  =  dImgCenterY ;
			dBIDirec[10] = -dImgCenterZ ;
			dBIDirec[11] = -dImgCenterX ;
			break ;
		case bpSupine:
		default:
			dBIDirec[0] = -dDirCos[0] ;
			dBIDirec[1] = -dDirCos[2] ;
			dBIDirec[2] = -dDirCos[1] ;
			dBIDirec[3] = -dDirCos[3] ;
			dBIDirec[4] = -dDirCos[5] ;
			dBIDirec[5] = -dDirCos[4] ;

			dBIDirec[9]  = -dImgCenterX ;
			dBIDirec[10] = -dImgCenterZ ;
			dBIDirec[11] = -dImgCenterY ;
			break ;
		}
	}
    dBIDirec[6] = dBIDirec[1]*dBIDirec[5] - dBIDirec[2]*dBIDirec[4] ;    //slice-x
	dBIDirec[7] = dBIDirec[2]*dBIDirec[3] - dBIDirec[0]*dBIDirec[5] ;    //slice-y
	dBIDirec[8] = dBIDirec[0]*dBIDirec[4] - dBIDirec[1]*dBIDirec[3] ;    //slice-z
}
