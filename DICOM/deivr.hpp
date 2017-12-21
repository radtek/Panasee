
class	DICOMObject;
//VR是一个很重要的类
//VR是Value Representation的缩写
//该类模拟了所有数据编码段
//VR的一般格式是(GroupID,ElementID,Type(可能没有),Length,Data)
class	VR
	{
	public:
		UINT16	Group;
		UINT16	Element;
		UINT32	Length;
		void		*Data;
		BOOL		ReleaseMemory;
		BOOL		Valid;
		// void pointers for historical reasons.  SQObjectArray
		// is problem best moved to an Array < DICOMObject * > * in
		// the future once the various system compilers work around
		// the linking problems...  ReferentialIntegrityDatabase should
		// remain void because it is not defined unless the (optional)
		// IOD library is linked in.
		void		*SQObjectArray;
		void		*ReferentialIntegrityDatabase;
		UINT16		TypeCode;
		VR() { Data = NULL; Length = 0; ReleaseMemory = TRUE; TypeCode = 0;
				SQObjectArray = NULL; Valid = FALSE; };
		VR(UINT16 g, UINT16 e, UINT32 l, BOOL	Alloc);
		VR(UINT16 g, UINT16 e, UINT32 l, void	*d);
		VR(UINT16 g, UINT16 e, UINT32 l, void	*d, BOOL);
		~VR();
		UINT		operator	>	(VR	&vr);
		UINT		operator	<	(VR	&vr);
		UINT		operator	==	(VR	&vr);

		// To support SetIf/Morph

		BOOL		SetIf(VR	*);
		BOOL		Morph(DICOMObject	*);
		BOOL		Reset ();
	};
//这个是一个VR的队列
class	VRGroupPQueue	:
	public	PQueueOfPtr<VR	*>
	{
	public:
		UINT16	Group;
		UINT32	Length;
		VR			*Element0;

	public:
		BOOL	Push(VR	*vr);
		VR		*Pop();
		VR		*GroupLength();
		UINT		operator	>	(VRGroupPQueue	&vrgroup);
		UINT		operator	<	(VRGroupPQueue	&vrgroup);
		UINT		operator	==	(VRGroupPQueue	&vrgroup);
		VRGroupPQueue(UINT16	G)	{ Group = G; Length = 0; };
	};
//这个是DICOM对象,由vr构成
//在实际的应用中,DICOM对象是一个包容器,包容所有命令和数据
class	DICOMObject	:
	public	PQueueOfPtr<VRGroupPQueue *>
	{
		VRGroupPQueue	   *CurrentGroup, *LastVRG;
		VR				   *LastVR;

		UINT16				LastGroup;
		UINT16				LastElement;

	public:
		UINT32				Length;
		BOOL				Packaged;
		Array<VRGroupPQueue *>		VRGroupPQueues;
		Array<DICOMObject *>		*SQObjectArray;
		UINT32				FreeSQObjectSize;
		DICOMObject			*AutoMakeDO;
		UINT32				FixedSize;
		BOOL				UseFixedSize;
	private:
		BOOL				Package();
	public:
		BOOL	ReGenerateGroupLength(BOOL bExplicit);//add by peak
		virtual		BOOL		Push(VR	*vr);
		virtual		BOOL		Push(DICOMObject *);	// Sequence Embedding
		virtual		VR		*Pop();
		LE_UINT16			GetUINT16(UINT16, UINT16);
		VR				*GetVR(UINT16, UINT16);
		virtual		BOOL		Reset();
		DICOMObject();
		~DICOMObject();
	};
//DICOM命令对象,是DICOM对象的一个表现形式
class	DICOMCommandObject	:
	public	DICOMObject
	{
	};
//DICOM数据对象,是DICOM对象的另外一个表现形式
class	DICOMDataObject	:
	public	DICOMObject
	{
	};
