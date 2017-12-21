/********************************************************************
File Name:	buffer.h
Written by: Yang Guang

Copyright(C) Yang Guang, 2000 -

Purpose:
	This file defines following classes:
    CBuffer 

History:
	2000/4/6	Created.
*********************************************************************/
#ifndef buffer_h_2000_10_10_yangguang
#define buffer_h_2000_10_10_yangguang

#include <iostream>

/*!
    This class is used to replace the traditional array where applicable. 
    The advantage provided by this class included:
    <OL>
    <LI>Automatical deletetion upon going out of the scope. </LI>
    <LI>Resizing.</LI>
    </OL>
*/
template <class T>
class CBuffer
{
	int m_nSize;        //!< Size of controlled sequence.
	int m_nBufferSize;  //!< Size of allocated space.

    //! Maximum space waste.
    /*!
        \remarks When the allocated space is more than this larger than the
        controlled sequence, the space will be reallocated to save space.
    */
	int m_nMaxWaste;    
	T*	m_pData;    //!< Pointer to the allocated space.

public:
    //! Default constructor.
	CBuffer() {
        m_nSize = 0; 
        m_nMaxWaste = 16384;
        m_pData = NULL;
        m_nBufferSize = 0;
    }
    //! Destructor.
	~CBuffer() {
        delete []m_pData;
    }
    //! Sets the maximum allowed waste space.
	void SetMaxWaste(int n) {
        m_nMaxWaste = n;
    }
    //! Gets the maximum allowed waste space.
	int GetMaxWaste() {
        return m_nMaxWaste;
    }
    //! Sets the size of the buffer (resize).
    /*!
        \param nSize Size of the buffer to allocate.
        \param bClear If \a true, set the contents of the buffer to zero,
        else, leave it unintialized.
    */
	bool SetSize(int nSize, bool bClear = true);
    //! Returns the current size of controlled sequence.
	int GetSize() {
        return m_nSize;
    }
	void Clear();	//!< Empty the buffer by releasing the space allocated.

    //! Operator used to facilitate the access of the buffer.
    /*!
        \param i Index of the element to retrieve.
    */
	T& operator[](int i){
        return m_pData[i];
    }
    //! OPerator used to facilitate the access of the buffer. Constant version.
    const T& operator[](int i) const{
        return m_pData[i];
    }

	operator T*(){
        return m_pData;
    }
	T* GetBuffer(){
        return m_pData;
    }
	void Fill(BYTE b);
};

//! This function set the size of the buffer. 
/*!
	It will re-allocate the memory when necessary. It will also set the contents
	of the buffer to zero.
*/
template <class T>
bool CBuffer<T>::SetSize(int nSize, bool bClear)
{
	m_nSize = nSize;

	if (nSize == 0)
	{
		delete m_pData;
		m_pData = NULL;
		m_nBufferSize = 0;
		return true;
	}

	// If the size is larger than the current size, we must re-allocate.
	// If the new size is much smaller than the current size, we also
	// reallocate the memory to save the memory
	if (nSize > m_nBufferSize ||  m_nBufferSize - nSize >= m_nMaxWaste)
	{
		delete []m_pData;
		try
		{
			m_pData = new T[nSize];
			m_nBufferSize = nSize;
		}
        catch(std::bad_alloc& e)
		{
            std::cerr << e.what();
			return false;
		}
	}
	if (bClear)
    {
		memset(m_pData, 0, m_nSize * sizeof (T));
    }

	return true;
}

template<class T>
void CBuffer<T>::Fill(BYTE b)
{
	memset(m_pData, 0, m_nSize * sizeof (T));
}

template <class T>
void CBuffer<T>::Clear()
{
	delete []m_pData;
	m_pData = NULL;
	m_nBufferSize = 0;
	m_nSize = 0;
}
#endif
