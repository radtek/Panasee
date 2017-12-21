// Base classes for all Chapter 3 Composite and regular Complex IOD modules
//本文件定义一些复合对象基础类
//对于这些对象,都有拷贝,设置,重置三个方法
//对于dicom通信对象,还添加了读写类.
class	CO_Abstract
	{
	public:
		virtual	BOOL	Morph(DICOMObject *);
		virtual	BOOL	SetIf(VR *);
		BOOL	Reset ();
	};


class	CCO_Abstract	:
		public	DICOMObject
	{
	// In a strict sense, should not be public.  But, otherwise the CCOC
	// Compiler needs to generate "friend" code which I have concerns about
	// for consistent implementations across the various compilers...
	// declaring this public is only a minor tweak anyways..
	public:
		VRReferentialIntegrityDatabase	ReferentialIntegrityDatabase;
	public:
		BOOL		Reset ();
		BOOL		Set ( DICOMObject * );
					CCO_Abstract ();
					CCO_Abstract ( DICOMObject * );		
		// Overloaded so that a Sync can be performed
		BOOL		Push ( DICOMObject *SQDO ) {return ( DICOMObject :: Push ( SQDO ) );}; //missing, add by Lin Tianyi
		BOOL		Push ( VR * );
		VR			*Pop ();
		BOOL		Sync ();
		BOOL		SetIf ( DICOMObject * );
		BOOL		Morph ( DICOMObject * );
					operator DICOMDataObject * ();
					operator DICOMCommandObject * ();
	};

