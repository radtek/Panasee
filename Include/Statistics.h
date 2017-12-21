// Statistics.h: interface for the CStatistics class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STATISTICS_H__397E975B_6EB9_48D6_A941_0356C5A7FD9C__INCLUDED_)
#define AFX_STATISTICS_H__397E975B_6EB9_48D6_A941_0356C5A7FD9C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class __declspec(dllexport) CStatistics  
{
public:

	//////////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////////
	
	CStatistics();
	virtual ~CStatistics();

	//////////////////////////////////////////////////////////////////////////
	// Operations
	//////////////////////////////////////////////////////////////////////////

	// Get the number of the values
	int		GetNumOfValues();

	// Get the number of the positive values ( >= 0 )
	int		GetNumOfPositiveValues();

	// Get the number of the negative values
	int		GetNumOfNegativeValues();

	// Get the pointer to the values
	double*	GetValues();

	// Get maximum positive value
	double	GetMaxPositiveValue();

	// Get maximum negative value
	double	GetMaxNegativeValue();

	// Get average value
	double	GetAverageValue();

	// Get average positive value
	double	GetAveragePositiveValue();

	// Get average negative value
	double	GetAverageNegativeValue();
	
	// Get standard deviation
	double	GetStandardDeviation();

	// Compute the statistics
	int		ComputeStatistics( double* pdValue, int nNumOfValues );

	// Save the statistics to TXT file
	int		SaveToTXTFile( CString sFileName );

	//////////////////////////////////////////////////////////////////////////
	// Members
	//////////////////////////////////////////////////////////////////////////

private:

	//////////////////////////////////////////////////////////////////////////
	// Operations
	//////////////////////////////////////////////////////////////////////////

	// Construct the statistics
	int		Construct();

	//////////////////////////////////////////////////////////////////////////
	// Members
	//////////////////////////////////////////////////////////////////////////

	// Pointer to the values
	double*	m_pdValue;

	// Size of the values
	int		m_nNumOfValues;

	// Size of the positive values
	int		m_nNumOfPositiveValues;

	// Size of the negative values
	int		m_nNumOfNegativeValues;

	// Standard deviation
	double	m_dStdDev;

	// Average value
	double	m_dAverage;

	// Average positive value
	double	m_dAveragePositive;

	// Average negative value
	double	m_dAverageNegative;

	// Maximum positive value
	double	m_dMaxPositive;

	// Maximum negative value
	double	m_dMaxNegative;

};

#endif // !defined(AFX_STATISTICS_H__397E975B_6EB9_48D6_A941_0356C5A7FD9C__INCLUDED_)
