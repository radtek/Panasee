//基本所有使用到DICOM类库的东西都要使用该头文件
#ifndef WINDOWS
#define WINDOWS
#endif
#ifndef	_DICOM_H_
#	define	_DICOM_H_
#	include	"base.hpp"		// C Header Files/Base Data Templates
#	include	"endian.hpp"	// Endian Nuetral types
#	include	"constant.h"	// Related constants
#	include	"version.h"		// Version Related Information
#	include	"socket.hpp"	// Socket Transport Layer
#	include	"deivr.hpp"		// DICOMObject/DICOMCMDObject/DICOMDATAObject
#	include	"rtc.hpp"		// Run-Time-Class support of VR
#	include	"aarq.hpp"		// A-ASSOCIATE-RQ PDU
#	include	"aaac.hpp"		// A-ASSOCIATE-AC PDU
#	include	"aarj.hpp"		// A-ASSOCIATE-RJ/A-RELEASE-RQ/A-RELEASE-RP/A-ABORT-RQ
#	include	"pdata.hpp"		// P-DATA-TF
#	include	"pdu.hpp"		// PDU Services

#	include	"dimsec.hpp"	// DIMSE-C Services CEchoRQ/RSP, CStoreRQ/RSP
							// CFindRQ/RSP, CMoveRQ/RSP
#	include	"dimsen.hpp"	// DIMSE-N Services
#	include	"verify.hpp"	// Verification SOP Class
#	include	"storage.hpp"	// CT Storage, MR Storage, etc.
#	include	"qrsop.hpp"		// Abstract Q/R Classes
#	include	"printsop.hpp"		// printsop
#	include "cdicomdir.hpp"
#endif
