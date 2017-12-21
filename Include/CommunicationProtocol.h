enum ENUM_TYPE_OF_PACKET
{
	// LOG
	TYPE_PACKET__LOG    = 0,
	// AMPCI
	TYPE_PACKET__TUNING_BREATHHOLD = 2, // Tuning
	// SERIAL
	TYPE_PACKET__GATING_SHIMMING = 3, // 门控
	// ETHERNET
	TYPE_PACKET__PRINTER   = 4, // 打印机
	TYPE_PACKET__PACS_SERVER  = 5, // PACS
	TYPE_PACKET__INTERVENTION  = 6, // 介入治疗
 
};

// 数据包子类型
enum ENUM_SUBTYPE_OF_PACKET
{
	SUBTYPE_PACKET__WND      = 0, // Calling窗体句柄。在初始化时使用，向ECS传递Calling窗体句柄

	// IMRI
	SUBTYPE_IMRI_PACKET__IMAGES    = 1, // 图像数据包
	SUBTYPE_IMRI_PACKET__LOCALIZATION  = 2, // 定位信息包
};

struct STRUCT_PACKET___TYPE
{
	ENUM_TYPE_OF_PACKET  nType;
	ENUM_SUBTYPE_OF_PACKET nSubType;
};


struct STRUCT_IMRI__LOCALIZATION
{
	float fCenterX, fCenterY, fCenterZ; // SLAB中心点的物理坐标(mm)
	float fFreqX, fFreqY, fFreqZ;  // SLAB频率编码方向
	float fPhaseX, fPhaseY, fPhaseZ; // SLAB相位编码方向
};

// 包的数据内容
union UNION_DATA
{
	HWND hCallingWnd;

	// IMRI
	// oType.nType = TYPE_PACKET__INTERVENTION
	char sImageFile[255]; // 由扫描终端发向监控终端的图像文件名.          
	// oType.nSubType = SUBTYPE_IMRI_PACKET__IMAGES
	STRUCT_IMRI__LOCALIZATION oLocInfo; // 由监控终端发向扫描终端的定位信息
	// oType.nSubType = SUBTYPE_IMRI_PACKET__LOCALIZATION
 
};

struct STRUCT_PACKET
{
	STRUCT_PACKET___TYPE oType;
	UNION_DATA oData;
};
