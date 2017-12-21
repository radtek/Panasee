

#	include	"dicom.hpp"

// Disk File I/O for PDU_Service Class
//打开文件读
BOOL
FileBuffer	::	OpenForRead ( char	*filename )
	{
#ifndef	WINDOWS
	fp = fopen(filename, "rb");
#else
	// Open Shared mode for Windows.  Unix will allow sharing by default
	fp = _fsopen(filename, "rb", _SH_DENYWR);
#endif
	if( fp )
		return ( TRUE );
	return ( FALSE );
	}
//打开文件写
BOOL
FileBuffer	::	OpenForWrite ( char	*filename )
	{
	fp = fopen(filename, "wb");
	if ( fp )
		return ( TRUE );
	return ( FALSE );
	}
//关闭文件
BOOL
FileBuffer	::	Close ()
	{
		if(fp!=NULL&&bMustClose)
		{
			fclose(fp);
		}
		fp=NULL;
		return ( TRUE );
	}
//写数据
BOOL
FileBuffer	::	SendBinary ( BYTE	*Data, UINT	Length )
	{
	fwrite((char*)Data, 1, Length, fp);
	return ( TRUE );
	}
//读数据
INT
FileBuffer	::	ReadBinary ( BYTE	*Data, UINT	Length )
	{
	INT		Ret;

	//fprintf(stderr, "ReadBinary(Data, %d) : ", Length);
	if(feof(fp))
		{
		//fprintf(stderr, " -1\n");
		return ( -1 );
		}
	Ret = (INT)fread((char*)Data, 1, Length, fp);
	//fprintf(stderr, " %d\n", Ret);
	return ( Ret );
	}
//加载DICOM文件
DICOMDataObject	*
PDU_Service	::	LoadDICOMDataObject	(
	char	*filename )
	{
	FileBuffer		IOBuffer;
	FILE			*fp;
	fp=NULL;
	char			s[256];
	DICOMDataObject	*DDO;
	DDO=NULL;
	UINT			Mode;
	UINT			CheckOffset;
	IOBuffer.bMustClose=FALSE;
try
{	
	fp = fopen(filename, "rb");
	if(!fp)
		return ( NULL );
	if(fseek(fp, 128, SEEK_SET))
	{
		fclose(fp);
		return (NULL);
	}
	if(fread(s, 1, 4, fp) < 4)
	{
		fclose(fp);
		return (NULL);
	}
	s[4] = '\0';
	if(strcmp(s, "DICM"))
		{
		CheckOffset = 0;
		fseek(fp, 0, SEEK_SET);
		}
	else
		CheckOffset = 128+4;

	if(fread(s,1,6,fp) < 6)
	{
		fclose(fp);
		return (NULL);
	}
	if(s[5]>10)
		Mode = TSA_EXPLICIT_LITTLE_ENDIAN;
	else
		Mode = TSA_IMPLICIT_LITTLE_ENDIAN;
	if(fseek(fp, CheckOffset, SEEK_SET))
	{
		fclose(fp);
		return (NULL);
	}
		
	IOBuffer.fp = fp;
	while(IOBuffer.Buffer :: Fill(50000))
		;	// still reading from disk

	DDO = new DICOMDataObject;

	//fprintf(stderr, "Attemping to Parse...\n\n");

	//Modified by Lin
	if(!Dynamic_ParseRawVRIntoDCM(IOBuffer, DDO,
		Mode))
		{
		delete DDO;
		DDO = 0x00000000;
		}
	IOBuffer.Close();
	if(fp)
		fclose(fp);
	fp=NULL;
	return ( DDO );
}
catch(...)
{
	if(fp)
		fclose(fp);
	if(DDO)
		delete DDO;
	return NULL;
}
}


//#	define	_ImplementationUID	"none yet"
//#	define	_ImplementationVersion	"0.1AlphaUCDMC "
//#	define	_SourceApplicationAE	"none            "
//生成dicom文件头
BOOL
PDU_Service	::	MakeFileMeta (DICOMDataObject	*DDOPtr, UINT Format)
	{
	VR		*vr, *vr1;
	UINT16	tuint16;
//	VR	*TempVR;
//	char	s[140];
	char	_ImplementationUID[128];
	char	_ImplementationVersion[128];
	char	_SourceApplicationAE[32];
	char	_TransferSyntax[128];
	ImplementationVersion	IV;
	ImplementationClass		IC;
	DICOMDataObject			DDOTemp;

	GetImplementationClass(IC);
	GetImplementationVersion(IV);
	strcpy(_ImplementationUID, (char*)IC.ImplementationName.GetBuffer(1));
	strcpy(_ImplementationVersion, (char*)IV.Version.GetBuffer(1));
	strcpy(_SourceApplicationAE, (char*)AAssociateRQ::CallingApTitle);
	MTrimRight(_SourceApplicationAE);
	if(strlen(_SourceApplicationAE)==0)
	{
		strcpy(_SourceApplicationAE,"COOLPACS");
	}
	// Strip away the existing Part 10 header..
	while(vr = DDOPtr->Pop())
		{
		if(vr->Group!=0x0002)
			DDOTemp.Push(vr);
		else
			delete vr;
		}
	while(vr = DDOTemp.Pop())
		DDOPtr->Push(vr);

//	vr = DDOPtr->GetVR(0x0002, 0x0001);	// always null
//	if(!vr)
		{
		// This does not contain the C10 Header yet, so we need to
		// constuct it
		vr = new VR(0x0002, 0x0001, 0x0002, TRUE);
		tuint16 = 0x0001;
		memcpy(vr->Data, (void*)&tuint16, 2);
		/* First byte should be 0, second should be 1
		   Thanx to William Peterson [wfpeterson@kinetixresources.com]
		*/
		//if uncomment below,osiris and dump can not recognize it,so use original source.peak
//		((char*)vr->Data)[0] = 0;
//		((char*)vr->Data)[1] = 1;

		vr->TypeCode = 'OB';//this is commented by lin

		switch(Format)
			{
			case DICOM_CHAPTER_10_IMPLICIT:
				break;
			case DICOM_MEDIA_EXPLICITLITTLEENDIAN:
			case DICOM_MEDIA_IMPLICITLITTLEENDIAN:
				vr->TypeCode = 'OB';
				break;
			case DICOM_MEDIA_JPEGLOSSLESSDEFAULT:
				strcpy(_TransferSyntax, _JpgLosslessDefaultUID);
				break;
			case DICOM_MEDIA_JPEGLOSSLESS:
				strcpy(_TransferSyntax, _JpgLosslessUID);
				break;
			case DICOM_MEDIA_JPEGLS:
				strcpy(_TransferSyntax, _JpgLSUID);
				break;
			case DICOM_MEDIA_JPEG2K:
				strcpy(_TransferSyntax, _Jpg2KUID);
				break;
			case DICOM_MEDIA_RLELOSSLESS:
				strcpy(_TransferSyntax, _RleLosslessUID);
				break;
			}
		DDOPtr->Push(vr);
		vr = DDOPtr->GetVR(0x0008, 0x0016);
		if(vr)
			{
			vr1 = new VR(0x0002, 0x0002, vr->Length, TRUE);
			memcpy(vr1->Data, vr->Data, vr->Length);
			vr1->TypeCode = 'UI'; //edit by peak
//			vr->TypeCode = 'UI';
			DDOPtr->Push(vr1);
			}
		vr = DDOPtr->GetVR(0x0008, 0x0018);
		if(vr)
			{
			vr1 = new VR(0x0002, 0x0003, vr->Length, TRUE);
			memcpy(vr1->Data, vr->Data, vr->Length);
			vr1->TypeCode = 'UI';
//			vr->TypeCode = 'UI';
			DDOPtr->Push(vr1);
			}
		switch(Format)
			{
		case DICOM_CHAPTER_10_IMPLICIT:
			strcpy(_TransferSyntax, _ImplicitLittleEndianUID);
			break;
//		case DICOM_CHAPTER_10_EXPLICIT:
		case DICOM_MEDIA_IMPLICITLITTLEENDIAN:
			strcpy(_TransferSyntax, _ImplicitLittleEndianUID);
			break;
		case DICOM_MEDIA_EXPLICITLITTLEENDIAN:
			strcpy(_TransferSyntax, _ExplicitLittleEndianUID);
			break;
		case DICOM_MEDIA_RLELOSSLESS:
			strcpy(_TransferSyntax, _RleLosslessUID);
			break;
			}
		vr = new VR(0x0002, 0x0010, strlen(_TransferSyntax), TRUE);
		memset(vr->Data, 0, vr->Length);
		memcpy(vr->Data, (void*)_TransferSyntax, strlen(_TransferSyntax));
		vr->TypeCode = 'UI';
		DDOPtr->Push(vr);
		vr = new VR(0x0002, 0x0012, strlen(_ImplementationUID), TRUE);
		memset(vr->Data, 0, vr->Length);
		memcpy(vr->Data, (void*)_ImplementationUID, strlen(_ImplementationUID));
		vr->TypeCode = 'UI';
		DDOPtr->Push(vr);
		vr = new VR(0x0002, 0x0013, strlen(_ImplementationVersion), TRUE);
		memset(vr->Data, 0, vr->Length);
		memcpy(vr->Data, (void*)_ImplementationVersion, strlen(_ImplementationVersion));
		vr->TypeCode = 'SH';
		DDOPtr->Push(vr);
		vr = new VR(0x0002, 0x0016, strlen(_SourceApplicationAE), TRUE);
		memset(vr->Data, 0, vr->Length);
		memcpy(vr->Data, (void*)_SourceApplicationAE, strlen(_SourceApplicationAE));
		vr->TypeCode = 'AE';
		DDOPtr->Push(vr);
		}
	return ( TRUE );
	}

//保存dicom文件。返回该序列的图像数量
int PDU_Service	::	SaveDICOMDataObject (
	char	*filename,
	UINT	Format,
	DICOMDataObject	*DDO ,
	int *pnImgNo ,
	BYTE* lpDcm,
	int& nDataLen)
{
	FileBuffer		IOBuffer;
	FILE			*fp;
	fp=NULL;
	char			s[264];
	UINT32			upper;
	UINT32			lower;
	UINT			nFileSize;

	IOBuffer.bMustClose = FALSE;	
	nDataLen = 0 ;
	
	int nImgNumOfSeries = 0 ;
	try
	{
		switch ( Format )
		{
			case	ACRNEMA_VR_DUMP:
				fp = fopen(filename, "wb");
				if(!fp)
					return ( FALSE );
				ImplicitLittleEndian_ParseDCMIntoRawVR(DDO, IOBuffer);
				IOBuffer.fp = fp;
				IOBuffer.Buffer::Flush();
				IOBuffer.Close();
				if(fp!=NULL)
					fclose(fp);
				fp=NULL;
				return ( TRUE );
			case	DICOM_CHAPTER_10_IMPLICIT:
				fp = fopen(filename, "wb");
				if(!fp)
					return ( FALSE );
				MakeFileMeta(DDO, DICOM_CHAPTER_10_IMPLICIT);
				ImplicitLittleEndian_ParseDCMIntoRawVR(DDO, IOBuffer);
				memset((void*)s, 0, 256);
				strcpy(&s[128], "DICM");
				fwrite(s, 1, 128 + 4, fp);
				IOBuffer.fp = fp;
				IOBuffer.Buffer::Flush();
				IOBuffer.Close();
				if(fp!=NULL)
					fclose(fp);
				fp=NULL;
				return ( TRUE );
			case DICOM_MEDIA_IMPLICITLITTLEENDIAN:
			case DICOM_MEDIA_JPEGLOSSLESSDEFAULT:
			case DICOM_MEDIA_JPEGLOSSLESS:
			case DICOM_MEDIA_JPEGLS:
			case DICOM_MEDIA_JPEG2K:

	//			MakeFileMeta(DDO, DICOM_CHAPTER_10_EXPLICIT);
				if(!MakeFileMeta(DDO, Format))
					return FALSE;
				
				nImgNumOfSeries = Dynamic_ParseDCMIntoRawVR(DDO, IOBuffer, 
					                                        TSA_EXPLICIT_LITTLE_ENDIAN,
															pnImgNo);
				/*
				if(nImgNumOfSeries < 0)
				{
		//		if(!Dynamic_ParseDCMIntoRawVR(DDO, IOBuffer, TSA_IMPLICIT_LITTLE_ENDIAN))
					return FALSE;
				}*/

				if(!GetFreeDiskSpace(filename, upper, lower)) //Get the space of the disk that contains the filename
				{
					return FALSE;
				}
                

				nFileSize = IOBuffer.GetOutgoingSize();

				nFileSize = nFileSize + 128 + 4;

				if( upper == 0)
				{
					if(lower <= nFileSize) 
					{
						PDUSetLastError(PDU_SAVE_NOFREEDISKSPACE);
						return FALSE;
					}
				}

				BYTE *lpTem;

				memset((void*)s, 0, 256);
				strcpy(&s[128], "DICM");
				memcpy(lpDcm,s,132);
//				IOBuffer.fp = fp;
                lpTem = lpDcm + 132 ;
				IOBuffer.Buffer::Flush(lpTem ,nDataLen);
				nDataLen += 132 ;
		 
				try
				{
					fp = fopen(filename, "wb");
					if(fp)
					{
						fwrite(lpDcm,1,nDataLen,fp);
						fclose(fp);
					}
					fp=NULL;
				}catch(...)
				{
				}

		
				return nImgNumOfSeries;
			case	DICOM_MEDIA_EXPLICITLITTLEENDIAN:

				if(!MakeFileMeta(DDO, DICOM_MEDIA_EXPLICITLITTLEENDIAN))
					return FALSE;
				
				if(!Explicit_ParseDCMIntoRawVR(DDO, IOBuffer))
					return FALSE;

				if(!GetFreeDiskSpace(filename, upper, lower))
					return FALSE;

				nFileSize = IOBuffer.GetOutgoingSize();

				nFileSize = nFileSize + 128 + 4;

				if( upper == 0)
					if(lower <= nFileSize) {
						PDUSetLastError(PDU_SAVE_NOFREEDISKSPACE);
						return FALSE;
					}

				fp = fopen(filename, "wb");
				if(!fp)
					return ( FALSE );

				memset((void*)s, 0, 256);
				strcpy(&s[128], "DICM");
				fwrite(s, 1, 128 + 4, fp);
				IOBuffer.fp = fp;
				IOBuffer.Buffer::Flush();
				IOBuffer.Close();
				if(fp!=NULL)
					fclose(fp);
				fp=NULL;
				return ( TRUE );

			case	DICOM_MEDIA_RLELOSSLESS:

				if(!MakeFileMeta(DDO, DICOM_MEDIA_RLELOSSLESS))
					return ( FALSE );

				if(!Explicit_ParseDCMIntoRawVR(DDO, IOBuffer))
					return ( FALSE );

				if(!GetFreeDiskSpace(filename, upper, lower))
					return ( FALSE );

				nFileSize = IOBuffer.GetOutgoingSize();

				nFileSize = nFileSize + 128 + 4;

				if( upper == 0)
					if(lower <= nFileSize) {
						PDUSetLastError(PDU_SAVE_NOFREEDISKSPACE);
						return ( FALSE );
					}

				fp = fopen(filename, "wb");
				if(!fp)
					return ( FALSE );

				memset((void*)s, 0, 256);
				strcpy(&s[128], "DICM");
				fwrite(s, 1, 128 + 4, fp);
				IOBuffer.fp = fp;
				IOBuffer.Buffer::Flush();
				IOBuffer.Close();
				if(fp!=NULL)
					fclose(fp);
				fp=NULL;
				return ( TRUE );
		}
	}
	catch(...)
	{
		if(fp!=NULL)
			fclose(fp);
		fp=NULL;
	}
	return ( FALSE );
}

//保存dicom文件
BOOL PDU_Service::SaveDICOMDataObject(
									  char	*filename,
									  UINT	Format,
									  DICOMDataObject	*DDO )
{
	FileBuffer		IOBuffer;
	FILE			*fp;
	fp=NULL;
	char			s[264];
	UINT32			upper;
	UINT32			lower;

	UINT			nFileSize;
	IOBuffer.bMustClose=FALSE;			
	try
	{
		switch ( Format )
		{
			case	ACRNEMA_VR_DUMP:
				fp = fopen(filename, "wb");
				if(!fp)
					return ( FALSE );
				ImplicitLittleEndian_ParseDCMIntoRawVR(DDO, IOBuffer);
				IOBuffer.fp = fp;
				IOBuffer.Buffer::Flush();
				IOBuffer.Close();
				if(fp!=NULL)
					fclose(fp);
				fp=NULL;
				return ( TRUE );
			case	DICOM_CHAPTER_10_IMPLICIT:
				fp = fopen(filename, "wb");
				if(!fp)
					return ( FALSE );
				MakeFileMeta(DDO, DICOM_CHAPTER_10_IMPLICIT);
				ImplicitLittleEndian_ParseDCMIntoRawVR(DDO, IOBuffer);
				memset((void*)s, 0, 256);
				strcpy(&s[128], "DICM");
				fwrite(s, 1, 128 + 4, fp);
				IOBuffer.fp = fp;
				IOBuffer.Buffer::Flush();
				IOBuffer.Close();
				if(fp!=NULL)
					fclose(fp);
				fp=NULL;
				return ( TRUE );
			case DICOM_MEDIA_IMPLICITLITTLEENDIAN:
			case DICOM_MEDIA_JPEGLOSSLESSDEFAULT:
			case DICOM_MEDIA_JPEGLOSSLESS:
			case DICOM_MEDIA_JPEGLS:
			case DICOM_MEDIA_JPEG2K:

	//			MakeFileMeta(DDO, DICOM_CHAPTER_10_EXPLICIT);
				if(!MakeFileMeta(DDO, Format))
					return FALSE;
				int nImgNo ;
				if(!Dynamic_ParseDCMIntoRawVR(DDO, IOBuffer, TSA_EXPLICIT_LITTLE_ENDIAN,&nImgNo))
		//		if(!Dynamic_ParseDCMIntoRawVR(DDO, IOBuffer, TSA_IMPLICIT_LITTLE_ENDIAN))
					return FALSE;

				if(!GetFreeDiskSpace(filename, upper, lower)) //Get the space of the disk that contains the filename
					return FALSE;


				nFileSize = IOBuffer.GetOutgoingSize();

				nFileSize = nFileSize + 128 + 4;

				if( upper == 0)
					if(lower <= nFileSize) {
						PDUSetLastError(PDU_SAVE_NOFREEDISKSPACE);
						return FALSE;
					}



				fp = fopen(filename, "wb");
				if(!fp)
					return ( FALSE );

				memset((void*)s, 0, 256);
				strcpy(&s[128], "DICM");
				fwrite(s, 1, 128 + 4, fp);

				IOBuffer.fp = fp;
				IOBuffer.Buffer::Flush();
				IOBuffer.Close();
				if(fp!=NULL)
					fclose(fp);
				fp=NULL;

				return ( TRUE );
			case	DICOM_MEDIA_EXPLICITLITTLEENDIAN:

				if(!MakeFileMeta(DDO, DICOM_MEDIA_EXPLICITLITTLEENDIAN))
					return FALSE;
				
				if(!Explicit_ParseDCMIntoRawVR(DDO, IOBuffer))
					return FALSE;

				if(!GetFreeDiskSpace(filename, upper, lower))
					return FALSE;

				nFileSize = IOBuffer.GetOutgoingSize();

				nFileSize = nFileSize + 128 + 4;

				if( upper == 0)
					if(lower <= nFileSize) {
						PDUSetLastError(PDU_SAVE_NOFREEDISKSPACE);
						return FALSE;
					}

				fp = fopen(filename, "wb");
				if(!fp)
					return ( FALSE );

				memset((void*)s, 0, 256);
				strcpy(&s[128], "DICM");
				fwrite(s, 1, 128 + 4, fp);
				IOBuffer.fp = fp;
				IOBuffer.Buffer::Flush();
				IOBuffer.Close();
				if(fp!=NULL)
					fclose(fp);
				fp=NULL;
				return ( TRUE );

			case	DICOM_MEDIA_RLELOSSLESS:

				if(!MakeFileMeta(DDO, DICOM_MEDIA_RLELOSSLESS))
					return ( FALSE );

				if(!Explicit_ParseDCMIntoRawVR(DDO, IOBuffer))
					return ( FALSE );

				if(!GetFreeDiskSpace(filename, upper, lower))
					return ( FALSE );

				nFileSize = IOBuffer.GetOutgoingSize();

				nFileSize = nFileSize + 128 + 4;

				if( upper == 0)
					if(lower <= nFileSize) {
						PDUSetLastError(PDU_SAVE_NOFREEDISKSPACE);
						return ( FALSE );
					}

				fp = fopen(filename, "wb");
				if(!fp)
					return ( FALSE );

				memset((void*)s, 0, 256);
				strcpy(&s[128], "DICM");
				fwrite(s, 1, 128 + 4, fp);
				IOBuffer.fp = fp;
				IOBuffer.Buffer::Flush();
				IOBuffer.Close();
				if(fp!=NULL)
					fclose(fp);
				fp=NULL;
				return ( TRUE );
		}
	}
	catch(...)
	{
		if(fp!=NULL)
			fclose(fp);
		fp=NULL;
	}
	return ( FALSE );
}
