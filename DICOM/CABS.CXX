
/* CO_Abstract and CCO_Abstract support classes */

#	include	"dicom.hpp"
#	include	"iod.hpp"
//将被重载
BOOL
CO_Abstract	::	Morph (
	DICOMObject	*)
	{
	return ( TRUE );	// we dont have any excess
	}
//将被重载

BOOL
CO_Abstract	::	SetIf (
	VR	*vr)
	{
	return ( FALSE );	// we never take any excess
	}
//将被重载

BOOL
CO_Abstract	::	Reset ()
	{
	return ( TRUE );	// and we never reset
	}
//重置数据
BOOL
CCO_Abstract	::	Reset ()
	{
	DICOMObject::Reset();
	return ( TRUE );
	}
//将DICOMObject里面的数据转移到本对象中
BOOL
CCO_Abstract	::	Set (
	DICOMObject	*DO)
	{
	VR	*vr;

	if ( ! DO )
		return ( FALSE );
	while(vr = DO->Pop())
		Push(vr);

	return ( TRUE );
	}

//构造函数
CCO_Abstract	::	CCO_Abstract ()
	{
	ReferentialIntegrityDatabase.LinkUp = this;
	}
//拷贝函数
CCO_Abstract	::	CCO_Abstract (
	DICOMObject	*DO)
	{
	ReferentialIntegrityDatabase.LinkUp = this;
	Set(DO);
	}
//将数据压入本对象
BOOL
CCO_Abstract	::	Push (
	VR	*vr)
	{
	ReferentialIntegrityDatabase.SetMaster ( vr );
	return ( DICOMObject :: Push ( vr ) );
	}
//弹出VR
VR	*
CCO_Abstract	::	Pop ()
	{
	VR	*vr = DICOMObject :: Pop ();

	if ( vr )
		ReferentialIntegrityDatabase.ClearMaster (vr->Group, vr->Element);

	return ( vr );
	}
//同步
BOOL
CCO_Abstract	::	Sync ()
	{
	ReferentialIntegrityDatabase.Sync ();
	return ( TRUE );
	}
//强制转换
CCO_Abstract	::	operator	DICOMDataObject	* ()
	{
	return((DICOMDataObject*)this);
	}
//强制转换

CCO_Abstract	::	operator	DICOMCommandObject	* ()
	{
	return((DICOMCommandObject*)this);
	}

