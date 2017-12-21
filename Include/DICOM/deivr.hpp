
class	DICOMObject;
//VR��һ������Ҫ����
//VR��Value Representation����д
//����ģ�����������ݱ����
//VR��һ���ʽ��(GroupID,ElementID,Type(����û��),Length,Data)
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
//�����һ��VR�Ķ���
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
//�����DICOM����,��vr����
//��ʵ�ʵ�Ӧ����,DICOM������һ��������,�����������������
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
//DICOM�������,��DICOM�����һ��������ʽ
class	DICOMCommandObject	:
	public	DICOMObject
	{
	};
//DICOM���ݶ���,��DICOM���������һ��������ʽ
class	DICOMDataObject	:
	public	DICOMObject
	{
	};
