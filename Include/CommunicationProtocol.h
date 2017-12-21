enum ENUM_TYPE_OF_PACKET
{
	// LOG
	TYPE_PACKET__LOG    = 0,
	// AMPCI
	TYPE_PACKET__TUNING_BREATHHOLD = 2, // Tuning
	// SERIAL
	TYPE_PACKET__GATING_SHIMMING = 3, // �ſ�
	// ETHERNET
	TYPE_PACKET__PRINTER   = 4, // ��ӡ��
	TYPE_PACKET__PACS_SERVER  = 5, // PACS
	TYPE_PACKET__INTERVENTION  = 6, // ��������
 
};

// ���ݰ�������
enum ENUM_SUBTYPE_OF_PACKET
{
	SUBTYPE_PACKET__WND      = 0, // Calling���������ڳ�ʼ��ʱʹ�ã���ECS����Calling������

	// IMRI
	SUBTYPE_IMRI_PACKET__IMAGES    = 1, // ͼ�����ݰ�
	SUBTYPE_IMRI_PACKET__LOCALIZATION  = 2, // ��λ��Ϣ��
};

struct STRUCT_PACKET___TYPE
{
	ENUM_TYPE_OF_PACKET  nType;
	ENUM_SUBTYPE_OF_PACKET nSubType;
};


struct STRUCT_IMRI__LOCALIZATION
{
	float fCenterX, fCenterY, fCenterZ; // SLAB���ĵ����������(mm)
	float fFreqX, fFreqY, fFreqZ;  // SLABƵ�ʱ��뷽��
	float fPhaseX, fPhaseY, fPhaseZ; // SLAB��λ���뷽��
};

// ������������
union UNION_DATA
{
	HWND hCallingWnd;

	// IMRI
	// oType.nType = TYPE_PACKET__INTERVENTION
	char sImageFile[255]; // ��ɨ���ն˷������ն˵�ͼ���ļ���.          
	// oType.nSubType = SUBTYPE_IMRI_PACKET__IMAGES
	STRUCT_IMRI__LOCALIZATION oLocInfo; // �ɼ���ն˷���ɨ���ն˵Ķ�λ��Ϣ
	// oType.nSubType = SUBTYPE_IMRI_PACKET__LOCALIZATION
 
};

struct STRUCT_PACKET
{
	STRUCT_PACKET___TYPE oType;
	UNION_DATA oData;
};
