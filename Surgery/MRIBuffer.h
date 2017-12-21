// MRIBuffer.h: interface for the CMRIBuffer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MRIBUFFER_H__13E22F2F_85F8_407A_B218_6ACA790D9A64__INCLUDED_)
#define AFX_MRIBUFFER_H__13E22F2F_85F8_407A_B218_6ACA790D9A64__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMRISeries;

#define IMG_BUFFER_SIZE 10   


class CMRIBuffer  
{
public:
	CMRIBuffer();
	virtual ~CMRIBuffer();

	BOOL Push(CMRISeries* pMRISeries);

	BOOL SetDisp(CMRISeries* pMRISeries);
	CMRISeries* GetDisp();

	CMRISeries* GetSeries(CString sStudyID, int nSeriesID);  // Get last MRI series with ID
	CMRISeries* GetSeries(CString sName); // Get series with name
	CMRISeries* GetSeriesAt(int iIndex);  // Get seires at index iIndex

	BOOL Delete(CString strName); // Delete series by name of the series.

private:
	CMRISeries* m_ppMRIBuffer[IMG_BUFFER_SIZE];		// MRI display buffer
	int  m_iDispIndex;		// point to item displayed
	int  m_iSaveIndex;		// point to slot where next 

	int  NextSlot(int iIndex);
	int  PreSlot(int iIndex);
};

#endif // !defined(AFX_MRIBUFFER_H__13E22F2F_85F8_407A_B218_6ACA790D9A64__INCLUDED_)
