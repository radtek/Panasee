
/***************************************************************************
 *
 * University of California, Davis
 * UCDMC DICOM Network Transport Libraries
 * Version 0.1 Beta
 *
 * Technical Contact: mhoskin@ucdavis.edu
 *
 ***************************************************************************/
//��������ľ���GROUPLENGTH�����⡣
//����IMPLICIT,���еĶ�һ�����ȣ�����EXPLICIT��SQ,OB,OW�����⣬�ر���SQ�ı��롣
//ͬʱ��EXPLICIT�����뱣������ΪDICOM �ļ�ͷ��FILE META������EXPLICIT���룬
//GROUPLENGTH������ģ�(0X0002,0X0000)�Ǳ����
//������EXPLICIT�����㳤�ȣ������Ҫ��IMPLICIT�����㳤�ȣ���ô��Ҫ����ת������
//�����ֵ����Я��

#	include	"dicom.hpp"
//#	include	"mini.hpp"
UINT32 GetVRRawLength(VR*vr);
//�õ�DDO�ĳ���
UINT32 GetDDORawLength(DICOMObject*pDDO)
{
	UINT nTotalLen=0;
	for(UINT j=0;j<pDDO->VRGroupPQueues.GetSize();j++)
	{
		VRGroupPQueue* pVRPQ=pDDO->VRGroupPQueues.Get(j);
		Array<VR *>* pGroup=pVRPQ;
		UINT nLen=0;
		for(UINT k=0;k<pGroup->GetSize();k++)
		{
			VR*vr1=pGroup->Get(k);
			nLen+=GetVRRawLength(vr1);
		}
		if(pVRPQ->Group==0xFFFE)//����FFFE������ITEM������GROUPLENGTH
			pVRPQ->Length=nLen;
		else
			pVRPQ->Length=nLen+sizeof(UINT16) + sizeof(UINT16) + sizeof(UINT32) +sizeof(UINT32);//��GROUPLENGTH
		nTotalLen+=pVRPQ->Length;
	}
	pDDO->Length=nTotalLen;
	return nTotalLen;
}
//�õ�VR�ĳ���
UINT32 GetVRRawLength(VR*vr)
{
	int nTotalLen=8;
	if(	(vr->TypeCode=='OB')||
		(vr->TypeCode=='OW')||
		(vr->TypeCode=='OF')||
		(vr->TypeCode=='UN')||
		(vr->TypeCode=='UT')||
		(vr->TypeCode=='SQ'))
	{
		nTotalLen+=sizeof(UINT32);
	}
	if(vr->SQObjectArray==NULL)
	{
		return nTotalLen+vr->Length;
	}
	Array<DICOMObject *> *pADO=(Array<DICOMObject *> *) vr->SQObjectArray;
	for(UINT i=0;i<pADO->GetSize();i++)
	{
		DICOMObject*pDDO=pADO->Get(i);
		if(vr->Group == 0x7fe0&&vr->Element==0x0010)
		{
			nTotalLen+=GetDDORawLength(pDDO)+(sizeof(UINT16) + sizeof(UINT16) + sizeof(UINT32));//����ٶ�����ITEM������PIXEL��EXPLICIT LENGTH�������⣩������UNDEFINED LENGTH����
		}
		else
		{
			nTotalLen+=GetDDORawLength(pDDO)+2*(sizeof(UINT16) + sizeof(UINT16) + sizeof(UINT32));//����ٶ�����ITEM������PIXEL��EXPLICIT LENGTH�������⣩������UNDEFINED LENGTH����
		}
	}
	nTotalLen+=8;//����EXPLICIT��SQ����Sequence Delimitation Item��β������IMPLICIT��û�������
	return nTotalLen;
}
/********************************
 *
 * Base VR Type
 *
 ********************************/
//���캯��,�Ƿ�����ڴ�
VR	::	VR(UINT16 g, UINT16 e, UINT32 l, BOOL	Alloc)
	{
	BYTE	*bptr = NULL;
	BOOL	cLast = FALSE;

	if ( l & 0x01 )
		{
		++l;
		cLast = TRUE;
		}


	Group = g;
	Element = e;
	Length = l;
	if(Alloc)
		Data = new BYTE [ Length ];
	else
		Data = NULL;

	if ( cLast )
		{
		bptr = (BYTE*)Data;
		bptr[Length-1] = '\0';
		}
	ReleaseMemory = TRUE;
	SQObjectArray = NULL;
	TypeCode = 0;
	Valid = FALSE;
	}
//��ʼ������,
VR	::	VR(UINT16 g, UINT16 e, UINT32 l, void	*d)
	{
	BYTE	*bptr = NULL;
	BOOL	cLast = FALSE;

/*	if ( l & 0x01 )
		{
		++l;
		cLast = TRUE;
		}*/

	Group = g;
	Element = e;
	Length = l;
//	if(Length&1)
//		++Length;
	Data = d;
	if ( cLast )
		{
		bptr = (BYTE*)Data;
		bptr[Length-1] = '\0';
		}
	ReleaseMemory = TRUE;
	SQObjectArray = NULL;
	TypeCode = 0;
	Valid = FALSE;
	}
//��ʼ��,�Ƿ��ͷ��ڴ�
VR	::	VR(UINT16 g, UINT16 e, UINT32 l, void	*d, BOOL	mFlag)
	{
	BYTE	*bptr = NULL;
	BOOL	cLast = FALSE;

/*	if ( l & 0x01 )
		{
		++l;
		cLast = TRUE;
		}*/

	Group = g;
	Element = e;
	Length = l;
//	if(Length&1)
//		++Length;
	Data = d;
	if ( cLast )
		{
		bptr = (BYTE*)Data;
		bptr[Length-1] = '\0';
		}
	ReleaseMemory = mFlag;
	SQObjectArray = NULL;
	TypeCode = 0;//������ζ�ţ�������½�һ��VR����ô�����TYPECODE��ֵ
	Valid = FALSE;
	}
//��������
VR	::	~VR()
	{
	Array<DICOMObject	*>	*ArrayDO;
/*
	printf("~VR()\n\tGroup = %4.4x Element = %4.4x Data = %x Length = %d\n",
		Group, Element, Data, Length);
	printf("\tReleasememory = %d, SQObjectArray = %x, RID = %x Valid = %d\n",
		ReleaseMemory, SQObjectArray, ReferentialIntegrityDatabase, Valid);
	fflush(stdout);
*/
	if(ReleaseMemory)
		if(Data)
			delete [] Data;
	if(SQObjectArray)
		{
		ArrayDO = (Array<DICOMObject *> *) SQObjectArray;
		while(ArrayDO->GetSize())
			{
			delete ArrayDO->Get(0);
			ArrayDO->RemoveAt(0);
			}
		delete ArrayDO;
		SQObjectArray = NULL;
		}
		
	}

//������
UINT	VR	::	operator	>	(VR	&vr)
	{
	if(Group > vr.Group)
		return ( 1 );
	if(Group == vr.Group)
		{
		if(Element > vr.Element)
			return ( 1 );
		}
	return ( 0 );
	}
//������
UINT	VR	::	operator	<	(VR	&vr)
	{
	if(Group < vr.Group)
		return ( 1 );
	if(Group == vr.Group)
		{
		if(Element < vr.Element)
			return ( 1 );
		}
	return ( 0 );
	}
//�Ƿ����,������
UINT	VR	::	operator	==	(VR	&vr)
	{
	if(Group == vr.Group)
	if(Element == vr.Element)
			return ( 1 );
	return ( 0 );
	}

/*****************************************************
 *
 * VR Group P-Queues
 *
 *****************************************************/
//ѹ������
BOOL	VRGroupPQueue	::	Push(VR	*vr)
	{
//	printf("[VRGroupPQueue : Push(%x)]\n", vr);
	if(vr->Group!=Group)
		return ( FALSE );
	Length += GetVRRawLength(vr);
	PQueueOfPtr<VR *> :: Push ( vr );
	return ( TRUE );
	}
//��������
VR	*	VRGroupPQueue	::	Pop()
	{
	VR	*tv;

//	printf("[VRGroupPQueue : Pop()]\n");
	if ( !PQueueOfPtr<VR *> :: GetSize())
		return ( (VR *) NULL );
	tv = PQueueOfPtr<VR *> :: Pop ();
	if(!tv)
		return ( tv );
	Length -= GetVRRawLength(tv);
	return ( tv );
	}
//�鳤��
VR	*	VRGroupPQueue	::	GroupLength()
	{
	LE_UINT32	*puint32;

	Element0 = new VR ( Group, 0x0000, 4, TRUE);
	if(!Element0)
		return ( NULL );
	puint32 = (LE_UINT32 *) Element0->Data;
	(*puint32) = (LE_UINT32) Length;
	Element0->TypeCode = 'UL';
	return ( Element0 );
	}
//������
UINT	VRGroupPQueue	::	operator	>	(VRGroupPQueue	&vrgroup)
	{
	if(Group > vrgroup.Group)
		return ( 1 );
	return ( 0 );
	}
//������
UINT	VRGroupPQueue	::	operator	<	(VRGroupPQueue	&vrgroup)
	{
	if(Group < vrgroup.Group)
		return ( 1 );
	return ( 0 );
	}
//������
UINT	VRGroupPQueue	::	operator	==	(VRGroupPQueue	&vrgroup)
	{
	if(Group == vrgroup.Group)
		return ( 1 );
	return ( 0 );
	}

/*****************************************************
 *
 * DICOM Objects
 *
 *****************************************************/
//DDO��ʼ��
DICOMObject	::	DICOMObject()
	{
	Packaged = FALSE;
	Length = 0;
	VRGroupPQueues.ClearType = FALSE;
	SQObjectArray = NULL;
	LastVR = NULL;
	LastVRG = NULL;
	FreeSQObjectSize = 0;
	AutoMakeDO = NULL;
	FixedSize = 0;
	UseFixedSize = FALSE;
	}
//����	
DICOMObject	::	~DICOMObject()
 	{
	VR	*VRPtr;

	if ( !Packaged)
		if ( !Package() )
			return;

	if ( Packaged )
		{
		while ( VRPtr = this->Pop())
			delete VRPtr;
		return;
		}
	while ( VRGroupPQueues.GetSize() )
		VRGroupPQueues.RemoveAt(0);
	}
//ѹջddo,��Ҫ������SQ
BOOL	DICOMObject	::	Push(DICOMObject	*SQDO)
	{
	Array<DICOMObject *>	*ArrayPtr;

	if ( Packaged )
		return ( FALSE );
	if ( ! LastVR )
		{
		return ( FALSE );	// not-supported yet
		if (!SQObjectArray)
			{
			SQObjectArray = new Array<DICOMObject *>;
			if ( !SQObjectArray )
				return ( FALSE );
			}
		SQObjectArray->Add ( SQDO );
		FreeSQObjectSize += SQDO->Length;
		return ( TRUE );
		}
	if (!LastVR->SQObjectArray)
		{
		ArrayPtr = new Array<DICOMObject *>;
		if (!ArrayPtr)
			return ( FALSE );
		LastVR->SQObjectArray = (void *) ArrayPtr;

		// "new sequence"
		LastVRG->Length +=  (sizeof(UINT16) + sizeof(UINT16) +
						sizeof(UINT32) );//SQ��DELIMITATION 8�ֽ�
		Length += (sizeof(UINT16) + sizeof(UINT16) +
						sizeof(UINT32) );
///////////////////////////////////////////////////////////////////
		}
	ArrayPtr = (Array<DICOMObject *> *) LastVR->SQObjectArray;
	ArrayPtr->Add ( SQDO );	
	LastVRG->Length += SQDO->Length + 
			(2 * (sizeof(UINT16) + sizeof(UINT16) + sizeof(UINT32)));
	Length += SQDO->Length + 
			(2 * (sizeof(UINT16) + sizeof(UINT16) + sizeof(UINT32)));
	if((LastVR->Group == 0x7fe0 )&&(LastVR->Element == 0x0010))
	{
		LastVRG->Length-=sizeof(UINT16) + sizeof(UINT16) + sizeof(UINT32);//PIXEL
		Length-=sizeof(UINT16) + sizeof(UINT16) + sizeof(UINT32);
	}
	return ( TRUE );
	}
//��LENGTH�ļ���������VRGROUP�ļ���
extern RTC GlobalRTC;
//ѹ��vr
BOOL	DICOMObject	::	Push(VR	*vr)
	{
	UINT			Index;
 	VRGroupPQueue	*VRGroupPQueuePtr;
	BOOL			rBOOL;
	UINT			LocalGroupLength;
//	printf("[Push]\n");
	if ( Packaged )
		return ( FALSE );	// already Pop() from this object

	if(vr->Length == 0xffffffff)
		{
//		fprintf(stdout, "Attemping to push -1 length\n");
		vr->Length = 0;
		}
	if(AutoMakeDO)
		{
		rBOOL = AutoMakeDO->Push ( vr );
		if ( rBOOL )
			LastVRG->Length += GetVRRawLength(vr);
		return ( rBOOL );
		}

	Index = 0;
	if(vr->Element == 0x0000)
		{
//		fprintf(stderr, "Attemping to push GLC: %d\n",
//			(*((UINT32 *) vr->Data)));
		delete vr;
		return ( TRUE );	// ignore Group Length Codes (auto calculated)
		}
	LastVR = vr;
	if(vr->Length&0x01)
	{
		vr->Length++;//����������⣬��Ϊ����Խ��
		char*pData=new char[vr->Length];
		memcpy(pData,vr->Data,vr->Length-1);
		GlobalRTC.RunTimeClass(vr);
		switch(vr->TypeCode)
		{
			//���漸��������' 'padding
			case 'AE':
			case 'CS':
			case 'DS':
			case 'IS':
			case 'LO':
			case 'LT':
			case 'PN':
			case 'SH':
			case 'ST':
			case 'TM':
			case 'UT':
				pData[vr->Length-1]=' ';//PEAK CHANGE AT 2006.8.25
				break;
			case 'DT':
			case 'OB':
			case 'UI':
				pData[vr->Length-1]='\0 ';//PEAK CHANGE AT 2006.8.25
				break;
			default:
				pData[vr->Length-1]='\0';//PEAK CHANGE AT 2006.8.25
				break;
		}
		if(vr->ReleaseMemory)
			delete vr->Data;
		vr->Data=pData;
		vr->ReleaseMemory=TRUE;
	}
	if(LastVRG)
		if(LastVRG->Group == vr->Group)
			{
//			Length += vr->Length + sizeof(UINT16) + sizeof(UINT16) +
//						sizeof(UINT32);
//			if(vr->Length&0x01)
//				++Length;
//			return ( LastVRG->Push ( vr ));
//peak change to follow
			LocalGroupLength=LastVRG->Length;
			rBOOL=LastVRG->Push ( vr );
			Length+=LastVRG->Length-LocalGroupLength;
			return rBOOL;
			}
//	printf("[VRGroupPQueues.GetSize() : %d]\n", VRGroupPQueues.GetSize());
	while ( Index < VRGroupPQueues.GetSize())
		{
		VRGroupPQueuePtr = VRGroupPQueues.Get(Index);
		if(!VRGroupPQueuePtr)
			return ( FALSE );
		if(VRGroupPQueuePtr->Group == vr->Group)
			{
			LastVRG = VRGroupPQueuePtr;
//			Length += vr->Length + sizeof(UINT16) + sizeof(UINT16) +
//						sizeof(UINT32);
//			if(vr->Length&0x01)
//				++Length;
//			return ( VRGroupPQueuePtr->Push ( vr ));
//peak change to follow
			LocalGroupLength=LastVRG->Length;
			rBOOL=LastVRG->Push ( vr );
			Length+=LastVRG->Length-LocalGroupLength;
			return rBOOL;
			}
		++Index;
		}

	VRGroupPQueuePtr = new VRGroupPQueue(vr->Group);
	if(!VRGroupPQueuePtr)
		return ( FALSE );

	if(vr->Group!=0xFFFE)//fffeû��GROUPLENGTH
	{
		Length += sizeof(UINT16) + sizeof(UINT16) + sizeof(UINT32) +sizeof(UINT32);//GROUPLENGTH
	}
	LastVRG = VRGroupPQueuePtr;	
	VRGroupPQueues.Add(VRGroupPQueuePtr);
//peak change to follow
	LocalGroupLength=LastVRG->Length;
	rBOOL=LastVRG->Push ( vr );
	Length+=LastVRG->Length-LocalGroupLength;
	return rBOOL;

//	return ( VRGroupPQueuePtr->Push(vr) );
	}
//����VR
VR	*	DICOMObject	::	Pop()
	{
	VR	*VRPtr;
	UINT LocalGroupLength;
	//return ( (VR *) NULL );

	if(!Packaged)
		if(!Package())
			return ( (VR *) NULL );
	if(!Packaged)
		return ( (VR *) NULL );

	if ( ! CurrentGroup )
		{
//		printf("[Pop : CurrentGroup = NULL]\n");
		if ( !PQueueOfPtr<VRGroupPQueue *>::GetSize())
			{
			Packaged = FALSE;
			LastVRG = NULL;
			LastVR = NULL;
			return ( (VR *) NULL );
			}
		CurrentGroup = PQueueOfPtr<VRGroupPQueue *>::Pop();
		if ( ! CurrentGroup )
			return ( (VR *) NULL );
		Length -= ( sizeof(UINT16) + sizeof(UINT16) + sizeof(UINT32) +
				sizeof(UINT32) );//GROUPLENGTH
	
		return ( CurrentGroup->GroupLength() );
		}
//peak add
	LocalGroupLength=CurrentGroup->Length;
//
	if ( ! ( VRPtr = CurrentGroup->Pop() ) )
		{
		delete CurrentGroup;
		CurrentGroup = NULL;
		return ( this->Pop() );
		}
//	Length -= (VRPtr->Length + sizeof(UINT16) + sizeof(UINT16) +
//			sizeof(UINT32) );
//	if(VRPtr->Length&0x01)
//		--Length;	// See Push
//peak change to follow
	Length-=LocalGroupLength-CurrentGroup->Length;
	return ( VRPtr );
	}

// this needs to be replaced with a more efficient algorithm in the
// future, but it will do for now.
//��ѯVR
VR		*DICOMObject	::	GetVR(UINT16	g, UINT16	e)
	{
	VR				*vr;
	UINT			Index,Index2;
		VRGroupPQueue	*VRGroupPQueuePtr;

	Index = 0;

	while ( Index < VRGroupPQueues.GetSize())
		{
		VRGroupPQueuePtr = VRGroupPQueues.Get(Index);
		if(!VRGroupPQueuePtr)
			return ( FALSE );
		if(VRGroupPQueuePtr->Group == g)
			{
			Index2 = 0;
			while ( Index2 < VRGroupPQueuePtr->GetSize() )
				{
				vr = VRGroupPQueuePtr->Get(Index2);
				if(!vr)
					return ( 0 );
				if(vr->Element == e)
					{
					return ( vr );
					}
				++Index2;
				}
			return ( NULL );
			}
		++Index;
		}
	return ( NULL );
	}
		
//�õ�VR,������ת����UINT16	
LE_UINT16	DICOMObject	::	GetUINT16(UINT16	g, UINT16	e)
	{
	VR				*vr;
//	UINT			Index,Index2;
//		VRGroupPQueue	*VRGroupPQueuePtr;
	LE_UINT16		Val;

	vr = this->GetVR ( g, e );
	if(!vr)
		return(0);
		
	if ( vr->Data )
		if(vr->Length == sizeof(UINT16))
			{
			memcpy(&Val, vr->Data, 2);
			return ( Val );
			}
	return ( 0 );
	}

//����ddo
BOOL	DICOMObject	::	Reset()
	{
	VR	*vr;

	while ( vr = Pop() )
		delete vr;

	LastVRG = NULL;
	LastVR = NULL;

	return ( TRUE );
	}
//���
BOOL	DICOMObject	::	Package()
	{
	UINT	Index;
	
	CurrentGroup = NULL;
	Index = 0;
//	printf("[start: Package()]\n");
	while ( Index < VRGroupPQueues.GetSize())
		{
		PQueueOfPtr<VRGroupPQueue *> :: Push (VRGroupPQueues.Get(Index) );
		++Index;
		}
	while ( VRGroupPQueues.GetSize() )
		VRGroupPQueues.RemoveAt(0);
		
//	printf("[Packaged : %d Groups]\n", Index);
	Packaged = TRUE;
	if ( Index )
		return ( TRUE );
	Packaged = FALSE;
	return ( FALSE );
	}
//�����������ݳ���
BOOL	DICOMObject::ReGenerateGroupLength(BOOL bExplicit)
{
//��������VR�ڵ㣬����OB,OW,OF,UN,UT,SQ�����⴦����Ϊ��EXPLICIT��IMPLICIT����4���ֽڵĲ�ֵ
	if(Packaged)
	{
		//�Ѿ���ʼPOP��
		return FALSE;
	}
	if(bExplicit==TRUE)//
		return TRUE;
	for(UINT i=0;i<VRGroupPQueues.GetSize();i++)
	{
		VRGroupPQueue* pVRPQ=VRGroupPQueues.Get(i);
		Array<VR *>* pGroup=pVRPQ;
		UINT nLen=0;
		for(UINT j=0;j<pGroup->GetSize();j++)
		{
			VR*vr=pGroup->Get(j);
			if(	(vr->TypeCode=='OB')||
				(vr->TypeCode=='OW')||
				(vr->TypeCode=='OF')||
				(vr->TypeCode=='UN')||
				(vr->TypeCode=='UT')||
				(vr->TypeCode=='SQ'))
			{
				nLen+=4;
			}
			if(vr->SQObjectArray)
			{
				Array < DICOMObject * > * pSQ=(Array < DICOMObject * > *)(vr->SQObjectArray);
				for(UINT k=0;k<pSQ->GetSize();k++)
				{
					nLen+=pSQ->Get(k)->Length;
					pSQ->Get(k)->ReGenerateGroupLength(bExplicit);
					nLen-=pSQ->Get(k)->Length;
				}
			}
		}
		pVRPQ->Length-=nLen;
		Length-=nLen;
	}
	return TRUE;
}


BOOL
VR	::	SetIf(
	VR	*vr)
	{
	Array<DICOMObject	*>	*ArrayDO;

	if(!vr)
		return ( FALSE );
	if(vr->Group==Group)
		if(vr->Element==Element)
			{
			if(ReleaseMemory)
				delete Data;
			if(SQObjectArray)
				{
				ArrayDO = (Array<DICOMObject *> *) SQObjectArray;
				while(ArrayDO->GetSize())
					{
					delete ArrayDO->Get(0);
					ArrayDO->RemoveAt(0);
					}
				delete ArrayDO;
				SQObjectArray = NULL;
				}
			Length = vr->Length;
			Data = vr->Data;
			SQObjectArray = vr->SQObjectArray;
			ReleaseMemory = vr->ReleaseMemory;
			if(vr->TypeCode!=0x0000)//peak
				TypeCode = vr->TypeCode; //add by Lin Tianyi
			Valid = vr->Valid; //add by Lin Tianyi
			vr->Data = NULL;
			vr->Length = 0;
			vr->SQObjectArray = NULL;
			vr->ReleaseMemory = FALSE;
			vr->Valid = TRUE;
			return ( TRUE );
			}
	return ( FALSE );
	}

/*
BOOL VR::SetIf(VR	*vr)
{ Array<DICOMObject*>	*ArrayDO;
  int			i, iSecialCodeOfVR;
  BOOL			rc = FALSE;
  char*			pCopy;
  unsigned char*	pRGB;
  unsigned char*	pMono;
  int			iNewLength;
  int			iNewValue, iPad = 0;

  if (!vr)
    return rc;
  if ((vr->Group==Group) && (vr->Element==Element))
  {
    iSecialCodeOfVR = GetSpecialValueCodeOfVR(vr);
    switch (iSecialCodeOfVR)
    { case VR_SPECIAL_NONE:
     //  Just replace the VR 
      if(ReleaseMemory)
        delete [] Data;
      if(SQObjectArray)
      { ArrayDO = (Array<DICOMObject *> *) SQObjectArray;
	while(ArrayDO->GetSize())
	{ delete ArrayDO->Get(0);
	  ArrayDO->RemoveAt(0);
	}
	delete ArrayDO;
	SQObjectArray = NULL;
      }
      Length = vr->Length;
      if (vr->Length)
      { Data = new BYTE[vr->Length];
	memcpy(Data, vr->Data, vr->Length);
	ReleaseMemory = TRUE;
      }
      else
	ReleaseMemory = FALSE;
      rc = TRUE;
      break;

      case VR_SPECIAL_FIX_TMS:
      // Change PatientName like 'LastName^FirstName^MiddleName' into 'LastName,FirstName,MiddleName'.
      if (Length > 0)
      { pCopy = (char*)malloc(Length+2);
	memcpy(pCopy, (char*)Data, Length);
	pCopy[Length] = 0;
	// Maybe padded with a space 
	if ((pCopy[Length-1] == ' ') || (pCopy[Length-1] == '^'))
	  pCopy[Length-1] = 0;
	if ((Length > 1) && (pCopy[Length-2] == '^'))
	  pCopy[Length-2] = 0;
	for (i=0; i<(int)strlen(pCopy); i++)
	  if (pCopy[i] == '^')
	    pCopy[i] = ',';
	// May pad it again 
	if (strlen(pCopy) & 0x01)
	  strcat(pCopy, " ");
	strncpy((char*)Data, pCopy, strlen(pCopy));
	Length = strlen(pCopy);
	free(pCopy);
      }
      rc = TRUE;
      break;

      case VR_SPECIAL_RGB_TO_MONO:
      // Convert triplets of bytes to just bytes (the mean of the RGB channels) 
      if (Length >= 3)
      { iNewLength = Length / 3;
	if (iNewLength & 0x01)
	  iPad = 1;
        pMono = (unsigned char*)malloc(iNewLength + iPad);
	pRGB = (unsigned char*)Data;
	for (i=0; i<iNewLength; i++)
	{ iNewValue = (pRGB[i * 3] + pRGB[i * 3 + 1] + pRGB[i * 3 + 2]) / 3; 
	  pMono[i] = (unsigned char)iNewValue;
	}
	if (iPad)
	  pMono[iNewLength] = 0;
	Length = iNewLength + iPad;
	free(Data);
	Data = pMono;
      }
      rc = TRUE;
      break;

      case VR_SPECIAL_RGB_TO_MONO_PLUS:
      // Convert triplets of bytes to just bytes only when the value of those three	 are equal (a B&W image). However, 'real' color (contours e.g.) are converted to very white (255).
      
      if (Length >= 3)
      { iNewLength = Length / 3;
	if (iNewLength & 0x01)
	  iPad = 1;
        pMono = (unsigned char*)malloc(iNewLength + iPad);
	pRGB = (unsigned char*)Data;
	for (i=0; i<iNewLength; i++)
	{ if ((pRGB[i * 3] == pRGB[i * 3 + 1]) && (pRGB[i * 3] == pRGB[i * 3 + 2])) 
	    pMono[i] = pRGB[i * 3];
	  else
	    pMono[i] = 255;
	}
	if (iPad)
	  pMono[iNewLength] = 0;
	Length = iNewLength + iPad;
	free(Data);
	Data = pMono;
      }
      rc = TRUE;
      break;
    }
  }
  return rc;
}

*/
//����
BOOL VR::Morph (DICOMObject	*DDO)
{
	Array<DICOMObject*>	*ArrayDO;
	if (!Length)
	{
		if(!Valid)
		{
			return ( FALSE );
		}
	}

	VR	*vr    = new VR(Group, Element, 0, (BOOL) FALSE);
	vr->Length = Length;
	vr->Data   = Data;
	vr->ReleaseMemory = ReleaseMemory;
	vr->TypeCode      = TypeCode; //peak add
	Length = 0;
	Data   = NULL;
	ReleaseMemory = FALSE;
	DDO->Push(vr);

	if(SQObjectArray)
	{
		ArrayDO = (Array<DICOMObject *> *) SQObjectArray;
		while(ArrayDO->GetSize())
		{
			DDO->Push(ArrayDO->Get(0));
			ArrayDO->RemoveAt(0);
		}
		delete ArrayDO;
		SQObjectArray = NULL;
	}
	return ( TRUE );
}
//����
BOOL VR::Reset ()
{
	Array<DICOMObject*>	*ArrayDO;

	if(ReleaseMemory)
	{
		if(Data)
		{
			delete [] Data;
		}
	}
	if(SQObjectArray)
	{
		ArrayDO = (Array<DICOMObject *> *) SQObjectArray;
		while(ArrayDO->GetSize())
		{
			delete ArrayDO->Get(0);
			ArrayDO->RemoveAt(0);
		}
		delete ArrayDO;
		SQObjectArray = NULL;
	}
	ReleaseMemory = FALSE;
	Data          = NULL;
	Length        = 0;
	return ( TRUE );
}



