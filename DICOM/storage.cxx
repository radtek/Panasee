
#	include	"dicom.hpp"


BOOL	StandardStorage :: Read (
	PDU_Service			*PDU,
	DICOMCommandObject	*DCO,
	DICOMDataObject		*DDO)
	{
	VR	*vr;
	UID	MyUID, uid;

	GetUID(MyUID);	// always try and use GetUID to obtain my own uid

	if ( ! PDU )
		return ( FALSE );

	if ( ! DCO )
		return ( FALSE );

	if ( ! DDO )
		return ( FALSE );

	vr = DCO->GetVR ( 0x0000, 0x0002 );
	if ( ! vr )
	{
		return ( FALSE );
	}

	if ( !SetUID(uid, vr) )
	{
		return ( FALSE );
	}

	if(MyUID==UID("1.2.840.10008.5.1.4.1.1.6")&&uid==UID("1.2.840.10008.5.1.4.1.1.6.1"))//here add
	{//by peak
		;//for 
	}//compatible with new version USStorage
	else// class
	{
		if ( MyUID != uid )
		{
			return ( FALSE );
		}
	}

	if ( CStoreRQ :: Read ( DCO, PDU, DDO ) )
		{
		return ( CStoreRSP :: Write ( PDU, DCO, CheckObject( DDO ) ) );
		}

	if ( CStoreRSP :: Read ( DCO ) )
		{
		// No worries..
		return ( TRUE );
		}
	return ( FALSE );	// corrupted message
	}

BOOL	StandardStorage :: Write (
	PDU_Service *PDU,
	DICOMDataObject *DDO )
	{
	DICOMCommandObject	DCO;
	UINT16 Status;

	if ( ! PDU )
		return ( FALSE );
	if ( ! CStoreRQ :: Write ( PDU, DDO ) )
		return ( FALSE );
	if(!PDU->Read ( & DCO ))
		return (FALSE);
	if ( ! CStoreRSP :: Read ( &DCO ) )
		return ( FALSE );
	Status = DCO.GetUINT16(0x0000, 0x0900);

	if((Status&0xff00) == 0xa700) {
		SetLastError(STORE_REFUSED_OUTOFRESOURCE);
		return ( FALSE );
	} else if((Status&0xff00) == 0xa900) {
		SetLastError(STORE_ERROR_DATASETDOESNOTMATCHSOPCLASS);
		return ( FALSE );
	} else if((Status&0xff00) == 0xc000) {
		SetLastError(STORE_ERROR_CANNOTUNDERSTAND);
		return ( FALSE );
	} else if(Status == 0xb000) {
		SetLastError(STORE_WARNING_COERCIONOFDATAELEMENTS);
	} else if(Status == 0xb007) {
		SetLastError(STORE_WARNING_DATASETDOESNOTMATCHSOPCLASS);
	} else if(Status == 0xb006) {
		SetLastError(STORE_WARNING_ELEMENTDISCARDED);
	}

	return ( TRUE );
	}

// Special "Void" storage class.  Accepts any DICOM transmission C-Store
BOOL	UnknownStorage	::	GetUID ( UID	&uid )
	{
	return ( FALSE );
	}

BOOL	UnknownStorage :: Read (
	PDU_Service			*PDU,
	DICOMCommandObject	*DCO,
	DICOMDataObject		*DDO)
	{
//	VR	*vr;
	UID	MyUID, uid;


	if ( ! PDU )
		return ( FALSE );

	if ( ! DCO )
		return ( FALSE );

	if ( ! DDO )
		return ( FALSE );

	if ( CStoreRQ :: Read ( DCO, PDU, DDO ) )
		{
		return ( CStoreRSP :: Write ( PDU, DCO, CheckObject ( DDO ) ) );
		}
	
	if ( CStoreRSP :: Read ( DCO ) )
		{
		// No worries..
		return ( TRUE );
		}
	return ( FALSE );	// corrupted message
	}

BOOL	RunTimeClassStorage :: GetUID ( UID	&uid )
	{
	uid = MyUID;
	return ( TRUE );
	}

BOOL	RunTimeClassStorage :: SetUID ( UID	&uid )
	{
	MyUID = uid;
	return ( TRUE );
	}

BOOL	RunTimeClassStorage :: SetUID ( VR	*vr )
	{
	char	s [ 66 ];

	memset((void*)s, 0, 64);
	if(vr)
		{
		memcpy((void*)s, vr->Data, vr->Length%64);
		int		Length;

		Length = strlen(s);
		while(Length)
			{
			if(s[Length-1] == ' ')
				{
				s[Length-1] = '\0';
				--Length;
				continue;
				}
			break;
			}

		MyUID.Set(s);
		return(TRUE);
		}
	return ( FALSE );
	}

BOOL	RunTimeClassStorage :: SetUID ( DICOMDataObject *DDOPtr)
	{
	return(SetUID (DDOPtr->GetVR(0x0008, 0x0016)));
	}

BOOL	CRStorage	::	GetUID ( UID	&uid )
	{
	uid.Set("1.2.840.10008.5.1.4.1.1.1");
	return ( TRUE );
	}

BOOL	CTStorage	::	GetUID ( UID	&uid )
	{
	uid.Set("1.2.840.10008.5.1.4.1.1.2");
	return ( TRUE );
	}

BOOL	USMultiframeStorage	::	GetUID ( UID	&uid )
	{
	uid.Set("1.2.840.10008.5.1.4.1.1.3.1");
	return ( TRUE );
	}

BOOL	MRStorage	::	GetUID ( UID	&uid )
	{
	uid.Set("1.2.840.10008.5.1.4.1.1.4");
	return ( TRUE );
	}

BOOL	NMStorage	::	GetUID ( UID	&uid )
	{
	uid.Set("1.2.840.10008.5.1.4.1.1.5");
	return ( TRUE );
	}

BOOL	USStorage	::	GetUID ( UID	&uid )
	{//edit by peak to consist with old version new usstorage uid must plus .1
	uid.Set("1.2.840.10008.5.1.4.1.1.6");
	return ( TRUE );
	}

BOOL	SCStorage	::	GetUID ( UID	&uid )
	{
	uid.Set("1.2.840.10008.5.1.4.1.1.7");
	return ( TRUE );
	}

BOOL	StandaloneOverlayStorage	::	GetUID ( UID	&uid )
	{
	uid.Set("1.2.840.10008.5.1.4.1.1.8");
	return ( TRUE );
	}

BOOL	StandaloneCurveStorage	::	GetUID ( UID	&uid )
	{
	uid.Set("1.2.840.10008.5.1.4.1.1.9");
	return ( TRUE );
	}

BOOL	StandaloneModalityLUTStorage	::	GetUID	(UID	&uid )
	{
	uid.Set("1.2.840.10008.5.1.4.1.1.10");
	return ( TRUE );
	}

BOOL	StandaloneVOILUTStorage	::	GetUID ( UID	&uid )
	{
	uid.Set("1.2.840.10008.5.1.4.1.1.11");
	return ( TRUE );
	}

BOOL	GEStorage::GEToStandard(DICOMDataObject*DDOPtr)
{
	DICOMDataObject	*DDOPtr1 = new DICOMDataObject;
	VR *vr;
	if(!DDOPtr->GetVR(0x0008,0x0005))
	{
		char *temp=new char[64];
		strcpy(temp,"ISO_IR 100");
		VR *vr1= new VR(0x0008, 0x0005, strlen(temp),temp, TRUE);
		DDOPtr1->Push(vr1);
	}
	if(!DDOPtr->GetVR(0x0008,0x0008))//zxh???
	{
		char *temp=new char[64];
		strcpy(temp,"ORIGINAL\\PRIMARY\\AXIAL");
		VR *vr1= new VR(0x0008, 0x0008, strlen(temp),temp, TRUE);
		DDOPtr1->Push(vr1);
	}
	if(!DDOPtr->GetVR(0x0028,0x0002))
	{
		unsigned short * temp= new unsigned short;
		*temp= 1;
		VR *vr1= new VR(0x0028, 0x0002, MAX_VRUS,(void*)temp);
		DDOPtr1->Push(vr1);
	}
	if(!DDOPtr->GetVR(0x0028,0x0004))
	{
		char *temp=new char[MAX_VRCS];
		strcpy(temp,"MONOCHROME2");
		VR *vr1= new VR(0x0028, 0x0004, strlen(temp),temp, TRUE);
		DDOPtr1->Push(vr1);
	}
	unsigned short nTempForElement;
	BOOL bGECT=TRUE;
	if((vr=DDOPtr->GetVR(0x0008,0x0060)))
	{
		if(strnicmp((char*)(vr->Data),"CT",2)==0)
			bGECT=TRUE;
		else
			bGECT=FALSE;
	}
	while(vr = DDOPtr->Pop())
	{
		if(vr->Group==0x0008&& vr->Element==0x0016)
		{
			delete vr;
			char *temp=new char[64];
			if(bGECT)
				strcpy(temp,"1.2.840.10008.5.1.4.1.1.2");
			else
				strcpy(temp,"1.2.840.10008.5.1.4.1.1.4");
			VR *vr1= new VR(0x0008, 0x0016, strlen(temp),temp, TRUE);
			DDOPtr1->Push(vr1);
		}
		else if(vr->Group==0x0008&& (vr->Element==0x0020||vr->Element==0x0021||vr->Element==0x0022||vr->Element==0x0023))
		{
			nTempForElement=vr->Element;
			char *temp=new char[11];//for date
			memset(temp,0,11);
			memcpy(temp,vr->Data,vr->Length);
			delete vr;
			GetLastNByte(temp,8);
			VR *vr1= new VR(0x0008, nTempForElement, strlen(temp),temp, TRUE);
			DDOPtr1->Push(vr1);
		}
		else if(vr->Group==0x0008&& (vr->Element==0x0030||vr->Element==0x0031||vr->Element==0x0032||vr->Element==0x0033))
		{
			nTempForElement=vr->Element;
			char *temp=new char[17];//for time
			memset(temp,0,17);
			memcpy(temp,vr->Data,vr->Length);
			delete vr;
			GetLastNByte(temp,16);
			temp[6]='\0';
			VR *vr1= new VR(0x0008, nTempForElement, strlen(temp),temp, TRUE);
			DDOPtr1->Push(vr1);
		}
		else if(vr->Group==0x0020&& vr->Element==0x0011)//Series number
		{
			char *temp=new char[12];
			memset(temp,0,12);
			memcpy(temp,vr->Data,vr->Length);
			int tmp;
			sscanf(temp,"%d",&tmp);
			sprintf(temp,"%d",tmp);
			delete vr;
			VR *vr1= new VR(0x0020, 0x0011, strlen(temp),temp, TRUE);
			DDOPtr1->Push(vr1);
		}
		else if(vr->Group==0x0020&& vr->Element==0x0013)//Image number
		{
			char *temp=new char[12];
			memset(temp,0,12);
			memcpy(temp,vr->Data,vr->Length);
			int tmp;
			sscanf(temp,"%d",&tmp);
			sprintf(temp,"%d",tmp);
			delete vr;
			VR *vr1= new VR(0x0020, 0x0013,strlen(temp),temp,TRUE);
			DDOPtr1->Push(vr1);
		}
		else
			DDOPtr1->Push(vr);
	}
//study InstanceUID
	char strTmp[MAX_PATH];
	vr = DDOPtr1->GetVR(0x0020, 0x000D);
	if(!vr || vr->Length <= 0)
	{
		vr = DDOPtr1->GetVR(0x0020, 0x0010);//StudyID
		if(!vr || vr->Length <= 0)
		{
			vr = DDOPtr1->GetVR(0x0008, 0x0020);//StudyDate
			if(!vr || vr->Length <= 0)
			{
				strcpy(strTmp,"88888888");
				goto nothing1;
			}
		}
		strncpy(strTmp,(char*)vr->Data,vr->Length);
		strTmp[vr->Length]='\0';
		ToUID(strTmp);
		if(strlen(strTmp)==0)
		{
			strcpy(strTmp,"88888888");
		}
nothing1:
		char* temp=new char[MAX_PATH];
		strcpy(temp,"1.2.840.113619.");
		strcat(temp,strTmp);
		vr = new VR(0x0020, 0x000D, strlen(temp), (void *)temp, TRUE);
   		DDOPtr1->Push(vr);
	}

//series InstanceUID 加上series number以区分同一天的不同series。//zxh study??
	vr = DDOPtr1->GetVR(0x0020, 0x000E);
	if(!vr || vr->Length <= 0)
	{
		vr = DDOPtr1->GetVR(0x0008, 0x0021);//Series date
		if(!vr || vr->Length <= 0)
		{
			vr = DDOPtr1->GetVR(0x0008, 0x0031);//Series time
			if(!vr || vr->Length <= 0)
			{
				strcpy(strTmp,"88888888");
				goto nothing2;
			}
		}
		strncpy(strTmp,(char*)vr->Data,vr->Length);
		strTmp[vr->Length]='\0';
		ToUID(strTmp);
		if(strlen(strTmp)==0)
		{
			strcpy(strTmp,"88888888");
		}
nothing2:
		char strSeriesNum[MAX_PATH];
		strSeriesNum[0]='\0';
		vr = DDOPtr1->GetVR(0x0020, 0x0011);//Series number
		if(vr&&vr->Length>0)
		{
			strncpy(strSeriesNum,(char*)vr->Data,vr->Length);
			strSeriesNum[vr->Length]='\0';
		}
		strcat(strTmp,".");
		strcat(strTmp,strSeriesNum);
		ToUID(strTmp);
		char* temp=new char[MAX_PATH];
		strcpy(temp,"1.2.840.113619.");
		strcat(temp,strTmp);
		vr = new VR(0x0020, 0x000E, strlen(temp), (void *)temp, TRUE);
   		DDOPtr1->Push(vr);
	}	
	DDOPtr->Reset();
	while(vr=DDOPtr1->Pop())
	{
		DDOPtr->Push(vr);
	}
	DDOPtr1->Reset();
	delete DDOPtr1;
	return (TRUE);
}
BOOL	GEMRStorage	::	GetUID( UID	& uid )
	{
	uid.Set("1.2.840.113619.4.2");
	return ( TRUE );
	}

BOOL	GECTStorage	::	GetUID ( UID & uid )
	{
	uid.Set("1.2.840.113619.4.3");
	return ( TRUE );
	}



void StandardStorage::SetLastError(UINT32 nLastError)
{
	 LastError = nLastError;
}

UINT32 StandardStorage::GetLastError()
{
	UINT32 nLastError = LastError;
	LastError = STORE_SUCCESS;
	return nLastError;
	
}
