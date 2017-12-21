#include "MyQueryRetrievePDU.h"

//获取可接受的TransferSyntax
BOOL MyQueryRetrievePDU::GetAcceptedTransferSyntax (UID &uid)
{
	PresentationContextAccept	PCA;

	if(AcceptedPresentationContexts.GetSize() < 1)
		return FALSE;

	PCA = AcceptedPresentationContexts.Get ( 0 );

	uid = PCA.TrnSyntax.TransferSyntaxName;

	return TRUE;
}

//添加传输语法
BOOL MyQueryRetrievePDU::AddTransferSyntaxs (PresentationContext& PresContext)
{
	if (m_bIsQuery == TRUE)
//TRUE means the current operation is Query
	{
		UID	uid1;
		TransferSyntax	TrnSyntax1;

//默认传输语法
//DICOM Implicit VR Little Endian传输语法
//1.2.840.10008.1.2
		uid1.Set("1.2.840.10008.1.2");
		TrnSyntax1.Set(uid1);
		PresContext.AddTransferSyntax(TrnSyntax1);
	}
	else
//FALSE means the current operation is Retrieve
	{
		UID	uid1;
		TransferSyntax	TrnSyntax1;


//默认传输语法
//DICOM Implicit VR Little Endian传输语法
//1.2.840.10008.1.2
		uid1.Set("1.2.840.10008.1.2");
		TrnSyntax1.Set(uid1);
		PresContext.AddTransferSyntax(TrnSyntax1);

		UID	uid2;
		TransferSyntax	TrnSyntax2;

//DICOM Explicit VR Little Endian传输语法
//1.2.840.10008.1.2.1
		uid2.Set("1.2.840.10008.1.2.1");
		TrnSyntax2.Set(uid2);
		PresContext.AddTransferSyntax(TrnSyntax2);

		/*UID	uid3;
		TransferSyntax	TrnSyntax3;

		uid3.Set("1.2.840.10008.1.2.5");
		TrnSyntax3.Set(uid3);
		PresContext.AddTransferSyntax(TrnSyntax3);*/
	}
	return ( TRUE );
}

void MyQueryRetrievePDU::SetQueryOrRetrieve (BOOL bIsQuery)
{
//TRUE means the current operation is Query
//FALSE means the current operation is Retrieve
	m_bIsQuery = bIsQuery;
}
