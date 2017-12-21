//定义DICOMDIR函数
//本函数只有个ADDTODIR函数
//该函数完成打开DICOMDIR,然后将DICOM文件的内容
//分析出来,添加到DICOMDIR的各层中.关于DICOMDIR各层参见CDICOMDIR.HPP文件
//然后保存修改过的DICOMDIR文件
//从实质上而言.DICOMDIR文件也是个DICOM文件.其格式在DICOM标准
//的ps3.10中定义
#ifndef _DICOMDIR_H
#define _DICOMDIR_H
#include "QRParam.h"
BOOL AddToDir(char*DcmDirPath,char*DcmFilePath);
#endif