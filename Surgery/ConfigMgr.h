#ifndef _CCONFIGMGR_H
#define _CCONFIGMGR_H

#define  SYSINFO	    "System"
#define  SYS_DEVICE_TYPE "Device Type"
#define  SYS_HAS_CAMERA	"Camera"
#define  SYS_HAS_MRI	"MRI"
#define  SYS_HAS_PROBE	"Probe"

#define  SERVER_IP_1    "IP_1"
#define  SERVER_IP_2    "IP_2"
#define  SERVER_IP_3    "IP_3"
#define  SERVER_IP_4    "IP_4"
#define  SERVER_PORT	"MRI Server Port"


#define  SYS_NAVI_MODE "Navigation Mode"
#define  SYS_AUTO_TABLE_SHIFT "Auto Shift Table"
#define  SYS_LAST_MODALITY		"Last Modality ID"
#define  SYS_MANUAL_CALI	"Manual Calibrate"
#define	 SYS_OPEN_DIALOG_INIT_FOLDER	"Open dialog init folder"

#define CONF_DICOM "DICOM"
#define CONF_DICOM_MODE "Mode"
#define CONF_DICOM_ENABLE  "Enable DICOM"
#define CONF_DICOM_PORT  "Port"
#define CONF_DICOM_TOSHIBA_TABLE "IsToshibaTable"  //added by ningsj 2009/6/10

#define GRADIENT "GRADIENT_CORRECTION"
#define GRADIENT_ENABLE "Enable"  //added by ningsj 2010/6/10
#define GRADIENT_REFDATA "RefDataFile"  

#define  COM			"COM"
#define  COM_BAUD_RATE	"Baud Rate"
#define  COM_STOP_BITS	"Stop Bits"
#define  COM_PARITY		"Parity"
#define  COM_DATA_BITS	"Data Bits"
#define  COM_HARDWARE	"Hardware Handshaking"
#define  COM_PORT		"Port"
#define  COM_RESET		"Rest Hardware"
#define  COM_WIRELESS   "Wireless"
#define  COM_GETTXIGNOREERROR	"Get TX ignore error"


#define  CONF_SET_DISPLAY   "Display setting"
#define  CONF_SET_DISPLAY_SHOW_CONTROL   "Show control panel"
#define  CONF_SET_DISPLAY_MODE		"Window mode"
#define  CONF_SET_DISPLAY_LAST_MODE "Window last mode"
#define  CONF_SET_DISPLAY_MR_LEFTTOP "MR LeftTop"
#define  CONF_SET_DISPLAY_MR_LEFTBOTTOM "MR LeftBottom"
#define  CONF_SET_DISPLAY_MR_RIGHTTOP "MR RightTop"
#define  CONF_SET_DISPLAY_MR_RIGHTBOTTOM "MR RightBottom"
#define  CONF_SET_DISPLAY_CT_LEFTTOP "CT LeftTop"
#define  CONF_SET_DISPLAY_CT_LEFTBOTTOM "CT LeftBottom"
#define  CONF_SET_DISPLAY_CT_RIGHTTOP "CT RightTop"
#define  CONF_SET_DISPLAY_CT_RIGHTBOTTOM "CT RightBottom"
#define  CONF_SET_DISPLAY_MR_LEFTTOP_DEFAULT "6,1,2,5"
#define  CONF_SET_DISPLAY_MR_LEFTBOTTOM_DEFAULT "3,4,18"
#define  CONF_SET_DISPLAY_MR_RIGHTTOP_DEFAULT "11,7,8,10"
#define  CONF_SET_DISPLAY_MR_RIGHTBOTTOM_DEFAULT "13,15,17"
#define  CONF_SET_DISPLAY_CT_LEFTTOP_DEFAULT "1,2,3,4,5"
#define  CONF_SET_DISPLAY_CT_LEFTBOTTOM_DEFAULT "6,7,8,9,10,11"
#define  CONF_SET_DISPLAY_CT_RIGHTTOP_DEFAULT "12,13,14,15"
#define  CONF_SET_DISPLAY_CT_RIGHTBOTTOM_DEFAULT ""

#define  CONF_BASE			"Base" //Add  by Dai Liang 2007/6/7
#define  CONF_BASE_INDEX	"Base index"//Add  by Dai Liang 2007/6/7

#define  CONF_PROBEHOLDER	"Probe holder"//Add  by Dai Liang 2007/6/7
#define  CONF_PROBEHOLDER_GROOVEANGLE "Groove angle of probe holder"//Add  by Dai Liang 2007/6/7
#define  CONF_PROBEHOLDER_ORI_X  "Orientation X"  //added by ningsj 2011/6/10
#define  CONF_PROBEHOLDER_ORI_Y  "Orientation Y"
#define  CONF_PROBEHOLDER_ORI_Z  "Orientation Z"

#define  CALI_SET       "Calibration"
#define  CALI_AXI_WIDTH "Axi window width"
#define  CALI_AXI_LEVEL "Axi window level"
#define  CALI_COR_WIDTH "Cor window width"
#define  CALI_COR_LEVEL "Cor window level"
#define  CALI_SAG_WIDTH "Sag window width"
#define  CALI_SAG_LEVEL "Sag window level"
#define	 CALI_THRESHOLD	"Threshhold"
#define  CONF_PROTOCAL_AXI_PART "Axial part"
#define  CONF_PROTOCAL_AXI_PROTOCAL "Axial protocal"
#define  CONF_PROTOCAL_SAG_PART "Sagital part"
#define  CONF_PROTOCAL_SAG_PROTOCAL "Sagital protocal"
#define  CONF_PROTOCAL_COR_PART "Coronal part"
#define  CONF_PROTOCAL_COR_PROTOCAL "Coronal protocal"
#define  CALI_CIRCLE_POINTS_MIN "Circle Points Minimum"
#define  CALI_CIRCLE_POINTS_MAX "Circle Points Maximum"

void SetINIFileName(char* pFileName);

class CConfigMgr
{
public:
	CConfigMgr(void);

	void Write(LPCTSTR pszSession, LPCTSTR pszEntry, int nValue, LPCTSTR pFile = NULL);
	void Write(LPCTSTR pszSession, LPCTSTR pszEntry, float fValue, LPCTSTR pFile = NULL);
	void Write(LPCTSTR pszSession, LPCTSTR pszEntry, double dbValue, LPCTSTR pFile = NULL);
	void Write(LPCTSTR pszSession, LPCTSTR pszEntry, LPCTSTR pszValue, LPCTSTR pFile = NULL);

	int Read(LPCTSTR pszSession, LPCTSTR pszEntry, int nDefault=0, LPCTSTR pFileName = NULL);
	float Read(LPCTSTR pszSession, LPCTSTR pszEntry, float nfefault=1.0, LPCTSTR pFile = NULL);
	CString Read(LPCTSTR pszSession, LPCTSTR pszEntry, LPCTSTR pszDefault=NULL, LPCTSTR pFile = NULL);
	double Read(LPCTSTR pszSession, LPCTSTR pszEntry, double dbDefault=0.0, LPCTSTR pFile = NULL);

	CString ReadREG(HKEY hKey, LPCTSTR pszSession, LPCTSTR pszEntry, LPCTSTR pszDefault=NULL);
private:
};
#endif
