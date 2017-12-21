#include <time.h>
#include "PrintImage.h"
//打印胶片
//整个打印环节中,包括几个部分
//1个FILM-SESSION包含N个FILMBOX
//每个FILMBOX包含N个IMAGEBOX
//下面是一个典型的打印流程
//A-ASSOCIATE
//N-GET (PRINTER SOP Instance)//此函数略去该过程.
//N-CREATE (Film Session SOP Instance)
//N-CREATE (Film Box SOP Instance)
//for (each image of film)
//{
//	N-SET (Image Box SOP Instance which encapsulates a PREFORMATTED IMAGE SOP Instance)
//}
//N-ACTION (PRINT, Film Box SOP Instance)
//N-DELETE (Film Box SOP Instance)
//N-EVENT-REPORT (PRINTER SOP Instance)
//A-RELEASE
int PrintGrayScaleImage(BOOL bColor,char*IP,char*Port,char*LocalAET,char*RemoteAET, char* ImageNames,char*WinLevels,int nNumberofCopies,char *ImageDisplayFormat, DICOM_PRINT_PRINTPRIORITY ePrintPriority, DICOM_PRINT_MEDIUMTYPE eMediumType, DICOM_PRINT_FILMDESTINATION eFilmDestination, DICOM_PRINT_FILMORIENTATION eFilmOrientation, DICOM_PRINT_FILMSIZEID eFilmSizeID, DICOM_PRINT_MAGNIFICATIONTYPE eMagnificationType, DICOM_PRINT_POLARITY ePolarity)
{
    UID TransferSytaxUID;
	UID aBasicFilmSessionSOPClassUID("1.2.840.10008.5.1.1.1");
	UID aBasicFilmBoxSOPClassUID("1.2.840.10008.5.1.1.2");
	UID aBasicGrayscaleImageBoxSOPClassUID("1.2.840.10008.5.1.1.4");
	UID aPrintJobSOPClassUID("1.2.840.10008.5.1.1.14");
	UID aBasicGrayscalePrintManagementMetaSOPClass("1.2.840.10008.5.1.1.9");
	UID aBasicPrinterSOPClassUID("1.2.840.10008.5.1.1.16");
    MyGrayscalePrintManagement GrayscaleSession;
	//[peak
//	UID aPrinterConfigurationRetrievalSOPClassUID("1.2.840.10008.5.1.1.16.376");
//	UID aPrintQueueManagementSOPClass("1.2.840.10008.5.1.1.26");
	//]
	UID aAppuid("1.2.840.10008.3.1.1.1");
	

	VR *aVR;
	//此处开始收集session的信息
///////////////////////////////////////////////////////
	//压入拷贝张数
	char * strNumberofCopies = new char[MAX_VRIS];
	_itoa(nNumberofCopies, strNumberofCopies, 10);

	aVR = new VR(0x2000, 0x0010, strlen(strNumberofCopies), (void *)strNumberofCopies);
    GrayscaleSession.Push(aVR);

///////////////////////////////////////////////////////
	//压入打印级别
	char * strPrintPriority = new char[MAX_VRIS];
	switch(ePrintPriority) 
	{
		case DICOM_PRINTPRIORITY_HIGH:
			strcpy(strPrintPriority, "HIGH");
			break;
		case DICOM_PRINTPRIORITY_MED:
			strcpy(strPrintPriority, "MED");
			break;
		case DICOM_PRINTPRIORITY_LOW:
			strcpy(strPrintPriority, "LOW");
			break;
	}
	aVR = new VR(0x2000, 0x0020, strlen(strPrintPriority), (void *)strPrintPriority);
    GrayscaleSession.Push(aVR);

///////////////////////////////////////////////////////
	//压入媒介类型
	char * strMediumType = new char[MAX_VRCS];

	switch(eMediumType) 
	{
		case DICOM_MEDIATYPE_PAPER:
			strcpy(strMediumType, "PAPER");
			break;
		case DICOM_MEDIATYPE_CLEARFILM:
			strcpy(strMediumType, "CLEAR FILM");
			break;
		case DICOM_MEDIATYPE_BLUEFILM:
			strcpy(strMediumType, "BLUE FILM");
			break;
	}

	aVR = new VR(0x2000, 0x0030, strlen(strMediumType), (void *)strMediumType);
    GrayscaleSession.Push(aVR);

///////////////////////////////////////////////////////
	//压入打印目的地
	char * strFilmDestination = new char[MAX_VRCS];

	switch(eFilmDestination) 
	{
		case DICOM_FILMDESTINATION_MAGAZINE:
			strcpy(strFilmDestination, "MAGAZINE");
			break;
		case DICOM_FILMDESTINATION_PROCESSOR:
			strcpy(strFilmDestination, "PROCESSOR");
			break;
	}		

	aVR = new VR(0x2000, 0x0040, strlen(strFilmDestination), (void *)strFilmDestination);
    GrayscaleSession.Push(aVR);

	//压入标签
	char* strLabel=new char[256];
	strcpy(strLabel,"COOLPACS System");
	aVR = new VR(0x2000, 0x0050, strlen(strLabel), (void *)strLabel);
    GrayscaleSession.Push(aVR);
	//压入session uid
	char UID[64];
	NewSOPInstanceUID("1.2.840.10008.5.1.1.1",UID);
	GrayscaleSession.itsSOPInstanceUID->Set(UID);

///////////////////////////////////////////////////////
	//此处开始收集FILMBOX的信息
	AbstractFilmBox *aFilmBox = new AbstractFilmBox(GrayscaleSession);

	//打印格式
	char * strImageDisplayFormat = new char[MAX_VRST];
	lstrcpy(strImageDisplayFormat,ImageDisplayFormat);

	aVR = new VR(0x2010, 0x0010, strlen(strImageDisplayFormat), (void *)strImageDisplayFormat);
    aFilmBox->Push(aVR);

///////////////////////////////////////////////////////
	//胶片方向
	char * strFilmOrientation = new char[MAX_VRCS];
	switch(eFilmOrientation) 
	{
		case DICOM_FILMORIENTATION_PORTRAIT:
			strcpy(strFilmOrientation, "PORTRAIT");
			break;
		case DICOM_FILMORIENTATION_LANDSCAPE:
			strcpy(strFilmOrientation, "LANDSCAPE");
			break;
	}

	aVR = new VR(0x2010, 0x0040, strlen(strFilmOrientation), (void *)strFilmOrientation);
    aFilmBox->Push(aVR);

///////////////////////////////////////////////////////
	//胶片大小
	char * strFilmSizeID = new char[MAX_VRCS];
	switch(eFilmSizeID) 
	{
		case DICOM_FILMSIZEID_8INX10IN:
			strcpy(strFilmSizeID, "8INX10IN");
			break;
		case DICOM_FILMSIZEID_10INX12IN:
			strcpy(strFilmSizeID, "10INX12IN");
			break;
		case DICOM_FILMSIZEID_10INX14IN:
			strcpy(strFilmSizeID, "10INX14IN");
			break;
		case DICOM_FILMSIZEID_11INX14IN:
			strcpy(strFilmSizeID, "11INX14IN");
			break;
		case DICOM_FILMSIZEID_14INX14IN:
			strcpy(strFilmSizeID, "14INX14IN");
			break;
		case DICOM_FILMSIZEID_14INX17IN:
			strcpy(strFilmSizeID, "14INX17IN");
			break;
		case DICOM_FILMSIZEID_24CMX24CM:
			strcpy(strFilmSizeID, "24CMX24CM");
			break;
		case DICOM_FILMSIZEID_24CMX30CM:
			strcpy(strFilmSizeID, "24CMX30CM");
			break;
	}

	aVR = new VR(0x2010, 0x0050, strlen(strFilmSizeID), (void *)strFilmSizeID);
    aFilmBox->Push(aVR);

///////////////////////////////////////////////////////
	//当需要放大的时候的放大算法
	char * strMagnificationType = new char[MAX_VRCS];
	switch(eMagnificationType) 
	{
		case DICOM_MAGNIFICATIONTYPE_REPLICATE:
			strcpy(strMagnificationType, "REPLICATE");
			break;
		case DICOM_MAGNIFICATIONTYPE_BILINEAR:
			strcpy(strMagnificationType, "BILINEAR");
			break;
		case DICOM_MAGNIFICATIONTYPE_CUBIC:
			strcpy(strMagnificationType, "CUBIC");
			break;
		case DICOM_MAGNIFICATIONTYPE_NONE:
			strcpy(strMagnificationType, "NONE");
			break;
	}
	aVR = new VR(0x2010, 0x0060, strlen(strMagnificationType), (void *)strMagnificationType);
    aFilmBox->Push(aVR);

//Requested Resolution ID (2020,0050) 
//STANDARD = approximately 4k x 5k printable
//pixels on a 14 x 17 inch film
//HIGH = Approximately twice the resolution of
//STANDARD.
//	char * Resolution = new char[MAX_VRCS];
//	strcpy(Resolution,"HIGH");
//	aVR = new VR(0x2020, 0x0050, strlen(Resolution), (void *)Resolution);
//  pAbstractFilmBox->Push(aVR);

	//filmbox uid
	NewSOPInstanceUID("1.2.840.10008.5.1.1.2",UID);
	aFilmBox->itsSOPInstanceUID->Set(UID);
//////////////////////////////////
	//此处开始收集IMAGEBOX的信息

	//分析出每个图象路径
	Array<string>arImageName;
	string sImageName(ImageNames),s1(",");
	string::size_type pos;
	while((pos=sImageName.find(s1,0))!=string::npos)
	{
		arImageName.Add(sImageName.substr(0,pos));
		sImageName=sImageName.substr(pos+1,sImageName.length());
	}
	for(unsigned int i=0;i<arImageName.GetSize();i++)
	{
		//读取bmp文件信息,包括图象大小和图象数据
		char strFilePath[MAX_PATH];
		strcpy(strFilePath,(char*)arImageName.Get(i).c_str());
		BYTE*pOut=NULL;
		int nWidth,nHeight;

		if(strlen(strFilePath)==0)
			continue;
		FILE*Fp=fopen(strFilePath,"rb");
		if(Fp==NULL)
			continue;
		BITMAPFILEHEADER hdr;
		BITMAPINFOHEADER* pbmih=(BITMAPINFOHEADER*)new BYTE[sizeof(BITMAPINFOHEADER)+256*sizeof (RGBQUAD)];
		fread(&hdr,sizeof(hdr),1,Fp);
		if(hdr.bfType!=0x4d42)
		{
			fclose(Fp);
			delete pbmih;
			continue;
		}
		fread(pbmih,sizeof(DWORD),1,Fp);
		fread((BYTE*)pbmih+sizeof(DWORD),pbmih->biSize-sizeof(DWORD),1,Fp);
		if(pbmih->biClrUsed>256)
		{
			fclose(Fp);
			delete pbmih;
			continue;
		}
		if(pbmih->biClrUsed==0&&pbmih->biBitCount==8)
		{
			pbmih->biClrUsed=256;
		}
		if(pbmih->biClrUsed>0)
		{
			fread((BYTE*)pbmih+pbmih->biSize,pbmih->biClrUsed*sizeof (RGBQUAD),1,Fp);
		}
	//	bmih.biSizeImage=(bmih.biWidth*3+3)>>2<<2;
	//	bmih.biSizeImage*=bmih.biHeight;
		if(pbmih->biSizeImage==0)
		{
			pbmih->biSizeImage=((pbmih->biWidth*pbmih->biBitCount+31)>>5)<<2;
			pbmih->biSizeImage*=pbmih->biHeight;
		}
		nWidth=pbmih->biWidth;
		nHeight=pbmih->biHeight;
		pOut=new BYTE[pbmih->biSizeImage];
		fread(pOut,pbmih->biSizeImage,1,Fp);
		fclose(Fp);

		//将24位或者8位的数据转为正常的由上到下扫描的8位数据
		int nLine=((pbmih->biWidth*pbmih->biBitCount+31)>>5)<<2;
		int nBB=pbmih->biBitCount>>3;
		if(pbmih->biBitCount>8)
		{
			for(int m=0;m<nWidth;m++)
			{
				for(int n=0;n<nHeight;n++)
				{
					pOut[m+(nHeight-1-n)*nWidth]=pOut[nBB*m+n*nLine];
				}
			}
		}
		else
		{
			BYTE*pOutBk=new BYTE[nWidth*nHeight];
			for(int n=0;n<nHeight;n++)
			{
				memcpy(pOutBk+(nHeight-1-n)*nWidth,pOut+n*nLine,nWidth);
			}
			delete pOut;
			pOut=pOutBk;
		}
		delete pbmih;			
		AbstractImageBox *aImageBox = new AbstractImageBox(GrayscaleSession);

		//压入图象位置
		unsigned short * pImageBoxPosition = new unsigned short;
		*pImageBoxPosition = i+1;

		aVR = new VR(0x2020, 0x0010, MAX_VRUS, (void *)pImageBoxPosition);
		aImageBox->Push(aVR);

///////////////////////////////////////////////////////
		//压入极性,正片还是负片
		char * strPolarity = new char[MAX_VRCS];
		switch(ePolarity) 
		{
			case DICOM_POLARITY_NORMAL:
				strcpy(strPolarity, "NORMAL");
				break;
			case DICOM_POLARITY_REVERSE:
				strcpy(strPolarity, "REVERSE");
				break;
		}	
		
		aVR = new VR(0x2020, 0x0020, strlen(strPolarity), (void *)strPolarity);
		aImageBox->Push(aVR);
	/*
		char * strMagnificationType = new char[MAX_VRCS];
		switch(nGlobalZoomType) {
		case DICOM_MAGNIFICATIONTYPE_REPLICATE:
			strcpy(strMagnificationType, "REPLICATE");
			break;
		case DICOM_MAGNIFICATIONTYPE_BILINEAR:
			strcpy(strMagnificationType, "BILINEAR");
			break;
		case DICOM_MAGNIFICATIONTYPE_CUBIC:
			strcpy(strMagnificationType, "CUBIC");
			break;
		case DICOM_MAGNIFICATIONTYPE_NONE:
			strcpy(strMagnificationType, "NONE");
			break;
		}

		aVR = new VR(0x2010, 0x0060, strlen(strMagnificationType), (void *)strMagnificationType);
		pAbstractImageBox->Push(aVR);
	//Requested Decimate/Crop Behavior (2020,0040)
		char * Behavior = new char[MAX_VRCS];
		strcpy(Behavior,"DECIMATE");
		aVR = new VR(0x2020, 0x0040, strlen(Behavior), (void *)Behavior);
		pAbstractImageBox->Push(aVR);
	*/
///////////////////////////////////////////////////////

		// create the PreformattedGrayscaleImageSequence attribute
		aVR = new VR(0x2020, 0x0110, 0, FALSE);
    	aImageBox->Push(aVR);

///////////////////////////////////////////////////////
	// create a DICOMDataObject containing the Preformatted
	// Grayscale Image and then Push() that onto the AbstractImageBox

		
		DICOMDataObject *aDDO = new DICOMDataObject;

///////////////////////////////////////////////////////
		//每个象素多少个取样点
		unsigned short * pSamplesPerPixel = new unsigned short;
		*pSamplesPerPixel = 1;

		aVR = new VR(0x0028, 0x0002, MAX_VRUS, (void *) pSamplesPerPixel);
		aDDO->Push(aVR);

///////////////////////////////////////////////////////
		//图象表示
		char * strPhotometricInterpretation = new char[MAX_VRCS];
		strcpy(strPhotometricInterpretation,"MONOCHROME2");
		aVR = new VR(0x0028, 0x0004, strlen(strPhotometricInterpretation), (void *)strPhotometricInterpretation);
   		aDDO->Push(aVR);

///////////////////////////////////////////////////////
		//行
		unsigned short * pRows = new unsigned short;
		*pRows = nHeight;

		aVR = new VR(0x0028, 0x0010, MAX_VRUS, (void *)pRows);
		aDDO->Push(aVR);

///////////////////////////////////////////////////////
		//列
		unsigned short * pColumns = new unsigned short;
		*pColumns = nWidth;

		aVR = new VR(0x0028, 0x0011, MAX_VRUS, (void *)pColumns);
		aDDO->Push(aVR);

///////////////////////////////////////////////////////
		//象素比例
		unsigned short nPixeAspectRatioLen = MAX_VRIS * 2;
		char * strPixeAspectRatio = new char[nPixeAspectRatioLen];
		lstrcpy(strPixeAspectRatio, "1\\1");

		aVR = new VR(0x0028, 0x0034, strlen(strPixeAspectRatio), (void *)strPixeAspectRatio);
		aDDO->Push(aVR);

///////////////////////////////////////////////////////
		//每个取样点分配为多少位
		unsigned short * pBitsAllocated = new unsigned short;
		*pBitsAllocated = 8;
		
		aVR = new VR(0x0028, 0x0100, MAX_VRUS, (void *)pBitsAllocated);
		aDDO->Push(aVR);

///////////////////////////////////////////////////////
		//每个取样点存储多少位
		unsigned short * pBitsStored = new unsigned short;
		*pBitsStored = 8;

		aVR = new VR(0x0028, 0x0101, MAX_VRUS, (void *)pBitsStored);
		aDDO->Push(aVR);

///////////////////////////////////////////////////////
		//最高位
		unsigned short * pHighBit = new unsigned short;
		*pHighBit = 7;

		aVR = new VR(0x0028, 0x0102, MAX_VRUS, (void *)pHighBit);
		aDDO->Push(aVR);

///////////////////////////////////////////////////////
		//象素值表达,是否有符号整数
		unsigned short * pPixelRepresentation = new unsigned short;
		*pPixelRepresentation = 0;

		aVR = new VR(0x0028, 0x0103, MAX_VRUS, (void *)pPixelRepresentation);
		aDDO->Push(aVR);

///////////////////////////////////////////////////////
			// NOTE: the memory pointed to by anImage is never
			// released -- memory leak -- need to hold ptr in an array
			// for later delete...
		//压入图象数据
		aVR = new VR(0x7fe0, 0x0010, nWidth*nHeight, (void *)pOut);
		aDDO->Push(aVR);

///////////////////////////////////////////////////////
	// add the sequence for the most recent item pushed

		aImageBox->Push(aDDO);


		aFilmBox->Add(aImageBox);
	}
	GrayscaleSession.Add(aFilmBox);
	//上面已经准备好数据.下面将连接对方,将数据送过去
	// create a run-time classer for the PDU_Service to use
	// in making explicit VR's out of implicit ones
	// tell RTC to keep descriptions and the dictionary name
//[peak


	PDU_Service pdu;
	// add the abstract syntaxes we need to use in the association
	//添加应用上下文
    pdu.SetApplicationContext(aAppuid);

	//添加需要对方支持的类
	pdu.AddAbstractSyntax(aBasicGrayscalePrintManagementMetaSOPClass);


//    pPDU->AddAbstractSyntax(aPrintJobSOPClassUID);
	//peak
  //  pPDU->AddAbstractSyntax(aPrinterConfigurationRetrievalSOPClassUID);
    //pPDU->AddAbstractSyntax(aPrintQueueManagementSOPClass);
	//peak
	// set application titles (MIR_PRINT_SCU for test with CTN)

	//添加本方的AET和对方的AET
    pdu.SetLocalAddress((BYTE *) (LPCTSTR) LocalAET);
    pdu.SetRemoteAddress((BYTE *) (LPCTSTR) RemoteAET);

	//开始连接
	if(!pdu.Connect((BYTE *) (LPCTSTR) IP, (BYTE *)Port))
	{
		return -16;
	}
	//看看类是否被接受,同时返回传输语法
	if(!pdu.IsAbstractSyntaxAccepted(aBasicGrayscalePrintManagementMetaSOPClass, TransferSytaxUID)) 
	{
		return -18;
	}

	// add the abstract syntaxes making up the
	// BasicGrayscalePrintManagementMetaSOPClass
	// as aliases so that PDU can find a presentation context to use for them

	//添加支持的类uid
	pdu.AddAbstractSyntaxAlias(aBasicGrayscalePrintManagementMetaSOPClass,
		aBasicFilmSessionSOPClassUID);
	pdu.AddAbstractSyntaxAlias(aBasicGrayscalePrintManagementMetaSOPClass,
		aBasicFilmBoxSOPClassUID);
	pdu.AddAbstractSyntaxAlias(aBasicGrayscalePrintManagementMetaSOPClass,
		aBasicGrayscaleImageBoxSOPClassUID);
	pdu.AddAbstractSyntaxAlias(aBasicGrayscalePrintManagementMetaSOPClass,
		aBasicPrinterSOPClassUID);

		// set an argument for event handler callbacks to use

	//	GrayscaleSession.SetPrinterEventUserArg((void *)&aRTC);
	//	GrayscaleSession.SetPrintJobEventUserArg((void *)&aRTC);
	// attempt to get printer status

//	DICOMDataObject aStatusDDO;
/*
	cerr << "Request printer status:" << endl;
	if(!GrayscaleSession.GetPrinter(&PDU, &aStatusDDO))
	{
		cerr << "GetPrinter() failed." << endl;
		exit(252);
	}

	cerr << "Got printer status!" << endl;
	DumpDDO(&aStatusDDO, 0, &aRTC);
	cerr << "Got printer status!" << endl << endl;
*/
	// all supposedly optional parameters


	// optional MemoryAllocation (not set)

	// Film box parameters:
	// Image display format STANDARD\2,1
	// FilmOrientation = PORTRAIT
	// FilmSizeID 8INX10IN
	// MagnificationType = REPLICATE
	// BorderDensity = BLACK
	// EmptyImageDensity = BLACK
	// print management object handles reference sequences for us...

//	DICOMDataObject ddo;
//	BOOL b=GrayscaleSession.GetPrinterInfo(pPDU,&ddo);
//	FILE*pF=fopen("d:\\ddo.txt","a+");
//	DumpDDO(pF,&ddo,0,pPDU->AttachedRTC);
//	fclose(pF);
	//写入数据,打印
	if(!GrayscaleSession.PrintFilm(&pdu))
	{
		return -38;
	}
	//由于不需要再打印,先删除FILMBOX
	if(!GrayscaleSession.DeleteFilm(&pdu))
	{
		return -40;
	}
	//删除整个FILM SESSION
	if(!GrayscaleSession.Delete(&pdu))
	{
		return -44;
	}
//	ddo.Reset();
//	GrayscaleSession.ReadEventReport (pPDU,(DICOMCommandObject*)&ddo);

	// terminate the association
	//关闭连接
	pdu.Close();
	return 1;
}
