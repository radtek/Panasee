#if !defined(AFX_GLOBAL_H__1E05947A_55BD_4DB9_A343_C14E4CDEE07E__INCLUDED_)
#define AFX_GLOBAL_H__1E05947A_55BD_4DB9_A343_C14E4CDEE07E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>

#include "dcmtk/config/osconfig.h"    /* make sure OS specific configuration is included first */

#define INCLUDE_CSTDLIB
#define INCLUDE_CSTRING
#define INCLUDE_CSTDARG
#define INCLUDE_CCTYPE
#define INCLUDE_CSIGNAL
#include "dcmtk/ofstd/ofstdinc.h"

#ifdef _WIN32
#include <process.h>     /* needed for declaration of getpid() */
#endif

BEGIN_EXTERN_C
#ifdef HAVE_SYS_STAT_H
#include <sys/stat.h>
#endif
#ifdef HAVE_FCNTL_H
#include <fcntl.h>       /* needed on Solaris for O_RDONLY */
#endif
END_EXTERN_C

#ifdef HAVE_GUSI_H
#include <GUSI.h>
#endif

#ifdef HAVE_WINDOWS_H
#include <direct.h>        /* for _mkdir() */
#endif


#include "dcmtk/dcmnet/dimse.h"
#include "dcmtk/dcmnet/diutil.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcdebug.h"
#include "dcmtk/dcmdata/dcuid.h"
#include "dcmtk/dcmdata/dcdict.h"
#include "dcmtk/dcmdata/cmdlnarg.h"
#include "dcmtk/ofstd/ofconapp.h"
#include "dcmtk/ofstd/ofstd.h"
#include "dcmtk/ofstd/ofdatime.h"
#include "dcmtk/dcmdata/dcuid.h"         /* for dcmtk version name */
#include "dcmtk/dcmnet/dicom.h"         /* for DICOM_APPLICATION_ACCEPTOR */
#include "dcmtk/dcmdata/dcdeftag.h"      /* for DCM_StudyInstanceUID */
#include "dcmtk/dcmdata/dcostrmz.h"      /* for dcmZlibCompressionLevel */
#include "dcmtk/dcmnet/dcasccfg.h"      /* for class DcmAssociationConfiguration */
#include "dcmtk/dcmnet/dcasccff.h"      /* for class DcmAssociationConfigurationFile */

#ifdef WITH_OPENSSL
#include "dcmtk/dcmtls/tlstrans.h"
#include "dcmtk/dcmtls/tlslayer.h"
#endif

#ifdef WITH_ZLIB
#include <zlib.h>          /* for zlibVersion() */
#endif

//  [8/29/2011 Administrator]


#include <Winsock2.h>
#include "..\..\..\Include\DCMMESSAGE.h"
#include "..\..\..\Include\GlobalDef.h"
#include "..\..\..\Include\dcmtk.h"
#include "..\..\Include\BI_ImageHeader.h"
#include "..\..\Include\CT_ImageHeader.h"

MR_STRUCT_IMAGE *g_MRImgData = NULL;
CArray<MR_STRUCT_IMAGE*, MR_STRUCT_IMAGE*> g_carrayMRImgs;

CT_STRUCT_IMAGE *g_CTImgData = NULL;
CArray<CT_STRUCT_IMAGE*, CT_STRUCT_IMAGE*> g_carrayCTImgs;

//  [8/29/2011 Administrator]

#if defined(HAVE_MKTEMP) && !defined(HAVE_PROTOTYPE_MKTEMP)
extern "C" {
	char * mktemp(char *);
}
#endif

// Solaris 2.5.1 has mkstemp() in libc.a but no prototype
#if defined(HAVE_MKSTEMP) && !defined(HAVE_PROTOTYPE_MKSTEMP)
extern "C" {
	int mkstemp(char *);
}
#endif


#ifdef PRIVATE_STORESCP_DECLARATIONS
PRIVATE_STORESCP_DECLARATIONS
#else
#define OFFIS_CONSOLE_APPLICATION "storescp"
#endif

static char rcsid[] = "$dcmtk: " OFFIS_CONSOLE_APPLICATION " v" OFFIS_DCMTK_VERSION " " OFFIS_DCMTK_RELEASEDATE " $";

#define APPLICATIONTITLE "STORESCP"     /* our application entity title */

#define PATH_PLACEHOLDER "#p"
#define FILENAME_PLACEHOLDER "#f"
#define CALLING_AETITLE_PLACEHOLDER "#a"
#define CALLED_AETITLE_PLACEHOLDER "#c"

static OFCondition processCommands(FCBI* pCBI,T_ASC_Association *assoc);
static OFCondition acceptAssociation(FCBI *pCBI,T_ASC_Network *net, DcmAssociationConfiguration& asccfg);
static OFCondition echoSCP(T_ASC_Association * assoc, T_DIMSE_Message * msg, T_ASC_PresentationContextID presID);
static OFCondition storeSCP(FCBI* pCBI,T_ASC_Association * assoc, T_DIMSE_Message * msg, T_ASC_PresentationContextID presID);
static void executeOnReception();
static void executeEndOfStudyEvents();
static void executeOnEndOfStudy();
static void renameOnEndOfStudy();
static OFString replaceChars( const OFString &srcstr, const OFString &pattern, const OFString &substitute );
static void executeCommand( const OFString &cmd );
static void cleanChildren(pid_t pid, OFBool synch);
static OFCondition acceptUnknownContextsWithPreferredTransferSyntaxes(
																	  T_ASC_Parameters * params,
																	  const char* transferSyntaxes[],
																	  int transferSyntaxCount,
																	  T_ASC_SC_ROLE acceptedRole = ASC_SC_ROLE_DEFAULT);
static int makeTempFile();
//  [8/30/2011 Administrator]
static void TimeConvert2(MR_BIDateTime &datetime,const char *p,int flag);
static void TimeConvert2(CT_BIDateTime &datetime,const char *p,int flag);
static void NameConvert2(char BIname[], char DIname[], int vlength);
static unsigned char AgeConvert2(char DIage[]);
static int GetSeriesOption(char szSeriesName[]);
static void GetDSVM(char* lpTemp, int nVLength, double dVal[], int& nVM);
static void CalcBIImgDirec(double dDirCos[6], double dTopLeft[3] ,
						 BYTE iPatPos, BYTE iPatEntry,
						 unsigned short sFovX, unsigned short sFovY,
						 double dBIDirec[]);
//  [8/30/2011 Administrator]

OFBool             opt_uniqueFilenames = OFFalse;
OFString           opt_fileNameExtension;
OFBool             opt_timeNames = OFFalse;
int				   timeNameCounter = -1; // "serial number" to differentiate between files with same timestamp
OFCmdUnsignedInt   opt_port = 104;
OFBool             opt_refuseAssociation = OFFalse;
OFBool             opt_rejectWithoutImplementationUID = OFFalse;
OFCmdUnsignedInt   opt_sleepAfter = 0;
OFCmdUnsignedInt   opt_sleepDuring = 0;
OFCmdUnsignedInt   opt_maxPDU = ASC_DEFAULTMAXPDU;
OFBool             opt_useMetaheader = OFTrue;
E_TransferSyntax   opt_networkTransferSyntax = EXS_Unknown;
E_TransferSyntax   opt_writeTransferSyntax = EXS_Unknown;
E_GrpLenEncoding   opt_groupLength = EGL_recalcGL;
E_EncodingType     opt_sequenceType = EET_ExplicitLength;
E_PaddingEncoding  opt_paddingType = EPD_withoutPadding;
OFCmdUnsignedInt   opt_filepad = 0;
OFCmdUnsignedInt   opt_itempad = 0;
OFCmdUnsignedInt   opt_compressionLevel = 0;
OFBool             opt_verbose = OFFalse;
OFBool             opt_debug = OFFalse;
OFBool             opt_bitPreserving = OFFalse;
OFBool             opt_ignore = OFFalse;
OFBool             opt_abortDuringStore = OFFalse;
OFBool             opt_abortAfterStore = OFFalse;
OFBool             opt_promiscuous = OFFalse;
OFBool             opt_correctUIDPadding = OFFalse;
OFBool             opt_inetd_mode = OFFalse;
OFString           callingaetitle = "AETITLE";  // calling AE title will be stored here
OFString           calledaetitle = "AETITLE";   // called AE title will be stored here
const char *       opt_respondingaetitle = APPLICATIONTITLE;
static OFBool      opt_secureConnection = OFFalse;    // default: no secure connection
static OFString    opt_outputDirectory(".");          // default: output directory equals "."
static const char *opt_sortConcerningStudies = NULL;  // default: no sorting
OFString           lastStudyInstanceUID;
OFString           subdirectoryPathAndName;
OFList<OFString>   outputFileNameArray;
static const char *opt_execOnReception = NULL;        // default: don't execute anything on reception
static const char *opt_execOnEndOfStudy = NULL;       // default: don't execute anything on end of study

OFString           lastStudySubdirectoryPathAndName;
static OFBool      opt_renameOnEndOfStudy = OFFalse;  // default: don't rename any files on end of study
static long        opt_endOfStudyTimeout = -1;        // default: no end of study timeout
static OFBool      endOfStudyThroughTimeoutEvent = OFFalse;
static const char *opt_configFile = NULL;
static const char *opt_profileName = NULL;
T_DIMSE_BlockingMode opt_blockMode = DIMSE_BLOCKING;
int                opt_dimse_timeout = 0;
int                opt_acse_timeout = 30;


char szAET[16]="COOLPACS";
char szPort[16]="104";
char szRootDir[256]="d:\\dicom\\imagedir\\";
BOOL bTerminated=TRUE;

#if defined(HAVE_FORK) || defined(_WIN32)
OFBool             opt_forkMode = OFFalse;
#endif

#ifdef _WIN32
OFBool             opt_forkedChild = OFFalse;
OFBool             opt_execSync = OFFalse;            // default: execute in background
#endif

#ifdef WITH_OPENSSL
static int         opt_keyFileFormat = SSL_FILETYPE_PEM;
static const char *opt_privateKeyFile = NULL;
static const char *opt_certificateFile = NULL;
static const char *opt_passwd = NULL;
#if OPENSSL_VERSION_NUMBER >= 0x0090700fL
static OFString    opt_ciphersuites(TLS1_TXT_RSA_WITH_AES_128_SHA ":" SSL3_TXT_RSA_DES_192_CBC3_SHA);
#else
static OFString    opt_ciphersuites(SSL3_TXT_RSA_DES_192_CBC3_SHA);
#endif
static const char *opt_readSeedFile = NULL;
static const char *opt_writeSeedFile = NULL;
static DcmCertificateVerification opt_certVerification = DCV_requireCertificate;
static const char *opt_dhparam = NULL;
#endif


#ifdef HAVE_WAITPID
/** signal handler for SIGCHLD signals that immediately cleans up
*  terminated children.
*/
#ifdef SIGNAL_HANDLER_WITH_ELLIPSE
extern "C" void sigChildHandler(...)
#else
extern "C" void sigChildHandler(int)
#endif
{
	int status = 0;
	waitpid( -1, &status, WNOHANG );
	signal(SIGCHLD, sigChildHandler);
}
#endif


#define SHORTCOL 4
#define LONGCOL 21


static OFCondition acceptAssociation(FCBI *pCBI,T_ASC_Network *net, DcmAssociationConfiguration& asccfg)
{
	char buf[BUFSIZ];
	T_ASC_Association *assoc;
	OFCondition cond;
	OFString sprofile;
	
#ifdef PRIVATE_STORESCP_VARIABLES
	PRIVATE_STORESCP_VARIABLES
#endif
		
		const char* knownAbstractSyntaxes[] =
	{
		UID_VerificationSOPClass
	};
	
	const char* transferSyntaxes[] = { NULL, NULL, NULL, NULL };
	int numTransferSyntaxes = 0;
	
	// try to receive an association. Here we either want to use blocking or
	// non-blocking, depending on if the option --eostudy-timeout is set.
	if( opt_endOfStudyTimeout == -1 )
		cond = ASC_receiveAssociation(pCBI,net, &assoc, opt_maxPDU, NULL, NULL, opt_secureConnection);
	else
		cond = ASC_receiveAssociation(pCBI,net, &assoc, opt_maxPDU, NULL, NULL, opt_secureConnection, DUL_NOBLOCK, OFstatic_cast(int, opt_endOfStudyTimeout));
	
	if (cond.code() == DULC_FORKEDCHILD)
	{
		// if (opt_verbose) DimseCondition::dump(cond);
		goto cleanup;
	}
	
	// if some kind of error occured, take care of it
	if (cond.bad())
	{

		// check what kind of error occurred. If no association was
		// received, check if certain other conditions are met
		if( cond == DUL_NOASSOCIATIONREQUEST )
		{
			//  [9/30/2011 Administrator]
			outputFileNameArray.clear();//确保outputFileNameArray清空
			lastStudyInstanceUID.clear();
			lastStudySubdirectoryPathAndName.clear();
			//  [9/30/2011 Administrator]
			// If in addition to the fact that no association was received also option --eostudy-timeout is set
			// and if at the same time there is still a study which is considered to be open (i.e. we were actually
			// expecting to receive more objects that belong to this study) (this is the case if lastStudyInstanceUID
			// does not equal NULL), we have to consider that all objects for the current study have been received.
			// In such an "end-of-study" case, we might have to execute certain optional functions which were specified
			// by the user through command line options passed to storescp.
			if( opt_endOfStudyTimeout != -1 && ! lastStudyInstanceUID.empty() )
			{
				// indicate that the end-of-study-event occured through a timeout event.
				// This knowledge will be necessary in function renameOnEndOFStudy().
				endOfStudyThroughTimeoutEvent = OFTrue;
				
				// before we actually execute those optional functions, we need to determine the path and name
				// of the subdirectory into which the DICOM files for the last study were written.
				lastStudySubdirectoryPathAndName = subdirectoryPathAndName;
				
				// now we can finally handle end-of-study events which might have to be executed
				executeEndOfStudyEvents();
				
				// also, we need to clear lastStudyInstanceUID to indicate
				// that the last study is not considered to be open any more.
				lastStudyInstanceUID.clear();
				
				// also, we need to clear subdirectoryPathAndName
				subdirectoryPathAndName.clear();
				
				// reset the endOfStudyThroughTimeoutEvent variable.
				endOfStudyThroughTimeoutEvent = OFFalse;
			}
		}
		// If something else was wrong we might have to dump an error message.
		else
		{
			if( opt_verbose ) DimseCondition::dump(cond);
		}
		
		// no matter what kind of error occurred, we need to do a cleanup
		goto cleanup;
	}
	
	if (opt_verbose)
	{
#if defined(HAVE_FORK) || defined(_WIN32)
		if (opt_forkMode)
		{
			printf("Association Received in %s process (pid: %ld)\n", (DUL_processIsForkedChild() ? "child" : "parent") , OFstatic_cast(long, getpid()));
		}
		else printf("Association Received\n");
#else
		printf("Association Received\n");
#endif
	}
	
	if (opt_debug)
	{
		printf("Parameters:\n");
		ASC_dumpParameters(assoc->params, COUT);
		
		DIC_AE callingTitle;
		DIC_AE calledTitle;
		ASC_getAPTitles(assoc->params, callingTitle, calledTitle, NULL);
		
		CERR << "called AE:  " << calledTitle << endl
			<< "calling AE: " << callingTitle << endl;
	}
	
	if (opt_refuseAssociation)
	{
		T_ASC_RejectParameters rej =
		{
			ASC_RESULT_REJECTEDPERMANENT,
				ASC_SOURCE_SERVICEUSER,
				ASC_REASON_SU_NOREASON
		};
		
		if (opt_verbose) printf("Refusing Association (forced via command line)\n");
		cond = ASC_rejectAssociation(assoc, &rej);
		if (cond.bad())
		{
			printf("Association Reject Failed:\n");
			DimseCondition::dump(cond);
		}
		goto cleanup;
	}
	
	switch (opt_networkTransferSyntax)
	{
    case EXS_LittleEndianImplicit:
		/* we only support Little Endian Implicit */
		transferSyntaxes[0]  = UID_LittleEndianImplicitTransferSyntax;
		numTransferSyntaxes = 1;
		break;
    case EXS_LittleEndianExplicit:
		/* we prefer Little Endian Explicit */
		transferSyntaxes[0] = UID_LittleEndianExplicitTransferSyntax;
		transferSyntaxes[1] = UID_BigEndianExplicitTransferSyntax;
		transferSyntaxes[2]  = UID_LittleEndianImplicitTransferSyntax;
		numTransferSyntaxes = 3;
		break;
    case EXS_BigEndianExplicit:
		/* we prefer Big Endian Explicit */
		transferSyntaxes[0] = UID_BigEndianExplicitTransferSyntax;
		transferSyntaxes[1] = UID_LittleEndianExplicitTransferSyntax;
		transferSyntaxes[2]  = UID_LittleEndianImplicitTransferSyntax;
		numTransferSyntaxes = 3;
		break;
    case EXS_JPEGProcess14SV1TransferSyntax:
		/* we prefer JPEGLossless:Hierarchical-1stOrderPrediction (default lossless) */
		transferSyntaxes[0] = UID_JPEGProcess14SV1TransferSyntax;
		transferSyntaxes[1] = UID_LittleEndianExplicitTransferSyntax;
		transferSyntaxes[2] = UID_BigEndianExplicitTransferSyntax;
		transferSyntaxes[3] = UID_LittleEndianImplicitTransferSyntax;
		numTransferSyntaxes = 4;
		break;
    case EXS_JPEGProcess1TransferSyntax:
		/* we prefer JPEGBaseline (default lossy for 8 bit images) */
		transferSyntaxes[0] = UID_JPEGProcess1TransferSyntax;
		transferSyntaxes[1] = UID_LittleEndianExplicitTransferSyntax;
		transferSyntaxes[2] = UID_BigEndianExplicitTransferSyntax;
		transferSyntaxes[3] = UID_LittleEndianImplicitTransferSyntax;
		numTransferSyntaxes = 4;
		break;
    case EXS_JPEGProcess2_4TransferSyntax:
		/* we prefer JPEGExtended (default lossy for 12 bit images) */
		transferSyntaxes[0] = UID_JPEGProcess2_4TransferSyntax;
		transferSyntaxes[1] = UID_LittleEndianExplicitTransferSyntax;
		transferSyntaxes[2] = UID_BigEndianExplicitTransferSyntax;
		transferSyntaxes[3] = UID_LittleEndianImplicitTransferSyntax;
		numTransferSyntaxes = 4;
		break;
    case EXS_JPEG2000:
		/* we prefer JPEG2000 Lossy */
		transferSyntaxes[0] = UID_JPEG2000TransferSyntax;
		transferSyntaxes[1] = UID_LittleEndianExplicitTransferSyntax;
		transferSyntaxes[2] = UID_BigEndianExplicitTransferSyntax;
		transferSyntaxes[3] = UID_LittleEndianImplicitTransferSyntax;
		numTransferSyntaxes = 4;
		break;
    case EXS_JPEG2000LosslessOnly:
		/* we prefer JPEG2000 Lossless */
		transferSyntaxes[0] = UID_JPEG2000LosslessOnlyTransferSyntax;
		transferSyntaxes[1] = UID_LittleEndianExplicitTransferSyntax;
		transferSyntaxes[2] = UID_BigEndianExplicitTransferSyntax;
		transferSyntaxes[3] = UID_LittleEndianImplicitTransferSyntax;
		numTransferSyntaxes = 4;
		break;
    case EXS_RLELossless:
		/* we prefer RLE Lossless */
		transferSyntaxes[0] = UID_RLELosslessTransferSyntax;
		transferSyntaxes[1] = UID_LittleEndianExplicitTransferSyntax;
		transferSyntaxes[2] = UID_BigEndianExplicitTransferSyntax;
		transferSyntaxes[3] = UID_LittleEndianImplicitTransferSyntax;
		numTransferSyntaxes = 4;
		break;
#ifdef WITH_ZLIB
    case EXS_DeflatedLittleEndianExplicit:
		/* we prefer Deflated Explicit VR Little Endian */
		transferSyntaxes[0] = UID_DeflatedExplicitVRLittleEndianTransferSyntax;
		transferSyntaxes[1] = UID_LittleEndianExplicitTransferSyntax;
		transferSyntaxes[2] = UID_BigEndianExplicitTransferSyntax;
		transferSyntaxes[3] = UID_LittleEndianImplicitTransferSyntax;
		numTransferSyntaxes = 4;
		break;
#endif
    default:
	/* We prefer explicit transfer syntaxes.
	* If we are running on a Little Endian machine we prefer
	* LittleEndianExplicitTransferSyntax to BigEndianTransferSyntax.
		*/
		if (gLocalByteOrder == EBO_LittleEndian)  /* defined in dcxfer.h */
		{
			transferSyntaxes[0] = UID_LittleEndianExplicitTransferSyntax;
			transferSyntaxes[1] = UID_BigEndianExplicitTransferSyntax;
		}
		else
		{
			transferSyntaxes[0] = UID_BigEndianExplicitTransferSyntax;
			transferSyntaxes[1] = UID_LittleEndianExplicitTransferSyntax;
		}
		transferSyntaxes[2] = UID_LittleEndianImplicitTransferSyntax;
		numTransferSyntaxes = 3;
		break;
	}
	
	if (opt_profileName)
	{
		/* perform name mangling for config file key */
		const char *c = opt_profileName;
		while (*c)
		{
			if (! isspace(*c)) sprofile += OFstatic_cast(char, toupper(*c));
			++c;
		}
		
		/* set presentation contexts as defined in config file */
		cond = asccfg.evaluateAssociationParameters(sprofile.c_str(), *assoc);
		if (cond.bad())
		{
			if (opt_verbose) DimseCondition::dump(cond);
			goto cleanup;
		}
	}
	else
	{
		/* accept the Verification SOP Class if presented */
		cond = ASC_acceptContextsWithPreferredTransferSyntaxes( assoc->params, knownAbstractSyntaxes, DIM_OF(knownAbstractSyntaxes), transferSyntaxes, numTransferSyntaxes);
		if (cond.bad())
		{
			if (opt_verbose) DimseCondition::dump(cond);
			goto cleanup;
		}
		
		/* the array of Storage SOP Class UIDs comes from dcuid.h */
		cond = ASC_acceptContextsWithPreferredTransferSyntaxes( assoc->params, dcmAllStorageSOPClassUIDs, numberOfAllDcmStorageSOPClassUIDs, transferSyntaxes, numTransferSyntaxes);
		if (cond.bad())
		{
			if (opt_verbose) DimseCondition::dump(cond);
			goto cleanup;
		}
		
		if (opt_promiscuous)
		{
			/* accept everything not known not to be a storage SOP class */
			cond = acceptUnknownContextsWithPreferredTransferSyntaxes(
				assoc->params, transferSyntaxes, numTransferSyntaxes);
			if (cond.bad())
			{
				if (opt_verbose) DimseCondition::dump(cond);
				goto cleanup;
			}
		}
	}
	
	/* set our app title */
	ASC_setAPTitles(assoc->params, NULL, NULL, opt_respondingaetitle);
	
	/* acknowledge or reject this association */
	cond = ASC_getApplicationContextName(assoc->params, buf);
	if ((cond.bad()) || strcmp(buf, UID_StandardApplicationContext) != 0)
	{
		/* reject: the application context name is not supported */
		T_ASC_RejectParameters rej =
		{
			ASC_RESULT_REJECTEDPERMANENT,
				ASC_SOURCE_SERVICEUSER,
				ASC_REASON_SU_APPCONTEXTNAMENOTSUPPORTED
		};
		
		if (opt_verbose) printf("Association Rejected: bad application context name: %s\n", buf);
		cond = ASC_rejectAssociation(assoc, &rej);
		if (cond.bad())
		{
			if (opt_verbose) DimseCondition::dump(cond);
		}
		goto cleanup;
		
	}
	else if (opt_rejectWithoutImplementationUID && strlen(assoc->params->theirImplementationClassUID) == 0)
	{
		/* reject: the no implementation Class UID provided */
		T_ASC_RejectParameters rej =
		{
			ASC_RESULT_REJECTEDPERMANENT,
				ASC_SOURCE_SERVICEUSER,
				ASC_REASON_SU_NOREASON
		};
		
		if (opt_verbose) printf("Association Rejected: No Implementation Class UID provided\n");
		cond = ASC_rejectAssociation(assoc, &rej);
		if (cond.bad())
		{
			if (opt_verbose) DimseCondition::dump(cond);
		}
		goto cleanup;
	}
	else
	{
#ifdef PRIVATE_STORESCP_CODE
		PRIVATE_STORESCP_CODE
#endif
			cond = ASC_acknowledgeAssociation(assoc);
		if (cond.bad())
		{
			DimseCondition::dump(cond);
			goto cleanup;
		}
		if (opt_verbose)
		{
			printf("Association Acknowledged (Max Send PDV: %lu)\n", assoc->sendPDVLength);
			if (ASC_countAcceptedPresentationContexts(assoc->params) == 0)
				printf("    (but no valid presentation contexts)\n");
			if (opt_debug) ASC_dumpParameters(assoc->params, COUT);
		}
	}
	
#ifdef BUGGY_IMPLEMENTATION_CLASS_UID_PREFIX
	/* active the dcmPeerRequiresExactUIDCopy workaround code
	* (see comments in dimse.h) for a implementation class UID
	* prefix known to exhibit the buggy behaviour.
	*/
	if (0 == strncmp(assoc->params->theirImplementationClassUID,
		BUGGY_IMPLEMENTATION_CLASS_UID_PREFIX,
		strlen(BUGGY_IMPLEMENTATION_CLASS_UID_PREFIX)))
	{
		dcmEnableAutomaticInputDataCorrection.set(OFFalse);
		dcmPeerRequiresExactUIDCopy.set(OFTrue);
	}
#endif
	
	// store calling and called aetitle in global variables to enable
	// the --exec options using them. Enclose in quotation marks because
	// aetitles may contain space characters.
	DIC_AE callingTitle;
	DIC_AE calledTitle;
	if (ASC_getAPTitles(assoc->params, callingTitle, calledTitle, NULL).good())
	{
		callingaetitle = "\"";
		callingaetitle += callingTitle;
		callingaetitle += "\"";
		calledaetitle = "\"";
		calledaetitle += calledTitle;
		calledaetitle += "\"";
	}
	else
	{
		// should never happen
		callingaetitle.clear();
		calledaetitle.clear();
	}
	
	/* now do the real work, i.e. receive DIMSE commmands over the network connection */
	/* which was established and handle these commands correspondingly. In case of */
	/* storscp only C-ECHO-RQ and C-STORE-RQ commands can be processed. */

	cond = processCommands(pCBI,assoc);
	
	if (cond == DUL_PEERREQUESTEDRELEASE)
	{
		if (opt_verbose) printf("Association Release\n");
		cond = ASC_acknowledgeRelease(assoc);
	}
	else if (cond == DUL_PEERABORTEDASSOCIATION)
	{
		if (opt_verbose) printf("Association Aborted\n");
	}
	else
	{
		fprintf(stderr, "storescp: DIMSE Failure (aborting association)\n");
		/* some kind of error so abort the association */
		cond = ASC_abortAssociation(assoc);
	}
	
cleanup:
	
	if (cond.code() == DULC_FORKEDCHILD) return cond;
	
	ASC_dropSCPAssociation(assoc);
/*
	cond = 
	if (cond.bad())
	{
		DimseCondition::dump(cond);
		exit(1);
	}
*/
	ASC_destroyAssociation(&assoc);
/*
	cond = 
	if (cond.bad())
	{
		DimseCondition::dump(cond);
		exit(1);
	}
*/	
	return cond;
}



static OFCondition
processCommands(FCBI* pCBI,T_ASC_Association * assoc)
/*
* This function receives DIMSE commmands over the network connection
* and handles these commands correspondingly. Note that in case of
* storscp only C-ECHO-RQ and C-STORE-RQ commands can be processed.
*
* Parameters:
*   assoc - [in] The association (network connection to another DICOM application).
*/
{
	OFCondition cond = EC_Normal;
	T_DIMSE_Message msg;
	T_ASC_PresentationContextID presID = 0;
	DcmDataset *statusDetail = NULL;
	
	// start a loop to be able to receive more than one DIMSE command
	while( cond == EC_Normal || cond == DIMSE_NODATAAVAILABLE || cond == DIMSE_OUTOFRESOURCES )
	{
		// receive a DIMSE command over the network
		if( opt_endOfStudyTimeout == -1 )
			cond = DIMSE_receiveCommand(assoc, DIMSE_BLOCKING, 0, &presID, &msg, &statusDetail);
		else
			cond = DIMSE_receiveCommand(assoc, DIMSE_NONBLOCKING, OFstatic_cast(int, opt_endOfStudyTimeout), &presID, &msg, &statusDetail);
		
		// check what kind of error occurred. If no data was
		// received, check if certain other conditions are met
		if( cond == DIMSE_NODATAAVAILABLE )
		{
			// If in addition to the fact that no data was received also option --eostudy-timeout is set and
			// if at the same time there is still a study which is considered to be open (i.e. we were actually
			// expecting to receive more objects that belong to this study) (this is the case if lastStudyInstanceUID
			// does not equal NULL), we have to consider that all objects for the current study have been received.
			// In such an "end-of-study" case, we might have to execute certain optional functions which were specified
			// by the user through command line options passed to storescp.
			if( opt_endOfStudyTimeout != -1 && ! lastStudyInstanceUID.empty() )
			{
				// indicate that the end-of-study-event occured through a timeout event.
				// This knowledge will be necessary in function renameOnEndOFStudy().
				endOfStudyThroughTimeoutEvent = OFTrue;
				
				// before we actually execute those optional functions, we need to determine the path and name
				// of the subdirectory into which the DICOM files for the last study were written.
				lastStudySubdirectoryPathAndName = subdirectoryPathAndName;
				
				// now we can finally handle end-of-study events which might have to be executed
				executeEndOfStudyEvents();
				
				// also, we need to clear lastStudyInstanceUID to indicate
				// that the last study is not considered to be open any more.
				lastStudyInstanceUID.clear();
				
				// also, we need to clear subdirectoryPathAndName
				subdirectoryPathAndName.clear();
				
				// reset the endOfStudyThroughTimeoutEvent variable.
				endOfStudyThroughTimeoutEvent = OFFalse;
			}
		}
		
		// if the command which was received has extra status
		// detail information, dump this information
		if (statusDetail != NULL)
		{
			printf("Extra Status Detail: \n");
			statusDetail->print(COUT);
			delete statusDetail;
		}
		
		// check if peer did release or abort, or if we have a valid message
		if (cond == EC_Normal)
		{
			// in case we received a valid message, process this command
			// note that storescp can only process a C-ECHO-RQ and a C-STORE-RQ
			switch (msg.CommandField)
			{
			case DIMSE_C_ECHO_RQ:
				// process C-ECHO-Request
				cond = echoSCP(assoc, &msg, presID);
				break;
			case DIMSE_C_STORE_RQ:
				// process C-STORE-Request
				cond = storeSCP(pCBI,assoc, &msg, presID);
				break;
			default:
				// we cannot handle this kind of message
				cond = DIMSE_BADCOMMANDTYPE;
				fprintf(stderr, "storescp: Cannot handle command: 0x%x\n", OFstatic_cast(unsigned, msg.CommandField));
				break;
			}
		}
	}
	return cond;
}



static OFCondition echoSCP( T_ASC_Association * assoc, T_DIMSE_Message * msg, T_ASC_PresentationContextID presID)
{
	if (opt_verbose)
	{
		printf("Received ");
		DIMSE_printCEchoRQ(stdout, &msg->msg.CEchoRQ);
	}
	
	/* the echo succeeded !! */
	OFCondition cond = DIMSE_sendEchoResponse(assoc, presID, &msg->msg.CEchoRQ, STATUS_Success, NULL);
	if (cond.bad())
	{
		fprintf(stderr, "storescp: Echo SCP Failed:\n");
		DimseCondition::dump(cond);
	}
	return cond;
}



struct StoreCallbackData
{
	char* imageFileName;
	DcmFileFormat* dcmff;
	T_ASC_Association* assoc;
};



static void
storeSCPCallback(FCBI* pCBI,
				 void *callbackData,
				 T_DIMSE_StoreProgress *progress,
				 T_DIMSE_C_StoreRQ *req,
				 char * /*imageFileName*/, DcmDataset **imageDataSet,
				 T_DIMSE_C_StoreRSP *rsp,
				 DcmDataset **statusDetail)
				 /*
				 * This function.is used to indicate progress when storescp receives instance data over the
				 * network. On the final call to this function (identified by progress->state == DIMSE_StoreEnd)
				 * this function will store the data set which was received over the network to a file.
				 * Earlier calls to this function will simply cause some information to be dumped to stdout.
				 *
				 * Parameters:
				 *   callbackData  - [in] data for this callback function
				 *   progress      - [in] The state of progress. (identifies if this is the initial or final call
				 *                   to this function, or a call in between these two calls.
				 *   req           - [in] The original store request message.
				 *   imageFileName - [in] The path to and name of the file the information shall be written to.
				 *   imageDataSet  - [in] The data set which shall be stored in the image file
				 *   rsp           - [inout] the C-STORE-RSP message (will be sent after the call to this function)
				 *   statusDetail  - [inout] This variable can be used to capture detailed information with regard to
				 *                   the status information which is captured in the status element (0000,0900). Note
				 *                   that this function does specify any such information, the pointer will be set to NULL.
				 */
{
//  	DIC_UI sopClass;
//  	DIC_UI sopInstance;
	
	// determine if the association shall be aborted
	if( (opt_abortDuringStore && progress->state != DIMSE_StoreBegin) ||
		(opt_abortAfterStore && progress->state == DIMSE_StoreEnd) )
	{
		if (opt_verbose)
			printf("ABORT initiated (due to command line options)\n");
		ASC_abortAssociation((OFstatic_cast(StoreCallbackData*, callbackData))->assoc);
		rsp->DimseStatus = STATUS_STORE_Refused_OutOfResources;
		return;
	}
	
	// if opt_sleepAfter is set, the user requires that the application shall
	// sleep a certain amount of seconds after having received one PDU.
	if (opt_sleepDuring > 0)
	{
		OFStandard::sleep(OFstatic_cast(unsigned int, opt_sleepDuring));
	}
	
	// dump some information if required (depending on the progress state)
	if (opt_verbose)
	{
		switch (progress->state)
		{
		case DIMSE_StoreBegin:
			printf("RECV:");
			break;
		case DIMSE_StoreEnd:
			printf("\n");
			break;
		default:
			putchar('.');
			break;
		}
		fflush(stdout);
	}
	
	// if this is the final call of this function, save the data which was received to a file
	// (note that we could also save the image somewhere else, put it in database, etc.)
	if (progress->state == DIMSE_StoreEnd)
	{
		// do not send status detail information
		*statusDetail = NULL;
		
		// remember callback data
		StoreCallbackData *cbdata = OFstatic_cast(StoreCallbackData *, callbackData);
		
		// Concerning the following line: an appropriate status code is already set in the resp structure,
		// it need not be success. For example, if the caller has already detected an out of resources problem
		// then the status will reflect this.  The callback function is still called to allow cleanup.
		//rsp->DimseStatus = STATUS_Success;
		
		// we want to write the received information to a file only if this information
		// is present and the options opt_bitPreserving and opt_ignore are not set.
		if ((imageDataSet)&&(*imageDataSet)&&(!opt_bitPreserving)&&(!opt_ignore))
		{
			OFString fileName;
			
			// in case option --sort-conc-studies is set, we need to perform some particular
			// steps to determine the actual name of the output file
			if( opt_sortConcerningStudies != NULL )
			{
				// determine the study instance UID in the (current) DICOM object that has just been received
				// note that if findAndGetString says the resulting study instance UID equals NULL, the study
				// instance UID in the (current) DICOM object is an empty string; in general: no matter what
				// happened, we want to create a new string that will contain a corresponding value for the
				// current study instance UID
				const char *tmpstr1 = NULL;
				OFString currentStudyInstanceUID;
				DcmTagKey studyInstanceUIDTagKey( DCM_StudyInstanceUID );
				OFCondition ec = (*imageDataSet)->findAndGetString( studyInstanceUIDTagKey, tmpstr1, OFFalse );
				if( ec != EC_Normal )
				{
					fprintf(stderr, "storescp: No study instance UID found in data set.\n");
					rsp->DimseStatus = STATUS_STORE_Error_CannotUnderstand;
					return;
				}
				if (tmpstr1) currentStudyInstanceUID = tmpstr1;
				
				// if this is the first DICOM object that was received or if the study instance UID in the
				// current DICOM object does not equal the last object's study instance UID we need to create
				// a new subdirectory in which the current DICOM object will be stored
				if( lastStudyInstanceUID.empty() || lastStudyInstanceUID != currentStudyInstanceUID)
				{
					// if lastStudyInstanceUID is non-empty, we have just completed receiving all objects for one
					// study. In such a case, we need to set a certain indicator variable (lastStudySubdirectoryPathAndName),
					// so that we know that executeOnEndOfStudy() might have to be executed later. In detail, this indicator
					// variable will contain the path and name of the last study's subdirectory, so that we can still remember
					// this directory, when we execute executeOnEndOfStudy(). The memory that is allocated for this variable
					// here will be freed after the execution of executeOnEndOfStudy().
					if( ! lastStudyInstanceUID.empty() )
					{
						lastStudySubdirectoryPathAndName = subdirectoryPathAndName;
					}
					
					// create the new lastStudyInstanceUID value according to the value in the current DICOM object
					lastStudyInstanceUID = currentStudyInstanceUID;
					
					// get the current time (needed for subdirectory name)
					OFDateTime dateTime;
					dateTime.setCurrentDateTime();
					// create a name for the new subdirectory. pattern: "[opt_sortConcerningStudies]_[YYYYMMDD]_[HHMMSSMMM]" (use current datetime)
					char buf[32];
					sprintf(buf, "_%04u%02u%02u_%02u%02u%02u%03u",
						dateTime.getDate().getYear(), dateTime.getDate().getMonth(), dateTime.getDate().getDay(),
						dateTime.getTime().getHour(), dateTime.getTime().getMinute(), dateTime.getTime().getIntSecond(), dateTime.getTime().getMilliSecond());
					OFString subdirectoryName = opt_sortConcerningStudies;
					subdirectoryName += buf;
					
					// create subdirectoryPathAndName (string with full path to new subdirectory)
					subdirectoryPathAndName = cbdata->imageFileName;
					size_t position = subdirectoryPathAndName.rfind(PATH_SEPARATOR);
					if (position != OFString_npos) subdirectoryPathAndName.erase(position+1);
					subdirectoryPathAndName += subdirectoryName;
					
					// check if the subdirectory is already existent
					// if it is already existent dump a warning
					if( OFStandard::dirExists(subdirectoryPathAndName) )
					{
						fprintf(stderr, "storescp: Warning: Subdirectory for studies already existent. (%s)\n", subdirectoryPathAndName.c_str() );
					}
					
					// if it is not existent create it
#ifdef HAVE_WINDOWS_H
					if( _mkdir( subdirectoryPathAndName.c_str() ) == -1 )
#else
						if( mkdir( subdirectoryPathAndName.c_str(), S_IRWXU | S_IRWXG | S_IRWXO ) == -1 )
#endif
						{
							fprintf(stderr, "storescp: Could not create subdirectory %s.\n", subdirectoryPathAndName.c_str() );
							rsp->DimseStatus = STATUS_STORE_Error_CannotUnderstand;
							return;
						}
						// all objects of a study have been received, so a new subdirectory is started.
						// ->timename counter can be reset, because the next filename can't cause a duplicate.
						// if no reset would be done, files of a new study (->new directory) would start with a counter in filename
						if (opt_timeNames)
							timeNameCounter = -1;
				}
				
				// integrate subdirectory name into file name (note that cbdata->imageFileName currently contains both
				// path and file name; however, the path refers to the output directory captured in opt_outputDirectory)
				char *tmpstr5 = strrchr( cbdata->imageFileName, PATH_SEPARATOR );
				fileName = subdirectoryPathAndName;
				fileName += tmpstr5;
				
				// update global variable outputFileNameArray
				// (might be used in executeOnReception() and renameOnEndOfStudy)
				outputFileNameArray.push_back(++tmpstr5);
			}
			// if option --sort-conc-studies is not set, the determination of the output file name is simple
			else
			{
				fileName = cbdata->imageFileName;
				
				// update global variables outputFileNameArray
				// (might be used in executeOnReception() and renameOnEndOfStudy)
				const char *tmpstr6 = strrchr( fileName.c_str(), PATH_SEPARATOR );
				outputFileNameArray.push_back(++tmpstr6);
			}
			
			// determine the transfer syntax which shall be used to write the information to the file
			E_TransferSyntax xfer = opt_writeTransferSyntax;
			if (xfer == EXS_Unknown) xfer = (*imageDataSet)->getOriginalXfer();
			
			//  [9/7/2011 WUZIYUAN]		
			DcmTagKey MediaStorageSOPClassUIDTagKey( DCM_MediaStorageSOPClassUID );				//DcmTagKey(0x0002, 0x0002)
			DcmTagKey TransferSyntaxUIDTagKey( DCM_TransferSyntaxUID );							//DcmTagKey(0x0002, 0x0010)
			DcmTagKey ImageTypeTagKey( DCM_ImageType );											//DcmTagKey(0x0008, 0x0008)
			DcmTagKey SOPClassUIDTagKey( DCM_SOPClassUID );										//DcmTagKey(0x0008, 0x0016)
			DcmTagKey StudyDateTagKey( DCM_StudyDate );											//DcmTagKey(0x0008, 0x0020)
			DcmTagKey SeriesDateTagKey( DCM_SeriesDate );										//DcmTagKey(0x0008, 0x0021)
			DcmTagKey AcquisitionDateTagKey( DCM_AcquisitionDate );								//DcmTagKey(0x0008, 0x0022)
			DcmTagKey StudyTimeTagKey( DCM_StudyTime );											//DcmTagKey(0x0008, 0x0030)
			DcmTagKey SeriesTimeTagKey( DCM_SeriesTime );										//DcmTagKey(0x0008, 0x0031)
			DcmTagKey AcquisitionTimeTagKey( DCM_AcquisitionTime );								//DcmTagKey(0x0008, 0x0032)
			DcmTagKey ModalityTagKey( DCM_Modality );                                           //DcmTagKey(0x0008, 0x0060)
			DcmTagKey ManufacturerTagKey( DCM_Manufacturer);									//DcmTagKey(0x0008, 0x0070)
			DcmTagKey InstitutionNameTagKey( DCM_InstitutionName );								//DcmTagKey(0x0008, 0x0080)
			DcmTagKey ReferringPhysiciansNameTagKey( DCM_ReferringPhysiciansName );				//DcmTagKey(0x0008, 0x0090)
			DcmTagKey StationNameTagKey( DCM_StationName );										//DcmTagKey(0x0008, 0x1010)
			DcmTagKey StudyDescriptionTagKey( DCM_StudyDescription );							//DcmTagKey(0x0008, 0x1030)
			DcmTagKey SeriesDescriptionTagKey( DCM_SeriesDescription );							//DcmTagKey(0x0008, 0x103e)
			DcmTagKey InstitutionalDepartmentNameTagKey( DCM_InstitutionalDepartmentName );     //DcmTagKey(0x0008, 0x1040)
			DcmTagKey ManufacturersModelNameTagKey( DCM_ManufacturersModelName );				//DcmTagKey(0x0008, 0x1090)
			DcmTagKey PatientsNameTagKey( DCM_PatientsName );									//DcmTagKey(0x0010, 0x0010)
			DcmTagKey PatientIDTagKey( DCM_PatientID );											//DcmTagKey(0x0010, 0x0020)
			DcmTagKey PatientsBirthDateTagKey( DCM_PatientsBirthDate );							//DcmTagKey(0x0010, 0x0030)
			DcmTagKey PatientsBirthTimeTagKey( DCM_PatientsBirthTime );							//DcmTagKey(0x0010, 0x0032)
			DcmTagKey PatientsSexTagKey( DCM_PatientsSex );										//DcmTagKey(0x0010, 0x0040)
			DcmTagKey PatientsAgeTagKey( DCM_PatientsAge );										//DcmTagKey(0x0010, 0x1010)
			DcmTagKey PatientsWeightTagKey( DCM_PatientsWeight );								//DcmTagKey(0x0010, 0x1030)
			DcmTagKey AdditionalPatientHistoryTagKey( DCM_AdditionalPatientHistory );			//DcmTagKey(0x0010, 0x21b0)
			DcmTagKey BodyPartExaminedTagKey( DCM_BodyPartExamined );							//DcmTagKey(0x0018, 0x0015)
			DcmTagKey ScanningSequenceTagKey( DCM_ScanningSequence );							//DcmTagKey(0x0018, 0x0020)
			DcmTagKey MRAcquisitionTypeTagKey( DCM_MRAcquisitionType );							//DcmTagKey(0x0018, 0x0023)
			DcmTagKey SequenceNameTagKey( DCM_SequenceName );									//DcmTagKey(0x0018, 0x0024)
			DcmTagKey SliceThicknessTagKey( DCM_SliceThickness );								//DcmTagKey(0x0018, 0x0050)
			DcmTagKey KVPTagKey( DCM_KVP );														//DcmTagKey(0x0018, 0x0060)
			DcmTagKey RepetitionTimeTagKey( DCM_RepetitionTime );								//DcmTagKey(0x0018, 0x0080)
			DcmTagKey EchoTimeTagKey( DCM_EchoTime );											//DcmTagKey(0x0018, 0x0081)
			DcmTagKey InversionTimeTagKey( DCM_InversionTime );									//DcmTagKey(0x0018, 0x0082)
			DcmTagKey NumberOfAveragesTagKey( DCM_NumberOfAverages );							//DcmTagKey(0x0018, 0x0083)
			DcmTagKey ImagingFrequencyTagKey( DCM_ImagingFrequency );							//DcmTagKey(0x0018, 0x0084)
			DcmTagKey ImagedNucleusTagKey( DCM_ImagedNucleus );									//DcmTagKey(0x0018, 0x0085)
			DcmTagKey EchoNumbersTagKey( DCM_EchoNumbers );										//DcmTagKey(0x0018, 0x0086)
			DcmTagKey MagneticFieldStrengthTagKey( DCM_MagneticFieldStrength );					//DcmTagKey(0x0018, 0x0087)
			DcmTagKey SpacingBetweenSlicesTagKey( DCM_SpacingBetweenSlices );					//DcmTagKey(0x0018, 0x0088)
			DcmTagKey NumberOfPhaseEncodingStepsTagKey( DCM_NumberOfPhaseEncodingSteps );       //DcmTagKey(0x0018, 0x0089)
			DcmTagKey DataCollectionDiameter( DCM_DataCollectionDiameter );						//DcmTagKey(0x0018, 0x0090)
			DcmTagKey EchoTrainLengthTagKey( DCM_EchoTrainLength );								//DcmTagKey(0x0018, 0x0091)
			DcmTagKey PixelBandwidthTagKey( DCM_PixelBandwidth );								//DcmTagKey(0x0018, 0x0095)
			DcmTagKey SoftwareVersionsTagKey( DCM_SoftwareVersions );							//DcmTagKey(0x0018, 0x1020)
			DcmTagKey ProtocolNameTagKey( DCM_ProtocolName );									//DcmTagKey(0x0018, 0x1030)
			DcmTagKey TriggerTimeTagKey( DCM_TriggerTime );										//DcmTagKey(0x0018, 0x1060)
			DcmTagKey SkipBeatsTagKey( DCM_SkipBeats );											//DcmTagKey(0x0018, 0x1086)
			DcmTagKey HeartRateTagKey( DCM_HeartRate );											//DcmTagKey(0x0018, 0x1088)
			DcmTagKey CardiacNumberOfImagesTagKey( DCM_CardiacNumberOfImages );					//DcmTagKey(0x0018, 0x1090)
			DcmTagKey ReconstructionDiameterTagKey( DCM_ReconstructionDiameter );				//DcmTagKey(0x0018, 0x1100)
			DcmTagKey GantryDetectorTiltTagKey( DCM_GantryDetectorTilt );						//DcmTagKey(0x0018, 0x1120)
			DcmTagKey TableHeightTagKey( DCM_TableHeight );										//DcmTagKey(0x0018, 0x1130)
			DcmTagKey RotationDirectionTagKey( DCM_RotationDirection );							//DcmTagKey(0x0018, 0x1140)
			DcmTagKey XRayTubeCurrentTagKey( DCM_XRayTubeCurrent );								//DcmTagKey(0x0018, 0x1151)
			DcmTagKey ExposureTagKey( DCM_Exposure );											//DcmTagKey(0x0018, 0x1152)
			DcmTagKey DateOfLastCalibrationTagKey( DCM_DateOfLastCalibration );					//DcmTagKey(0x0018, 0x1200)
			DcmTagKey TimeOfLastCalibrationTagKey( DCM_TimeOfLastCalibration );					//DcmTagKey(0x0018, 0x1201)
			DcmTagKey ReceiveCoilNameTagKey( DCM_ReceiveCoilName );								//DcmTagKey(0x0018, 0x1250)
			DcmTagKey TransmitCoilNameTagKey( DCM_TransmitCoilName );							//DcmTagKey(0x0018, 0x1251)
			DcmTagKey AcquisitionMatrixTagKey( DCM_AcquisitionMatrix );							//DcmTagKey(0x0018, 0x1310)
			DcmTagKey InPlanePhaseEncodingDirectionTagKey( DCM_InPlanePhaseEncodingDirection ); //DcmTagKey(0x0018, 0x1312)
			DcmTagKey FlipAngleTagKey( DCM_FlipAngle );											//DcmTagKey(0x0018, 0x1314)
			DcmTagKey SARTagKey( DCM_SAR );														//DcmTagKey(0x0018, 0x1316)
			DcmTagKey PatientPositionTagKey( DCM_PatientPosition );								//DcmTagKey(0x0018, 0x5100)
			DcmTagKey StudyIDTagKey( DCM_StudyID );												//DcmTagKey(0x0020, 0x0010)
			DcmTagKey SeriesNumberTagKey( DCM_SeriesNumber );									//DcmTagKey(0x0020, 0x0011)
			DcmTagKey InstanceNumberTagKey( DCM_InstanceNumber );								//DcmTagKey(0x0020, 0x0013)
			DcmTagKey ImagePositionPatientTagKey( DCM_ImagePositionPatient );					//DcmTagKey(0x0020, 0x0032)
			DcmTagKey ImageOrientationPatientTagKey( DCM_ImageOrientationPatient );				//DcmTagKey(0x0020, 0x0037)
			DcmTagKey ImagesInAcquisitionTagKey( DCM_ImagesInAcquisition );						//DcmTagKey(0x0020, 0x1002)
			DcmTagKey SliceLocationTagKey( DCM_SliceLocation );									//DcmTagKey(0x0020, 0x1041)
			DcmTagKey RowsTagKey( DCM_Rows );													//DcmTagKey(0x0028, 0x0010)
			DcmTagKey ColumnsTagKey( DCM_Columns );												//DcmTagKey(0x0028, 0x0011)
			DcmTagKey PixelSpacingTagKey( DCM_PixelSpacing );									//DcmTagKey(0x0028, 0x0030)
			DcmTagKey WindowCenterTagKey( DCM_WindowCenter );									//DcmTagKey(0x0028, 0x1050)
			DcmTagKey WindowWidthTagKey( DCM_WindowWidth );										//DcmTagKey(0x0028, 0x1051)
			DcmTagKey RescaleInterceptTagKey( DCM_RescaleIntercept );							//DcmTagKey(0x0028, 0x1052)	
			DcmTagKey RescaleSlopeTagKey( DCM_RescaleSlope );									//DcmTagKey(0x0028, 0x1053)
			DcmTagKey PixelDataTagKey( DCM_PixelData );											//DcmTagKey(0x7fe0, 0x0010)
			
			char *lpTemp = NULL;
			OFString currentValue;
			
			char			szTemp[64] ;
			unsigned short	sPoints ;
			int				count = 0 ;
			int				i     = 0 ;
			double			dVal[20] ;
			double			dImgDirec[6] ;
			double			dXBImgDirec[12] ;
			double			dOffSet[3] ;
			double			dSliceThick = 5.0;
			double          dSliceLocation = 0.0;
			short			nRescaleIntercept=0;
			short			nRescaleSlope=1;
			int				nImageSize = 1 ;
			int				nVM;
			int				nModality = -1;

			//DcmTagKey(0x0008, 0x0060)
			OFCondition eb = (*imageDataSet)->findAndGetString( ModalityTagKey, lpTemp, OFFalse);   
			if(eb != EC_Normal)
			{
				//cout<<"Unable to get element: DCM_Modality\n";
				return;
			}
			else
			{				
				if ((strncmp(lpTemp, "CT", strlen(lpTemp)) == 0) ||					
					(strncmp(lpTemp, "DF", strlen(lpTemp)) == 0) ||					
					(strncmp(lpTemp, "DS", strlen(lpTemp)) == 0) ||					
					(strncmp(lpTemp, "DX", strlen(lpTemp)) == 0) ||					
					(strncmp(lpTemp, "MG", strlen(lpTemp)) == 0) ||					
					(strncmp(lpTemp, "PX", strlen(lpTemp)) == 0) ||					
					(strncmp(lpTemp, "RF", strlen(lpTemp)) == 0) ||					
					(strncmp(lpTemp, "RG", strlen(lpTemp)) == 0) ||					
					(strncmp(lpTemp, "XA", strlen(lpTemp)) == 0))					
				{					
					nModality = M_CT;					
				}
				else if ((strncmp(lpTemp, "MA", strlen(lpTemp)) == 0) ||					
						 (strncmp(lpTemp, "MR", strlen(lpTemp)) == 0) ||					
						 (strncmp(lpTemp, "MS", strlen(lpTemp)) == 0))					
				{					
					nModality = M_MRI;					
				}				
				else if ((strncmp(lpTemp, "ST", strlen(lpTemp)) == 0) ||					
						 (strncmp(lpTemp, "NM", strlen(lpTemp)) == 0))					
				{					
					nModality = M_SPECT;					
				}				
				else if ((strncmp(lpTemp, "PT", strlen(lpTemp)) == 0))					
				{					
					nModality = M_PET;					
				}				
				else if ((strncmp(lpTemp, "CR", strlen(lpTemp)) == 0))					
				{					
					nModality = M_CR;					
				}			
				else if ((strncmp(lpTemp, "US", strlen(lpTemp)) == 0))					
				{					
					nModality = M_US;					
				}				
				else
				{				
					nModality = M_UNKNOWN;					
				}
			}		
			
			if(nModality == M_MRI)
			{
				if(g_MRImgData)
				{
					delete g_MRImgData->pData;
					delete g_MRImgData;
					g_MRImgData = NULL;
				}

				g_MRImgData = new MR_STRUCT_IMAGE;
				//construct MR_STRUCT_IMAGE
				memset(&(g_MRImgData->oHeader.headInfo), 0 ,sizeof(MR_ImageHeader) );
				// Initialize File Header
				g_MRImgData->oHeader.headInfo.BITitle[0] = 'D';
				g_MRImgData->oHeader.headInfo.BITitle[1] = 'C';
				g_MRImgData->oHeader.headInfo.BITitle[2] = 'M';
				g_MRImgData->oHeader.headInfo.BITitle[3] = 'M';
				g_MRImgData->oHeader.headInfo.BITitle[4] = 'R';
				g_MRImgData->oHeader.headInfo.Version = int (1) << 16 + 0;
				g_MRImgData->oHeader.headInfo.Size = sizeof (MR_HeaderInfo);
				g_MRImgData->oHeader.headInfo.NumHdrFields = 6; 
				g_MRImgData->oHeader.headInfo.HeaderSize =  sizeof(MR_HeaderInfo)   + sizeof(MR_SiteField) +
															sizeof(MR_PatientField) + sizeof(MR_StudyField)+ 
															sizeof(MR_SeriesField)  + sizeof(MR_ScanField) + 
															sizeof (MR_ImageField);
				
				//SiteField
				g_MRImgData->oHeader.siteField.Size = sizeof(MR_SiteField);
				
				//PatientField
				g_MRImgData->oHeader.patientField.Size = sizeof(MR_PatientField);
				
				//StudyField
				g_MRImgData->oHeader.studyField.Size = sizeof(MR_StudyField);
				
				//SeriesField
				g_MRImgData->oHeader.seriesField.Size = sizeof(MR_SeriesField);
				g_MRImgData->oHeader.seriesField.NumSlab = 1 ;
				g_MRImgData->oHeader.seriesField.RcvCoil = ctSystemCoil;
				g_MRImgData->oHeader.seriesField.TXCoil  = ctSystemCoil;
				
				//ScanField
				g_MRImgData->oHeader.scanField.Size = sizeof(MR_ScanField);
				
				//ImageField
				g_MRImgData->oHeader.imageField.Size = sizeof(MR_ImageField);
				
				//DcmTagKey(0x0002, 0x0002)
				eb = (*imageDataSet)->findAndGetString( MediaStorageSOPClassUIDTagKey, lpTemp, OFFalse);   
				if(eb == EC_Normal && lpTemp != NULL)
				{
					if(strncmp(lpTemp, "1.2.840.10008.5.1.4.1.1.4", strlen(lpTemp)) != 0)
						return;
				}					
				//DcmTagKey(0x0002, 0x0010)
				eb = (*imageDataSet)->findAndGetString( TransferSyntaxUIDTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					if(strncmp(lpTemp, "1.2.840.10008.1.2", strlen(lpTemp)) != 0)
						return;
				}
				//DcmTagKey(0x0008, 0x0008)
				eb = (*imageDataSet)->findAndGetString( ImageTypeTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
				{
					if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\MPR", strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.imageField.Type = itMPR;
					}
					else if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\PROJECTION IMAGE ", strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.imageField.Type = itProjection;
					}
					else if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\IMAGE",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.imageField.Type=itImage;
					}
					else if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\T1 MAP",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.imageField.Type = itT1Map ;
					}
					else if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\T2 MAP", strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.imageField.Type=itT2Map;
					}
					else if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\DIFFUSION MAP", strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.imageField.Type=itDiffusionMap;
					}
					else if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\DENSITY MAP",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.imageField.Type = itDensityMap;
					}
					else if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\PHASE MAP",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.imageField.Type = itPhaseMap;
					}
					else if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\VELOCITY MAP",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.imageField.Type = itVelocityMap;
					}
					else if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\IMAGE ADDITION",strlen(lpTemp))==0)
					{
						g_MRImgData->oHeader.imageField.Type = itImageAddition ;
					}
					else if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\PHASE SUBTRACT",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.imageField.Type = itPhaseSubtract ;
					}
					else if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\MODULUS SUBTRACT", strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.imageField.Type = itModulusSubtract ;
					}
					else if(strncmp(lpTemp,"ORIGINAL\\PRIMARY\\OTHER", strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.imageField.Type = itOther ;
					}
				}
				//DcmTagKey(0x0008, 0x0016)
				eb = (*imageDataSet)->findAndGetString( SOPClassUIDTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
				{
					if (strncmp(lpTemp, "1.2.840.10008.5.1.4.1.1.4", strlen(lpTemp)) != 0)
						return;
				}
				//DcmTagKey(0x0008, 0x0020)
				eb = (*imageDataSet)->findAndGetString( StudyDateTagKey, lpTemp, OFFalse );   
				if(eb == EC_Normal && lpTemp != NULL)
					TimeConvert2(g_MRImgData->oHeader.studyField.Time, lpTemp, 1);
				//DcmTagKey(0x0008, 0x0021)
				eb = (*imageDataSet)->findAndGetString( SeriesDateTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
					TimeConvert2(g_MRImgData->oHeader.seriesField.DateTime, lpTemp, 1);
				//DcmTagKey(0x0008, 0x0022)
				eb = (*imageDataSet)->findAndGetString( AcquisitionDateTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
					TimeConvert2(g_MRImgData->oHeader.imageField.DateTime, lpTemp, 1);
				//DcmTagKey(0x0008, 0x0030)
				eb = (*imageDataSet)->findAndGetString( StudyTimeTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)	
					TimeConvert2(g_MRImgData->oHeader.studyField.Time, lpTemp, 2);
				//DcmTagKey(0x0008, 0x0031)
				eb = (*imageDataSet)->findAndGetString( SeriesTimeTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
					TimeConvert2(g_MRImgData->oHeader.seriesField.DateTime, lpTemp, 2);
				//DcmTagKey(0x0008, 0x0032)
				eb = (*imageDataSet)->findAndGetString( AcquisitionTimeTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
					TimeConvert2(g_MRImgData->oHeader.imageField.DateTime, lpTemp, 2);
				//DcmTagKey(0x0008, 0x0080)
				eb = (*imageDataSet)->findAndGetString( InstitutionNameTagKey, lpTemp, OFFalse );  
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(g_MRImgData->oHeader.siteField.InstName, lpTemp, strlen(lpTemp));
					if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
					{
						g_MRImgData->oHeader.siteField.InstName[strlen(lpTemp)-1] = 0x00;
					}
				}
				//DcmTagKey(0x0008, 0x0090)
				eb = (*imageDataSet)->findAndGetString( ReferringPhysiciansNameTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
					NameConvert2(g_MRImgData->oHeader.studyField.RefPhy,lpTemp,strlen(lpTemp));
				//DcmTagKey(0x0008, 0x1010)
				eb = (*imageDataSet)->findAndGetString( StationNameTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(g_MRImgData->oHeader.siteField.StationName,lpTemp,strlen(lpTemp));
					if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
					{
						g_MRImgData->oHeader.siteField.StationName[strlen(lpTemp)-1] = 0x00;
					}
				}
				//DcmTagKey(0x0008, 0x1030)
				eb = (*imageDataSet)->findAndGetString( StudyDescriptionTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(g_MRImgData->oHeader.studyField.Desc, lpTemp, strlen(lpTemp));
					if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
					{
						g_MRImgData->oHeader.studyField.Desc[strlen(lpTemp)-1] = 0x00;
					}
				}
				//DcmTagKey(0x0008, 0x103e)
				eb = (*imageDataSet)->findAndGetString( SeriesDescriptionTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(g_MRImgData->oHeader.seriesField.Desc, lpTemp, strlen(lpTemp));
					if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
					{
						g_MRImgData->oHeader.seriesField.Desc[strlen(lpTemp)-1] = 0x00;
					}
				}
				//DcmTagKey(0x0008, 0x1040)
				eb = (*imageDataSet)->findAndGetString( InstitutionalDepartmentNameTagKey,lpTemp,OFFalse);
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(g_MRImgData->oHeader.siteField.InstDeptName, lpTemp, strlen(lpTemp));
					if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
					{
						g_MRImgData->oHeader.siteField.InstDeptName[strlen(lpTemp)-1] = 0x00;
					}
				}
				//DcmTagKey(0x0008, 0x1090)
				eb = (*imageDataSet)->findAndGetString( ManufacturersModelNameTagKey,lpTemp,OFFalse);
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(g_MRImgData->oHeader.siteField.BISerialNum, lpTemp, strlen(lpTemp));
					if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
					{
						g_MRImgData->oHeader.siteField.BISerialNum[strlen(lpTemp)-1] = 0x00;
					}
				}
				//DcmTagKey(0x0010, 0x0010)
				eb = (*imageDataSet)->findAndGetString( PatientsNameTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					NameConvert2(g_MRImgData->oHeader.patientField.Name, lpTemp, strlen(lpTemp));
				}		
				//DcmTagKey(0x0010, 0x0020)
				eb = (*imageDataSet)->findAndGetString( PatientIDTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(g_MRImgData->oHeader.patientField.Id, lpTemp, strlen(lpTemp));
					if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
					{
						g_MRImgData->oHeader.patientField.Id[strlen(lpTemp)-1]=0x00;
					}
				}
				//DcmTagKey(0x0010, 0x0030)
				eb = (*imageDataSet)->findAndGetString( PatientsBirthDateTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
					TimeConvert2(g_MRImgData->oHeader.patientField.BirthDate, lpTemp, 1);
				//DcmTagKey(0x0010, 0x0032)
				eb = (*imageDataSet)->findAndGetString( PatientsBirthTimeTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
					TimeConvert2(g_MRImgData->oHeader.patientField.BirthDate, lpTemp, 2);
				//DcmTagKey(0x0010, 0x0040)
				eb = (*imageDataSet)->findAndGetString( PatientsSexTagKey, lpTemp, OFFalse );  
				if(eb == EC_Normal)
				{
					if(lpTemp == NULL)
					{
						g_MRImgData->oHeader.patientField.Sex = 'U';
					}
					else
					{
						g_MRImgData->oHeader.patientField.Sex = *lpTemp;
					}
				}
				//DcmTagKey(0x0010, 0x1010)
				eb = (*imageDataSet)->findAndGetString( PatientsAgeTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
					g_MRImgData->oHeader.patientField.Age = AgeConvert2(lpTemp);
				//DcmTagKey(0x0010, 0x1030)
				eb = (*imageDataSet)->findAndGetString( PatientsWeightTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp, lpTemp, strlen(lpTemp));
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					g_MRImgData->oHeader.patientField.Weight = atoi(szTemp);
				}
				//DcmTagKey(0x0010, 0x21b0)
				eb = (*imageDataSet)->findAndGetString( AdditionalPatientHistoryTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(g_MRImgData->oHeader.patientField.Desc,lpTemp,strlen(lpTemp));
					if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
					{
						g_MRImgData->oHeader.patientField.Desc[strlen(lpTemp)-1] = 0x00;
					}
				}
				//DcmTagKey(0x0018, 0x0015)
				eb = (*imageDataSet)->findAndGetString( BodyPartExaminedTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					if(strncmp(lpTemp, "SKULL ", strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.studyField.PatBodyPart = bpHead;
					}
					else if(strncmp(lpTemp, "NECK", strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.studyField.PatBodyPart = bpNeck;
					}
					else if(strncmp(lpTemp, "SHOULDER", strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.studyField.PatBodyPart = bpShoulder;
					}
					else if(strncmp(lpTemp, "CSPINE", strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.studyField.PatBodyPart = bpCervicalSpine;
					}
					else if(strncmp(lpTemp,"TSPINE",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.studyField.PatBodyPart = bpThoracicSpine;
					}
					else if(strncmp(lpTemp,"LSPINE",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.studyField.PatBodyPart = bpLumberSpine;
					}
					else if(strncmp(lpTemp,"CHEST ",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.studyField.PatBodyPart = bpChest;
					}
					else if(strncmp(lpTemp,"BREAST",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.studyField.PatBodyPart = bpBreast;
					}
					else if(strncmp(lpTemp,"ABODOMEN",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.studyField.PatBodyPart = bpAbdomen;
					}
					else if(strncmp(lpTemp,"PELVIS",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.studyField.PatBodyPart = bpPelvis;
					}
					else if(strncmp(lpTemp,"HIP ",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.studyField.PatBodyPart = bpHip;
					}
					else if(strncmp(lpTemp,"ELBOW ",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.studyField.PatBodyPart = bpElbow;
					}
					else if(strncmp(lpTemp,"WRIST ",strlen(lpTemp)) == 0)
						g_MRImgData->oHeader.studyField.PatBodyPart = bpWrist;
					else if(strncmp(lpTemp,"ARM ",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.studyField.PatBodyPart = bpArm;
					}
					else if(strncmp(lpTemp,"HAND",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.studyField.PatBodyPart = bpHand;
					}
					else if(strncmp(lpTemp,"KNEE",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.studyField.PatBodyPart = bpKnee;
					}
					else if(strncmp(lpTemp,"LEG ",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.studyField.PatBodyPart = bpLeg;
					}
					else if(strncmp(lpTemp,"ANKLE ",strlen(lpTemp))==0)
					{
						g_MRImgData->oHeader.studyField.PatBodyPart = bpAnkle;
					}
					else if(strncmp(lpTemp,"FOOT",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.studyField.PatBodyPart = bpFoot;
					}
					else if(strncmp(lpTemp,"HEART ",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.studyField.PatBodyPart = bpHeart;
					}
					else if(strncmp(lpTemp,"VASCULARHEAD",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.studyField.PatBodyPart = bpVascularHead;
					}
					else if(strncmp(lpTemp,"VASCULARNECK",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.studyField.PatBodyPart = bpVascularNeck;
					}
					else if(strncmp(lpTemp,"VASCULARBODY",strlen(lpTemp))==0)
					{
						g_MRImgData->oHeader.studyField.PatBodyPart=bpVascularBody;
					}
					else if(strncmp(lpTemp,"VASCULARPER ",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.studyField.PatBodyPart = bpVascularPeripheral;
					}
				}
				//DcmTagKey(0x0018, 0x0020)
				eb = (*imageDataSet)->findAndGetString( ScanningSequenceTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(g_MRImgData->oHeader.seriesField.PulseSeqName, lpTemp, strlen(lpTemp));
					if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
					{
						g_MRImgData->oHeader.seriesField.PulseSeqName[strlen(lpTemp)-1] = 0x00;
					}
					g_MRImgData->oHeader.seriesField.SequenceOptions = GetSeriesOption(g_MRImgData->oHeader.seriesField.PulseSeqName) ;
				}
				//DcmTagKey(0x0018, 0x0023)
				eb = (*imageDataSet)->findAndGetString( MRAcquisitionTypeTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					if (strncmp(lpTemp, "2D", 2) == 0)
					{
						g_MRImgData->oHeader.seriesField.Mode = im2D ;
					}
					else if (strncmp(lpTemp, "3D", 2) == 0)
					{
						g_MRImgData->oHeader.seriesField.Mode = im3D ;
					}
				}
				//DcmTagKey(0x0018, 0x0024)
				eb = (*imageDataSet)->findAndGetString( SequenceNameTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(g_MRImgData->oHeader.seriesField.PulseSeqName, lpTemp, strlen(lpTemp));
					if(*(lpTemp+strlen(lpTemp)-1) == ' ')
					{
						g_MRImgData->oHeader.seriesField.PulseSeqName[strlen(lpTemp)-1] = 0x00;
					}
				}
				//DcmTagKey(0x0018, 0x0050)			
				eb = (*imageDataSet)->findAndGetString( SliceThicknessTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp, lpTemp, strlen(lpTemp));
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					g_MRImgData->oHeader.seriesField.SliceThickness = (float)atof(szTemp);
					dSliceThick = g_MRImgData->oHeader.seriesField.SliceThickness ;
				}
				//DcmTagKey(0x0018, 0x0080)
				eb = (*imageDataSet)->findAndGetString( RepetitionTimeTagKey, lpTemp, OFFalse );  
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp, lpTemp, strlen(lpTemp));
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					g_MRImgData->oHeader.seriesField.RepTime = strtoul(szTemp,NULL,10);
				}
				//DcmTagKey(0x0018, 0x0081)
				eb = (*imageDataSet)->findAndGetString( EchoTimeTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp, lpTemp, strlen(lpTemp));
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					g_MRImgData->oHeader.seriesField.EchoTime = strtoul(szTemp,NULL,10);
				}
				//DcmTagKey(0x0018, 0x0082)
				eb = (*imageDataSet)->findAndGetString( InversionTimeTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp,lpTemp,strlen(lpTemp));
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					g_MRImgData->oHeader.seriesField.TI = strtoul(szTemp,NULL,10);
				}
				//DcmTagKey(0x0018, 0x0083)
				eb = (*imageDataSet)->findAndGetString( NumberOfAveragesTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp,lpTemp,strlen(lpTemp));
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					g_MRImgData->oHeader.seriesField.NumAvg = (short)strtoul(szTemp,NULL,10);
				}
				//DcmTagKey(0x0018, 0x0084)
				eb = (*imageDataSet)->findAndGetString( ImagingFrequencyTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp,lpTemp,strlen(lpTemp));
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					g_MRImgData->oHeader.siteField.SF = (float)strtod(szTemp,NULL);
				}
				//DcmTagKey(0x0018, 0x0085)			
				eb = (*imageDataSet)->findAndGetString( ImagedNucleusTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
				{
					if(strncmp(lpTemp,"1H",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.seriesField.NucType = ntH1;
					}
					else if(strncmp(lpTemp,"3He ",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.seriesField.NucType = ntHe3;
					}
					else if(strncmp(lpTemp,"13C ",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.seriesField.NucType = ntC13;
					}
					else if(strncmp(lpTemp,"19F ",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.seriesField.NucType = ntF19;
					}
					else if(strncmp(lpTemp,"23Na",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.seriesField.NucType = ntNa23;
					}
					else if(strncmp(lpTemp,"31P ",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.seriesField.NucType = ntP31;
					}
					else if(strncmp(lpTemp,"39K ",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.seriesField.NucType = ntK39;
					}
					else if(strncmp(lpTemp,"129Xe ",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.seriesField.NucType = ntXe129;
					}
				}
				//DcmTagKey(0x0018, 0x0086)
				eb = (*imageDataSet)->findAndGetString( EchoNumbersTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp,lpTemp,strlen(lpTemp));
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					g_MRImgData->oHeader.imageField.EchoNum = (unsigned short)strtoul(szTemp,NULL,10);
				}
				//DcmTagKey(0x0018, 0x0087)
				eb = (*imageDataSet)->findAndGetString( MagneticFieldStrengthTagKey,lpTemp,OFFalse);
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp,lpTemp,strlen(lpTemp));
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					g_MRImgData->oHeader.siteField.MagCalibStrength = (float)strtod(szTemp,NULL);
				}
				//DcmTagKey(0x0018, 0x0088)
				eb = (*imageDataSet)->findAndGetString( SpacingBetweenSlicesTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp, lpTemp ,strlen(lpTemp));
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					g_MRImgData->oHeader.seriesField.SliceGap  = (float)strtod(szTemp,NULL);
					g_MRImgData->oHeader.seriesField.SliceGap -= (float)dSliceThick ;
				}
				//DcmTagKey(0x0018, 0x0089)
				eb = (*imageDataSet)->findAndGetString( NumberOfPhaseEncodingStepsTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp, lpTemp, strlen(lpTemp)) ;
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					g_MRImgData->oHeader.seriesField.PhasePoints = (unsigned short)strtoul(szTemp,NULL,10);
				}
				//DcmTagKey(0x0018, 0x0091)
				eb = (*imageDataSet)->findAndGetString( EchoTrainLengthTagKey, lpTemp, OFFalse );  
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp, lpTemp, strlen(lpTemp));
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					g_MRImgData->oHeader.seriesField.EchoTrainLength = (unsigned short)strtoul(szTemp,NULL,10);
				}
				//DcmTagKey(0x0018, 0x0095)
				eb = (*imageDataSet)->findAndGetString( PixelBandwidthTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp, lpTemp, strlen(lpTemp));
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					g_MRImgData->oHeader.seriesField.AcqBandwidth = (float)strtod(szTemp,NULL);
				}
				//DcmTagKey(0x0018, 0x1030)
				eb = (*imageDataSet)->findAndGetString( ProtocolNameTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(g_MRImgData->oHeader.seriesField.Protocol, lpTemp, strlen(lpTemp));
					if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
					{
						g_MRImgData->oHeader.seriesField.Protocol[strlen(lpTemp)-1] = 0x00;
					}
				}
				//DcmTagKey(0x0018, 0x1060)
				eb = (*imageDataSet)->findAndGetString( TriggerTimeTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
					strncpy(szTemp, lpTemp, strlen(lpTemp));
				//DcmTagKey(0x0018, 0x1086)
				eb = (*imageDataSet)->findAndGetString( SkipBeatsTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
					strncpy(szTemp,lpTemp,strlen(lpTemp));
				//DcmTagKey(0x0018, 0x1088)
				eb = (*imageDataSet)->findAndGetString( HeartRateTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
					strncpy(szTemp,lpTemp,strlen(lpTemp));
				
				eb = (*imageDataSet)->findAndGetString( CardiacNumberOfImagesTagKey, lpTemp, OFFalse );
				//DcmTagKey(0x0018, 0x1200)
				eb = (*imageDataSet)->findAndGetString( DateOfLastCalibrationTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
					TimeConvert2(g_MRImgData->oHeader.siteField.LastCalib,lpTemp,1);
				//DcmTagKey(0x0018, 0x1201)
				eb = (*imageDataSet)->findAndGetString( TimeOfLastCalibrationTagKey, lpTemp, OFFalse );  
				if(eb == EC_Normal && lpTemp != NULL)
					TimeConvert2(g_MRImgData->oHeader.siteField.LastCalib,lpTemp,2);
				//DcmTagKey(0x0018, 0x1250)
				eb = (*imageDataSet)->findAndGetString( ReceiveCoilNameTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(g_MRImgData->oHeader.seriesField.UserRcvCoilDesc, lpTemp, strlen(lpTemp));
					if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
					{
						g_MRImgData->oHeader.seriesField.UserRcvCoilDesc[strlen(lpTemp)-1] = 0x00;
					}
				}
				//DcmTagKey(0x0018, 0x1251)
				eb = (*imageDataSet)->findAndGetString( TransmitCoilNameTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(g_MRImgData->oHeader.seriesField.UserTxCoilDesc, lpTemp, strlen(lpTemp));
					if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
					{
						g_MRImgData->oHeader.seriesField.UserTxCoilDesc[strlen(lpTemp)-1] = 0x00;
					}
				}
				//DcmTagKey(0x0018, 0x1310)
				eb = (*imageDataSet)->findAndGetString( AcquisitionMatrixTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					sPoints  = *(unsigned short*)lpTemp;
					lpTemp += 2 ;
					if (sPoints > 0)
					{
						g_MRImgData->oHeader.seriesField.ReadPoints  = sPoints;
					}
					sPoints  = *(unsigned short*)lpTemp ;
					lpTemp += 2 ;
					if (sPoints > 0)
					{
						g_MRImgData->oHeader.seriesField.ReadPoints  = sPoints;
					}
					sPoints  = *(unsigned short*)lpTemp ;
					lpTemp += 2 ;
					if (sPoints > 0)
					{
						g_MRImgData->oHeader.seriesField.PhasePoints  = sPoints;
					}
					sPoints  = *(unsigned short*)lpTemp ;
					lpTemp += 2 ;
					if (sPoints > 0)
					{
						g_MRImgData->oHeader.seriesField.PhasePoints  = sPoints;
					}
					lpTemp -= 8 ;
				}
				//DcmTagKey(0x0018, 0x1312)
				eb = (*imageDataSet)->findAndGetString( InPlanePhaseEncodingDirectionTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					if (strncmp(lpTemp, "ROW", 3) == 0)
					{
						g_MRImgData->oHeader.seriesField.OtherOptions += 0x400 ;
					}
				}
				//DcmTagKey(0x0018, 0x1314)
				eb = (*imageDataSet)->findAndGetString( FlipAngleTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp, lpTemp, strlen(lpTemp));
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					g_MRImgData->oHeader.seriesField.FlipAngle = (float)atof(szTemp);
				}
				//DcmTagKey(0x0018, 0x1316)
				eb = (*imageDataSet)->findAndGetString( SARTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp, lpTemp, strlen(lpTemp));
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					g_MRImgData->oHeader.seriesField.SARAvg = (int)strtod(szTemp,NULL);
				}
				//DcmTagKey(0x0018, 0x5100)
				eb = (*imageDataSet)->findAndGetString( PatientPositionTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					if(strncmp(lpTemp,"HFS ",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.studyField.PatEntry    = beHeadFirst;
						g_MRImgData->oHeader.studyField.PatPosition = bpSupine;
					}
					else if(strncmp(lpTemp,"HFP ",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.studyField.PatEntry    = beHeadFirst;
						g_MRImgData->oHeader.studyField.PatPosition = bpProne;
					}
					else if(strncmp(lpTemp,"HFDR",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.studyField.PatEntry    = beHeadFirst;
						g_MRImgData->oHeader.studyField.PatPosition = bpDecubitusRight;
					}
					else if(strncmp(lpTemp,"HFDL",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.studyField.PatEntry    = beHeadFirst;
						g_MRImgData->oHeader.studyField.PatPosition = bpDecubitusLeft;
					}
					else if(strncmp(lpTemp,"FFS ",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.studyField.PatEntry    = beFootFirst;
						g_MRImgData->oHeader.studyField.PatPosition = bpSupine;
					}
					else if(strncmp(lpTemp,"FFP ",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.studyField.PatEntry    = beFootFirst;
						g_MRImgData->oHeader.studyField.PatPosition = bpProne;
					}
					else if(strncmp(lpTemp,"FFDR",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.studyField.PatEntry    = beFootFirst;
						g_MRImgData->oHeader.studyField.PatPosition = bpDecubitusRight;
					}
					else if(strncmp(lpTemp,"FFDL",strlen(lpTemp)) == 0)
					{
						g_MRImgData->oHeader.studyField.PatEntry    = beFootFirst;
						g_MRImgData->oHeader.studyField.PatPosition = bpDecubitusLeft;
					}
				}
				//DcmTagKey(0x0020, 0x0010)
				eb = (*imageDataSet)->findAndGetString( StudyIDTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(g_MRImgData->oHeader.studyField.Id, lpTemp, strlen(lpTemp));
					if(*(lpTemp+strlen(lpTemp)-1) == ' ')
					{
						g_MRImgData->oHeader.studyField.Id[strlen(lpTemp)-1]=0x00;
					}
				}
				//DcmTagKey(0x0020, 0x0011)
				eb = (*imageDataSet)->findAndGetString( SeriesNumberTagKey, lpTemp, OFFalse );  
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp, lpTemp, strlen(lpTemp));
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					g_MRImgData->oHeader.seriesField.Id = (BYTE)strtoul(szTemp,NULL,10);
				}
				//DcmTagKey(0x0020, 0x0013)
				eb = (*imageDataSet)->findAndGetString( InstanceNumberTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp, lpTemp, strlen(lpTemp));
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					g_MRImgData->oHeader.imageField.Id = (unsigned short)strtoul(szTemp,NULL,10);
				}
				//DcmTagKey(0x0020, 0x0032)
				eb = (*imageDataSet)->findAndGetString( ImagePositionPatientTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
				{
					GetDSVM(lpTemp, strlen(lpTemp), dVal, nVM);
					if (nVM != 3)
					{
						return ;
					}
					dOffSet[0] = dVal[0] ;
					dOffSet[1] = dVal[1] ;
					dOffSet[2] = dVal[2] ;
					g_MRImgData->oHeader.scanField.SliceOffset = 0 ;
					g_MRImgData->oHeader.scanField.ReadOffset  = 0 ;
					g_MRImgData->oHeader.scanField.PhaseOffset = 0 ;
				}
				//DcmTagKey(0x0020, 0x0037)
				eb = (*imageDataSet)->findAndGetString( ImageOrientationPatientTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					GetDSVM(lpTemp, strlen(lpTemp), dVal, nVM);
					if (nVM != 6)
					{
						return ;
					}
					else
					{
						dImgDirec[0] = dVal[0] ;
						dImgDirec[1] = dVal[1] ;
						dImgDirec[2] = dVal[2] ;
						dImgDirec[3] = dVal[3] ;
						dImgDirec[4] = dVal[4] ;
						dImgDirec[5] = dVal[5] ;
					}
				}
				//DcmTagKey(0x0020, 0x1002)
				eb = (*imageDataSet)->findAndGetString( ImagesInAcquisitionTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp, lpTemp, strlen(lpTemp));
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					g_MRImgData->oHeader.seriesField.ResolutionZ = (unsigned short)strtoul(szTemp,NULL,10);
				}
				//DcmTagKey(0x0028, 0x0010)
				Uint16 RowsValue;
				UINT16 ColumnsValue;
				eb = (*imageDataSet)->findAndGetUint16( RowsTagKey, RowsValue, OFFalse );
				if(eb == EC_Normal)
					g_MRImgData->oHeader.seriesField.ResolutionX = RowsValue;
				//DcmTagKey(0x0028, 0x0011)
				eb = (*imageDataSet)->findAndGetUint16( ColumnsTagKey, ColumnsValue, OFFalse );
				if(eb == EC_Normal)
					g_MRImgData->oHeader.seriesField.ResolutionY = ColumnsValue ;
				//DcmTagKey(0x0028, 0x0030)
				eb = (*imageDataSet)->findAndGetString( PixelSpacingTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
				{
					GetDSVM(lpTemp, strlen(lpTemp), dVal, nVM);
					if (nVM == 2)
					{
						g_MRImgData->oHeader.seriesField.FovX = 
							(unsigned short)((double)g_MRImgData->oHeader.seriesField.ResolutionX*dVal[0]) ;
						g_MRImgData->oHeader.seriesField.FovY = 
							(unsigned short)((double)g_MRImgData->oHeader.seriesField.ResolutionY*dVal[1]) ;
						CalcBIImgDirec(dImgDirec, dOffSet ,
							g_MRImgData->oHeader.studyField.PatPosition, 
							g_MRImgData->oHeader.studyField.PatEntry, 
							g_MRImgData->oHeader.seriesField.FovX,
							g_MRImgData->oHeader.seriesField.FovY,
							dXBImgDirec) ;
						g_MRImgData->oHeader.scanField.FreqDirX  = (float)dXBImgDirec[0] ;
						g_MRImgData->oHeader.scanField.FreqDirY  = (float)dXBImgDirec[1] ;
						g_MRImgData->oHeader.scanField.FreqDirZ  = (float)dXBImgDirec[2] ;
						g_MRImgData->oHeader.scanField.PhaseDirX = (float)dXBImgDirec[3] ;
						g_MRImgData->oHeader.scanField.PhaseDirY = (float)dXBImgDirec[4] ;
						g_MRImgData->oHeader.scanField.PhaseDirZ = (float)dXBImgDirec[5] ;
						g_MRImgData->oHeader.scanField.SliceDirX = (float)dXBImgDirec[6] ;
						g_MRImgData->oHeader.scanField.SliceDirY = (float)dXBImgDirec[7] ;
						g_MRImgData->oHeader.scanField.SliceDirZ = (float)dXBImgDirec[8] ;
						
						g_MRImgData->oHeader.imageField.fFreqDirX     = (float)dXBImgDirec[0] ;
						g_MRImgData->oHeader.imageField.fFreqDirY     = (float)dXBImgDirec[1] ;
						g_MRImgData->oHeader.imageField.fFreqDirZ     = (float)dXBImgDirec[2] ;
						g_MRImgData->oHeader.imageField.fPhaseDirX    = (float)dXBImgDirec[3] ;
						g_MRImgData->oHeader.imageField.fPhaseDirY    = (float)dXBImgDirec[4] ;
						g_MRImgData->oHeader.imageField.fPhaseDirZ    = (float)dXBImgDirec[5] ;
						g_MRImgData->oHeader.imageField.fSliceDirX    = (float)dXBImgDirec[6] ;
						g_MRImgData->oHeader.imageField.fSliceDirY    = (float)dXBImgDirec[7] ;
						g_MRImgData->oHeader.imageField.fSliceDirZ    = (float)dXBImgDirec[8] ;
						g_MRImgData->oHeader.imageField.fCenterPointX = (float)dXBImgDirec[9] ;
						g_MRImgData->oHeader.imageField.fCenterPointY = (float)dXBImgDirec[10] ;
						g_MRImgData->oHeader.imageField.fCenterPointZ = (float)dXBImgDirec[11] ;
					}
					else
					{
						return;
					}
				}
				//DcmTagKey(0x0028, 0x1050)
				eb = (*imageDataSet)->findAndGetString( WindowCenterTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp, lpTemp, strlen(lpTemp));
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					g_MRImgData->oHeader.imageField.WindowLevel = (short)strtoul(szTemp,NULL,10);
				}
				//DcmTagKey(0x0028, 0x1051)
				eb = (*imageDataSet)->findAndGetString( WindowWidthTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp, lpTemp, strlen(lpTemp));
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					g_MRImgData->oHeader.imageField.WindowWidth = (short)strtoul(szTemp,NULL,10);
				}
				
				const Uint16 *pixelData = NULL;
				unsigned long cnt = 0; 
				//DcmTagKey(0x7fe0, 0x0010)
				if ((*imageDataSet)->findAndGetUint16Array(PixelDataTagKey, pixelData, &cnt).good())
				{ 
					int nImgSize = 2*g_MRImgData->oHeader.seriesField.ResolutionX*g_MRImgData->oHeader.seriesField.ResolutionY ;
					g_MRImgData->oHeader.headInfo.DataMatrixSize = nImgSize ;
					nImageSize   = g_MRImgData->oHeader.seriesField.ResolutionX*g_MRImgData->oHeader.seriesField.ResolutionY ;
					
					g_MRImgData->pData = new WORD[nImageSize];
					if (g_MRImgData->pData != NULL)
					{
						memcpy(g_MRImgData->pData, pixelData, nImageSize*2);					
					}
					else
					{
						return;
					}
				} 
				
				pCBI->iModality = M_MRI;
				pCBI->pDcmData = g_MRImgData;
				::PostMessage((HWND)pCBI->hwnd,WM_XBDICOMMSG,STORAGE_RECEIVE_MRSERIES,NULL);

			}
			else if (nModality == M_CT)
			{
				if(g_CTImgData)
				{
					delete g_CTImgData->pData;
 					delete g_CTImgData;
					g_CTImgData = NULL;
				}

				g_CTImgData = new CT_STRUCT_IMAGE;
				
				memset(&(g_CTImgData->oHeader.headInfo), 0 ,sizeof(g_CTImgData->oHeader) );
				// Initialize File Header
				//headInfo
				g_CTImgData->oHeader.headInfo.BITitle[0] = 'D';
				g_CTImgData->oHeader.headInfo.BITitle[1] = 'C';
				g_CTImgData->oHeader.headInfo.BITitle[2] = 'M';
				g_CTImgData->oHeader.headInfo.BITitle[3] = 'C';
				g_CTImgData->oHeader.headInfo.BITitle[4] = 'T';
				g_CTImgData->oHeader.headInfo.Version = int (1) << 16 + 0;
				
				//DcmTagKey(0x0002, 0x0002)
				eb = (*imageDataSet)->findAndGetString( MediaStorageSOPClassUIDTagKey, lpTemp, OFFalse);   
				if(eb == EC_Normal && lpTemp != NULL)
				{
					if(strncmp(lpTemp, "1.2.840.10008.5.1.4.1.1.2", strlen(lpTemp)) != 0)
						return;
				}			
				//DcmTagKey(0x0002, 0x0010)
				eb = (*imageDataSet)->findAndGetString( TransferSyntaxUIDTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					if(strncmp(lpTemp, "1.2.840.10008.1.2", strlen(lpTemp)) != 0)
						return;
				}
				//DcmTagKey(0x0008, 0x0016)
				eb = (*imageDataSet)->findAndGetString( SOPClassUIDTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
				{
					if (strncmp(lpTemp, "1.2.840.10008.5.1.4.1.1.2", strlen(lpTemp)) != 0)
						return;
				}
				//DcmTagKey(0x0008, 0x0020)
				eb = (*imageDataSet)->findAndGetString( StudyDateTagKey, lpTemp, OFFalse );   
				if(eb == EC_Normal && lpTemp != NULL)
					TimeConvert2(g_CTImgData->oHeader.studyField.Time, lpTemp, 1);
				//DcmTagKey(0x0008, 0x0021)
				eb = (*imageDataSet)->findAndGetString( SeriesDateTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
					TimeConvert2(g_CTImgData->oHeader.seriesField.DateTime, lpTemp, 1);
				//DcmTagKey(0x0008, 0x0022)
				eb = (*imageDataSet)->findAndGetString( AcquisitionDateTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
					TimeConvert2(g_CTImgData->oHeader.imageField.DateTime, lpTemp, 1);
				//DcmTagKey(0x0008, 0x0030)
				eb = (*imageDataSet)->findAndGetString( StudyTimeTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)	
					TimeConvert2(g_CTImgData->oHeader.studyField.Time, lpTemp, 2);
				//DcmTagKey(0x0008, 0x0031)
				eb = (*imageDataSet)->findAndGetString( SeriesTimeTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
					TimeConvert2(g_CTImgData->oHeader.seriesField.DateTime, lpTemp, 2);
				//DcmTagKey(0x0008, 0x0032)
				eb = (*imageDataSet)->findAndGetString( AcquisitionTimeTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
					TimeConvert2(g_CTImgData->oHeader.imageField.DateTime, lpTemp, 2);
				//DcmTagKey(0x0008, 0x0070)
				eb = (*imageDataSet)->findAndGetString( ManufacturerTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(g_CTImgData->oHeader.siteField.Manufacturer, lpTemp, strlen(lpTemp));
					if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
					{
						g_CTImgData->oHeader.siteField.Manufacturer[strlen(lpTemp)-1] = 0x00;
					}
				}
				//DcmTagKey(0x0008, 0x0080)
				eb = (*imageDataSet)->findAndGetString( InstitutionNameTagKey, lpTemp, OFFalse );  
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(g_CTImgData->oHeader.siteField.InstName, lpTemp, strlen(lpTemp));
					if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
					{
						g_CTImgData->oHeader.siteField.InstName[strlen(lpTemp)-1] = 0x00;
					}
				}
				//DcmTagKey(0x0008, 0x1010)
				eb = (*imageDataSet)->findAndGetString( StationNameTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(g_CTImgData->oHeader.siteField.StationName,lpTemp,strlen(lpTemp));
					if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
					{
						g_CTImgData->oHeader.siteField.StationName[strlen(lpTemp)-1] = 0x00;
					}
				}
				//DcmTagKey(0x0008, 0x1030)
				eb = (*imageDataSet)->findAndGetString( StudyDescriptionTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(g_CTImgData->oHeader.studyField.Desc, lpTemp, strlen(lpTemp));
					if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
					{
						g_CTImgData->oHeader.studyField.Desc[strlen(lpTemp)-1] = 0x00;
					}
				}
				//DcmTagKey(0x0008, 0x103e)
				eb = (*imageDataSet)->findAndGetString( SeriesDescriptionTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(g_CTImgData->oHeader.seriesField.Desc, lpTemp, strlen(lpTemp));
					if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
					{
						g_CTImgData->oHeader.seriesField.Desc[strlen(lpTemp)-1] = 0x00;
					}
				}
				//DcmTagKey(0x0008, 0x1040)
				eb = (*imageDataSet)->findAndGetString( InstitutionalDepartmentNameTagKey,lpTemp,OFFalse);
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(g_CTImgData->oHeader.siteField.InstDeptName, lpTemp, strlen(lpTemp));
					if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
					{
						g_CTImgData->oHeader.siteField.InstDeptName[strlen(lpTemp)-1] = 0x00;
					}
				}
				//DcmTagKey(0x0008, 0x1090)
				eb = (*imageDataSet)->findAndGetString( ManufacturersModelNameTagKey,lpTemp,OFFalse);
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(g_CTImgData->oHeader.siteField.BISerialNum, lpTemp, strlen(lpTemp));
					if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
					{
						g_CTImgData->oHeader.siteField.BISerialNum[strlen(lpTemp)-1] = 0x00;
					}
				}
				//DcmTagKey(0x0010, 0x0010)
				eb = (*imageDataSet)->findAndGetString( PatientsNameTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					NameConvert2(g_CTImgData->oHeader.patientField.Name, lpTemp, strlen(lpTemp));
				}		
				//DcmTagKey(0x0010, 0x0020)
				eb = (*imageDataSet)->findAndGetString( PatientIDTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(g_CTImgData->oHeader.patientField.Id, lpTemp, strlen(lpTemp));
					if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
					{
						g_CTImgData->oHeader.patientField.Id[strlen(lpTemp)-1]=0x00;
					}
				}
				//DcmTagKey(0x0010, 0x0030)
				eb = (*imageDataSet)->findAndGetString( PatientsBirthDateTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
					TimeConvert2(g_CTImgData->oHeader.patientField.BirthDate, lpTemp, 1);

				//DcmTagKey(0x0010, 0x0032)
				eb = (*imageDataSet)->findAndGetString( PatientsBirthTimeTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
					TimeConvert2(g_CTImgData->oHeader.patientField.BirthDate, lpTemp, 2);
				//DcmTagKey(0x0010, 0x0040)
				eb = (*imageDataSet)->findAndGetString( PatientsSexTagKey, lpTemp, OFFalse );  
				if(eb == EC_Normal)
				{
					if(lpTemp == NULL)
					{
						g_CTImgData->oHeader.patientField.Sex = 'U';
					}
					else
					{
						g_CTImgData->oHeader.patientField.Sex = *lpTemp;
					}
				}
				//DcmTagKey(0x0010, 0x1010)
				eb = (*imageDataSet)->findAndGetString( PatientsAgeTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
					g_CTImgData->oHeader.patientField.Age = AgeConvert2(lpTemp);
				//DcmTagKey(0x0010, 0x1030)
				eb = (*imageDataSet)->findAndGetString( PatientsWeightTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp, lpTemp, strlen(lpTemp));
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					g_CTImgData->oHeader.patientField.Weight = atoi(szTemp);
				}
				//DcmTagKey(0x0010, 0x21b0)
				eb = (*imageDataSet)->findAndGetString( AdditionalPatientHistoryTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(g_CTImgData->oHeader.patientField.Desc,lpTemp,strlen(lpTemp));
					if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
					{
						g_CTImgData->oHeader.patientField.Desc[strlen(lpTemp)-1] = 0x00;
					}
				}
				//DcmTagKey(0x0018, 0x0050)			
				eb = (*imageDataSet)->findAndGetString( SliceThicknessTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp, lpTemp, strlen(lpTemp));
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					g_CTImgData->oHeader.seriesField.SliceThickness = (float)atof(szTemp);
					dSliceThick = g_CTImgData->oHeader.seriesField.SliceThickness ;
				}
				//DcmTagKey(0x0018, 0x0060)			
				eb = (*imageDataSet)->findAndGetString( KVPTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp, lpTemp, strlen(lpTemp));
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					g_CTImgData->oHeader.seriesField.KVP = (unsigned short)atoi(szTemp);
				}
				//DcmTagKey(0x0018, 0x0088)
				eb = (*imageDataSet)->findAndGetString( SpacingBetweenSlicesTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp, lpTemp ,strlen(lpTemp));
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					g_CTImgData->oHeader.seriesField.SliceGap  = (float)strtod(szTemp,NULL);
					g_CTImgData->oHeader.seriesField.SliceGap -= (float)dSliceThick ;
				}
				//DcmTagKey(0x0018, 0x0090)
				eb = (*imageDataSet)->findAndGetString( DataCollectionDiameter, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp, lpTemp, strlen(lpTemp));
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					g_CTImgData->oHeader.seriesField.CollectDiameter = (unsigned short)atoi(szTemp);
				}
				//DcmTagKey(0x0018, 0x1020)
				eb = (*imageDataSet)->findAndGetString( SoftwareVersionsTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(g_CTImgData->oHeader.siteField.SoftVer, lpTemp, strlen(lpTemp));
					if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
					{
						g_CTImgData->oHeader.seriesField.Protocol[strlen(lpTemp)-1] = 0x00;
					}
				}
				//DcmTagKey(0x0018, 0x1030)
				eb = (*imageDataSet)->findAndGetString( ProtocolNameTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(g_CTImgData->oHeader.seriesField.Protocol, lpTemp, strlen(lpTemp));
					if(*(lpTemp + strlen(lpTemp) - 1) == ' ')
					{
						g_CTImgData->oHeader.seriesField.Protocol[strlen(lpTemp)-1] = 0x00;
					}
				}
				
				eb = (*imageDataSet)->findAndGetString( CardiacNumberOfImagesTagKey, lpTemp, OFFalse );
				
				//DcmTagKey(0x0018, 0x1100)
				eb = (*imageDataSet)->findAndGetString( ReconstructionDiameterTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp, lpTemp, strlen(lpTemp));
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					g_CTImgData->oHeader.seriesField.DataDiameter = (unsigned short)atoi(szTemp);
				}
				//DcmTagKey(0x0018, 0x1120)
				eb = (*imageDataSet)->findAndGetString( GantryDetectorTiltTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp, lpTemp, strlen(lpTemp));
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					g_CTImgData->oHeader.seriesField.DetectorTilt = (unsigned short)atoi(szTemp);
				}
				//DcmTagKey(0x0018, 0x1130)
				eb = (*imageDataSet)->findAndGetString( TableHeightTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp, lpTemp, strlen(lpTemp));
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					g_CTImgData->oHeader.seriesField.TableHeight = (short)atoi(szTemp);
				}
				//DcmTagKey(0x0018, 0x1140)
				eb = (*imageDataSet)->findAndGetString( RotationDirectionTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
				{
					if (strncmp(lpTemp, "CW", 2) == 0)
					{
						g_CTImgData->oHeader.seriesField.RotateDir = 1 ;
					}
					else if (strncmp(lpTemp, "CC", 2) == 0)
					{
						g_CTImgData->oHeader.seriesField.RotateDir = 2 ;
					}
					else
					{
						g_CTImgData->oHeader.seriesField.RotateDir = 0 ;
					}
				}
				//DcmTagKey(0x0018, 0x1151)
				eb = (*imageDataSet)->findAndGetString( XRayTubeCurrentTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp,lpTemp,strlen(lpTemp));
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					g_CTImgData->oHeader.seriesField.TubeCurrent = (unsigned short)atoi(szTemp);
				}
				//DcmTagKey(0x0018, 0x1152)
				eb = (*imageDataSet)->findAndGetString( ExposureTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp,lpTemp,strlen(lpTemp));
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					g_CTImgData->oHeader.seriesField.Exposure = (unsigned short)atoi(szTemp);
				}
				//DcmTagKey(0x0018, 0x1200)
				eb = (*imageDataSet)->findAndGetString( DateOfLastCalibrationTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
					TimeConvert2(g_CTImgData->oHeader.siteField.LastCalib,lpTemp,1);
				//DcmTagKey(0x0018, 0x1201)
				eb = (*imageDataSet)->findAndGetString( TimeOfLastCalibrationTagKey, lpTemp, OFFalse );  
				if(eb == EC_Normal && lpTemp != NULL)
					TimeConvert2(g_CTImgData->oHeader.siteField.LastCalib,lpTemp,2);
				//DcmTagKey(0x0018, 0x5100)
				eb = (*imageDataSet)->findAndGetString( PatientPositionTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					if(strncmp(lpTemp,"HFS ",strlen(lpTemp)) == 0)
					{
						g_CTImgData->oHeader.studyField.PatEntry    = beHeadFirst;
						g_CTImgData->oHeader.studyField.PatPosition = bpSupine;
					}
					else if(strncmp(lpTemp,"HFP ",strlen(lpTemp)) == 0)
					{
						g_CTImgData->oHeader.studyField.PatEntry    = beHeadFirst;
						g_CTImgData->oHeader.studyField.PatPosition = bpProne;
					}
					else if(strncmp(lpTemp,"HFDR",strlen(lpTemp)) == 0)
					{
						g_CTImgData->oHeader.studyField.PatEntry    = beHeadFirst;
						g_CTImgData->oHeader.studyField.PatPosition = bpDecubitusRight;
					}
					else if(strncmp(lpTemp,"HFDL",strlen(lpTemp)) == 0)
					{
						g_CTImgData->oHeader.studyField.PatEntry    = beHeadFirst;
						g_CTImgData->oHeader.studyField.PatPosition = bpDecubitusLeft;
					}
					else if(strncmp(lpTemp,"FFS ",strlen(lpTemp)) == 0)
					{
						g_CTImgData->oHeader.studyField.PatEntry    = beFootFirst;
						g_CTImgData->oHeader.studyField.PatPosition = bpSupine;
					}
					else if(strncmp(lpTemp,"FFP ",strlen(lpTemp)) == 0)
					{
						g_CTImgData->oHeader.studyField.PatEntry    = beFootFirst;
						g_CTImgData->oHeader.studyField.PatPosition = bpProne;
					}
					else if(strncmp(lpTemp,"FFDR",strlen(lpTemp)) == 0)
					{
						g_CTImgData->oHeader.studyField.PatEntry    = beFootFirst;
						g_CTImgData->oHeader.studyField.PatPosition = bpDecubitusRight;
					}
					else if(strncmp(lpTemp,"FFDL",strlen(lpTemp)) == 0)
					{
						g_CTImgData->oHeader.studyField.PatEntry    = beFootFirst;
						g_CTImgData->oHeader.studyField.PatPosition = bpDecubitusLeft;
					}
				}
				//DcmTagKey(0x0020, 0x0010)
				eb = (*imageDataSet)->findAndGetString( StudyIDTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(g_CTImgData->oHeader.studyField.Id, lpTemp, strlen(lpTemp));
					if(*(lpTemp+strlen(lpTemp)-1) == ' ')
					{
						g_CTImgData->oHeader.studyField.Id[strlen(lpTemp)-1]=0x00;
					}
				}
				//DcmTagKey(0x0020, 0x0011)
				eb = (*imageDataSet)->findAndGetString( SeriesNumberTagKey, lpTemp, OFFalse );  
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp, lpTemp, strlen(lpTemp));
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					g_CTImgData->oHeader.seriesField.Id = (BYTE)strtoul(szTemp,NULL,10);
				}
				//DcmTagKey(0x0020, 0x0013)
				eb = (*imageDataSet)->findAndGetString( InstanceNumberTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp, lpTemp, strlen(lpTemp));
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					g_CTImgData->oHeader.imageField.Id = (unsigned short)strtoul(szTemp,NULL,10);
				}
				//DcmTagKey(0x0020, 0x0032)
				eb = (*imageDataSet)->findAndGetString( ImagePositionPatientTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
				{
					GetDSVM(lpTemp, strlen(lpTemp), dVal, nVM);
					if (nVM != 3)
					{
						return ;
					}
					dOffSet[0] = dVal[0] ;
					dOffSet[1] = dVal[1] ;
					dOffSet[2] = dVal[2] ;
				}
				//DcmTagKey(0x0020, 0x0037)
				eb = (*imageDataSet)->findAndGetString( ImageOrientationPatientTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					GetDSVM(lpTemp, strlen(lpTemp), dVal, nVM);
					if (nVM != 6)
					{
						return ;
					}
					else
					{
						dImgDirec[0] = dVal[0] ;
						dImgDirec[1] = dVal[1] ;
						dImgDirec[2] = dVal[2] ;
						dImgDirec[3] = dVal[3] ;
						dImgDirec[4] = dVal[4] ;
						dImgDirec[5] = dVal[5] ;
					}
				}
				//DcmTagKey(0x0020, 0x1002)
				eb = (*imageDataSet)->findAndGetString( ImagesInAcquisitionTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp, lpTemp, strlen(lpTemp));
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					g_CTImgData->oHeader.seriesField.ResolutionZ = (unsigned short)strtoul(szTemp,NULL,10);
				}
				//DcmTagKey(0x0020, 0x1041)
				eb = (*imageDataSet)->findAndGetString( SliceLocationTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp, lpTemp, strlen(lpTemp));
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					dSliceLocation = atof(szTemp);
					//g_CTImgData->oHeader.seriesField.ResolutionZ = (unsigned short)strtoul(szTemp,NULL,10);
				}
				//DcmTagKey(0x0028, 0x0010)
				Uint16 RowsValue;
				UINT16 ColumnsValue;
				eb = (*imageDataSet)->findAndGetUint16( RowsTagKey, RowsValue, OFFalse );
				if(eb == EC_Normal)
					g_CTImgData->oHeader.seriesField.ResolutionX = RowsValue;
				//DcmTagKey(0x0028, 0x0011)
				eb = (*imageDataSet)->findAndGetUint16( ColumnsTagKey, ColumnsValue, OFFalse );
				if(eb == EC_Normal)
					g_CTImgData->oHeader.seriesField.ResolutionY = ColumnsValue ;
				//DcmTagKey(0x0028, 0x0030)
				eb = (*imageDataSet)->findAndGetString( PixelSpacingTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
				{
					GetDSVM(lpTemp, strlen(lpTemp), dVal, nVM);
					if (nVM == 2)
					{
						g_CTImgData->oHeader.seriesField.FovX = 
							(unsigned short)((double)g_CTImgData->oHeader.seriesField.ResolutionX*dVal[0]) ;
						g_CTImgData->oHeader.seriesField.FovY = 
							(unsigned short)((double)g_CTImgData->oHeader.seriesField.ResolutionY*dVal[1]) ;
						CalcBIImgDirec(dImgDirec, dOffSet ,
							g_CTImgData->oHeader.studyField.PatPosition, 
							g_CTImgData->oHeader.studyField.PatEntry, 
							g_CTImgData->oHeader.seriesField.FovX,
							g_CTImgData->oHeader.seriesField.FovY,
							dXBImgDirec) ;
						g_CTImgData->oHeader.scanField.FreqDirX  = (float)dXBImgDirec[0] ;
						g_CTImgData->oHeader.scanField.FreqDirY  = (float)dXBImgDirec[1] ;
						g_CTImgData->oHeader.scanField.FreqDirZ  = (float)dXBImgDirec[2] ;
						g_CTImgData->oHeader.scanField.PhaseDirX = (float)dXBImgDirec[3] ;
						g_CTImgData->oHeader.scanField.PhaseDirY = (float)dXBImgDirec[4] ;
						g_CTImgData->oHeader.scanField.PhaseDirZ = (float)dXBImgDirec[5] ;
						g_CTImgData->oHeader.scanField.SliceDirX = (float)dXBImgDirec[6] ;
						g_CTImgData->oHeader.scanField.SliceDirY = (float)dXBImgDirec[7] ;
						g_CTImgData->oHeader.scanField.SliceDirZ = (float)dXBImgDirec[8] ;
						
						g_CTImgData->oHeader.imageField.fCenterPointX = (float)dXBImgDirec[9] ;
						g_CTImgData->oHeader.imageField.fCenterPointY = (float)dXBImgDirec[10] ;
						g_CTImgData->oHeader.imageField.fCenterPointZ = (float)dXBImgDirec[11] ;
						
						if(0/*m_bToshibaCTTable = FALSE*/)
						{
							g_CTImgData->oHeader.imageField.fCenterPointY = (float)dSliceLocation;
						}
					}
					else
					{
						return;
					}
				}
				//DcmTagKey(0x0028, 0x1050)
				eb = (*imageDataSet)->findAndGetString( WindowCenterTagKey, lpTemp, OFFalse ); 
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp, lpTemp, strlen(lpTemp));
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					/*CT图像窗位需要加上1024*/
					g_CTImgData->oHeader.imageField.WindowLevel = 1024 + (short)strtoul(szTemp,NULL,10);
				}
				//DcmTagKey(0x0028, 0x1051)
				eb = (*imageDataSet)->findAndGetString( WindowWidthTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp, lpTemp, strlen(lpTemp));
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					g_CTImgData->oHeader.imageField.WindowWidth = (short)strtoul(szTemp,NULL,10);
				}
				//DcmTagKey(0x0028, 0x1052)
				eb = (*imageDataSet)->findAndGetString( RescaleInterceptTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp, lpTemp, strlen(lpTemp));
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					nRescaleIntercept = (short)strtoul(szTemp,NULL,10);
				}
				//DcmTagKey(0x0028, 0x1053)
				eb = (*imageDataSet)->findAndGetString( RescaleSlopeTagKey, lpTemp, OFFalse );
				if(eb == EC_Normal && lpTemp != NULL)
				{
					strncpy(szTemp, lpTemp, strlen(lpTemp));
					if(szTemp[strlen(lpTemp) - 1] == ' ')
					{
						szTemp[strlen(lpTemp)-1] = 0x00 ;
					}
					else
					{
						szTemp[strlen(lpTemp)] = 0x00 ;
					}
					nRescaleSlope = (short)strtoul(szTemp,NULL,10);
				}
				
				const Uint16 *pixelData = NULL;
				unsigned long cnt = 0; 
				//DcmTagKey(0x7fe0, 0x0010)
				if ((*imageDataSet)->findAndGetUint16Array(PixelDataTagKey, pixelData, &cnt).good())
				{ 
					int nImgSize = 2*g_CTImgData->oHeader.seriesField.ResolutionX*g_CTImgData->oHeader.seriesField.ResolutionY ;
					nImageSize   = g_CTImgData->oHeader.seriesField.ResolutionX*g_CTImgData->oHeader.seriesField.ResolutionY ;
					
					g_CTImgData->pData = new WORD[nImageSize];
					if (g_CTImgData->pData != NULL)
					{
						memcpy(g_CTImgData->pData, pixelData, nImageSize*2);
						if(nRescaleIntercept!=-1024)
						{
							for(int ii=0;ii<nImageSize;ii++)
							{
								g_CTImgData->pData[ii]+=(1024+nRescaleIntercept);
							}
						}
					}
					else
					{
						return;
					}				
				} 
				
				pCBI->iModality = M_CT;
				pCBI->pDcmData = g_CTImgData;
				::PostMessage((HWND)pCBI->hwnd,WM_XBDICOMMSG,STORAGE_RECEIVE_CTSERIES,NULL);
			}else if ( nModality == M_SPECT || 
					   nModality == M_PET   ||
					   nModality == M_CR    ||
					   nModality == M_US    ||
					   nModality ==	M_UNKNOWN)
			{
				return;
			}
			
			pCBI->cbProc(pCBI);
			//  [9/7/2011 WUZIYUAN]

			// store file either with meta header or as pure dataset
/*			OFCondition cond = cbdata->dcmff->saveFile(fileName.c_str(), xfer, opt_sequenceType, opt_groupLength,
				opt_paddingType, OFstatic_cast(Uint32, opt_filepad),
				OFstatic_cast(Uint32, opt_itempad), !opt_useMetaheader);
			if (cond.bad())
			{
				fprintf(stderr, "storescp: Cannot write image file: %s\n", fileName.c_str());
				rsp->DimseStatus = STATUS_STORE_Refused_OutOfResources;
			}
			
			// check the image to make sure it is consistent, i.e. that its sopClass and sopInstance correspond
			// to those mentioned in the request. If not, set the status in the response message variable.
			if ((rsp->DimseStatus == STATUS_Success)&&(!opt_ignore))
			{
				// which SOP class and SOP instance ?
				if (! DU_findSOPClassAndInstanceInDataSet(*imageDataSet, sopClass, sopInstance, opt_correctUIDPadding))
				{
					fprintf(stderr, "storescp: Bad image file: %s\n", fileName.c_str());
					rsp->DimseStatus = STATUS_STORE_Error_CannotUnderstand;
				}
				else if (strcmp(sopClass, req->AffectedSOPClassUID) != 0)
				{
					rsp->DimseStatus = STATUS_STORE_Error_DataSetDoesNotMatchSOPClass;
				}
				else if (strcmp(sopInstance, req->AffectedSOPInstanceUID) != 0)
				{
					rsp->DimseStatus = STATUS_STORE_Error_DataSetDoesNotMatchSOPClass;
				}
			}
*/			
    }
	
    // in case opt_bitPreserving is set, do some other things
    if( opt_bitPreserving )
    {
		// we need to set outputFileNameArray and outputFileNameArrayCnt to be
		// able to perform the placeholder substitution in executeOnReception()
		char *tmpstr7 = strrchr( cbdata->imageFileName, PATH_SEPARATOR );
		outputFileNameArray.push_back(++tmpstr7);
    }
  }
  
  // return
  return;
}


static OFCondition storeSCP(FCBI* pCBI,
							T_ASC_Association *assoc,
							T_DIMSE_Message *msg,
							T_ASC_PresentationContextID presID)
							/*
							* This function processes a DIMSE C-STORE-RQ commmand that was
							* received over the network connection.
							*
							* Parameters:
							*   assoc  - [in] The association (network connection to another DICOM application).
							*   msg    - [in] The DIMSE C-STORE-RQ message that was received.
							*   presID - [in] The ID of the presentation context which was specified in the PDV which contained
							*                 the DIMSE command.
							*/
{
	OFCondition cond = EC_Normal;
	T_DIMSE_C_StoreRQ *req;
	char imageFileName[2048];
	
	// assign the actual information of the C-STORE-RQ command to a local variable
	req = &msg->msg.CStoreRQ;
	
	// if opt_ignore is set, the user requires that the data shall be received but not
	// stored. in this case, we want to create a corresponding temporary filename for
	// a file in which the data shall be stored temporarily. If this is not the case,
	// create a real filename (consisting of path and filename) for a real file.
	if (opt_ignore)
	{
#ifdef _WIN32
		tmpnam(imageFileName);
#else
		strcpy(imageFileName, NULL_DEVICE_NAME);
#endif
	}
	else
	{
		//3 possibilities: create unique filenames (fn), create timestamp fn, create fn from SOP Instance UIDs
		if (opt_uniqueFilenames)
		{
			// create unique filename by generating a temporary UID and using ".X." as an infix
			char buf[70];
			dcmGenerateUniqueIdentifier(buf);
			sprintf(imageFileName, "%s%c%s.X.%s%s", opt_outputDirectory.c_str(), PATH_SEPARATOR, dcmSOPClassUIDToModality(req->AffectedSOPClassUID), buf, opt_fileNameExtension.c_str());
		}
		else if (opt_timeNames)
		{
			// create a name for the new file. pattern: "[YYYYMMDDHHMMSSMMM]_[NUMBER].MODALITY[EXTENSION]" (use current datetime)
			// get the current time (needed for file name)
			OFDateTime dateTime;
			dateTime.setCurrentDateTime();
			// used to hold prospective filename
			char cmpFileName[2048];
			// next if/else block generates prospective filename, that is compared to last written filename
			if (timeNameCounter == -1)
			{
				// timeNameCounter not set -> last written filename has to be without "serial number"
				sprintf(cmpFileName, "%04u%02u%02u%02u%02u%02u%03u.%s%s",
					dateTime.getDate().getYear(), dateTime.getDate().getMonth(), dateTime.getDate().getDay(),
					dateTime.getTime().getHour(), dateTime.getTime().getMinute(), dateTime.getTime().getIntSecond(), dateTime.getTime().getMilliSecond(),
					dcmSOPClassUIDToModality(req->AffectedSOPClassUID), opt_fileNameExtension.c_str());
			}
			else
			{
				// counter was active before, so generate filename with "serial number" for comparison
				sprintf(cmpFileName, "%04u%02u%02u%02u%02u%02u%03u_%04u.%s%s", //millisecond version
					dateTime.getDate().getYear(), dateTime.getDate().getMonth(), dateTime.getDate().getDay(),
					dateTime.getTime().getHour(), dateTime.getTime().getMinute(), dateTime.getTime().getIntSecond(), dateTime.getTime().getMilliSecond(),
					timeNameCounter, dcmSOPClassUIDToModality(req->AffectedSOPClassUID), opt_fileNameExtension.c_str());
			}
			if ( (outputFileNameArray.size()!=0) && (outputFileNameArray.back() == cmpFileName) )
			{
				// if this is not the first run and the prospective filename is equal to the last written filename
				// generate one with a serial number (incremented by 1)
				timeNameCounter++;
				sprintf(imageFileName, "%s%c%04u%02u%02u%02u%02u%02u%03u_%04u.%s%s", opt_outputDirectory.c_str(), PATH_SEPARATOR, //millisecond version
					dateTime.getDate().getYear(), dateTime.getDate().getMonth(), dateTime.getDate().getDay(),
					dateTime.getTime().getHour(), dateTime.getTime().getMinute(), dateTime.getTime().getIntSecond(), dateTime.getTime().getMilliSecond(),
					timeNameCounter, dcmSOPClassUIDToModality(req->AffectedSOPClassUID), opt_fileNameExtension.c_str());
			}
			else
			{
				//first run or filenames are different: create filename without serial number
				sprintf(imageFileName, "%s%c%04u%02u%02u%02u%02u%02u%03u.%s%s", opt_outputDirectory.c_str(), PATH_SEPARATOR, //millisecond version
					dateTime.getDate().getYear(), dateTime.getDate().getMonth(), dateTime.getDate().getDay(),
					dateTime.getTime().getHour(), dateTime.getTime().getMinute(),dateTime.getTime().getIntSecond(), dateTime.getTime().getMilliSecond(),
					dcmSOPClassUIDToModality(req->AffectedSOPClassUID), opt_fileNameExtension.c_str());
				// reset counter, because timestamp and therefore filename has changed
				timeNameCounter = -1;
			}
		}
		else
		{
			// don't create new UID, use the study instance UID as found in object
			sprintf(imageFileName, "%s%c%s.%s%s", opt_outputDirectory.c_str(), PATH_SEPARATOR, dcmSOPClassUIDToModality(req->AffectedSOPClassUID), req->AffectedSOPInstanceUID, opt_fileNameExtension.c_str());
		}
	}
	
	// dump some information if required
	if (opt_verbose)
	{
		printf("Received ");
		DIMSE_printCStoreRQ(stdout, req);
	}
	
	// intialize some variables
	StoreCallbackData callbackData;
	callbackData.assoc = assoc;
	callbackData.imageFileName = imageFileName;
	DcmFileFormat dcmff;
	callbackData.dcmff = &dcmff;
	
	// define an address where the information which will be received over the network will be stored
	DcmDataset *dset = dcmff.getDataset();
	
	// if opt_bitPreserving is set, the user requires that the data shall be
	// written exactly as it was received. Depending on this option, function
	// DIMSE_storeProvider must be called with certain parameters.
	if (opt_bitPreserving)
	{
		cond = DIMSE_storeProvider(pCBI,assoc, presID, req, imageFileName, opt_useMetaheader, NULL,
			storeSCPCallback, &callbackData, opt_blockMode, opt_dimse_timeout);
	}
	else
	{
		cond = DIMSE_storeProvider(pCBI,assoc, presID, req, NULL, opt_useMetaheader, &dset,
			storeSCPCallback, &callbackData, opt_blockMode, opt_dimse_timeout);
	}
	
	// if some error occured, dump corresponding information and remove the outfile if necessary
	if (cond.bad())
	{
		fprintf(stderr, "storescp: Store SCP Failed:\n");
		DimseCondition::dump(cond);
		// remove file
		if (!opt_ignore)
		{
			if (strcmp(imageFileName, NULL_DEVICE_NAME) != 0) unlink(imageFileName);
		}
	}
#ifdef _WIN32
	else if (opt_ignore)
	{
		if (strcmp(imageFileName, NULL_DEVICE_NAME) != 0) unlink(imageFileName); // delete the temporary file
	}
#endif
	
	// if everything was successful so far and option --exec-on-reception is set,
	// we want to execute a certain command which was passed to the application
	if( cond.good() && opt_execOnReception != NULL )
		executeOnReception();
	
	// if everything was successful so far, go ahead and handle possible end-of-study events
	if( cond.good() )
		executeEndOfStudyEvents();
	
	// if opt_sleepAfter is set, the user requires that the application shall
	// sleep a certain amount of seconds after storing the instance data.
	if (opt_sleepAfter > 0)
	{
		OFStandard::sleep(OFstatic_cast(unsigned int, opt_sleepAfter));
	}
	
	// return return value
	return cond;
}



static void executeEndOfStudyEvents()
/*
* This function deals with the execution of end-of-study-events. In detail,
* events that need to take place are sepcified by the user through certain
* command line options. The options that define these end-of-study-events
* are "--rename-on-eostudy" and "--exec-on-eostudy".
*
* Parameters:
*   none.
*/
{
	// if option --rename-on-eostudy is set and variable lastStudySubdirectoryPathAndName
	// does not equal NULL (i.e. we received all objects that belong to one study, or - in
	// other words - it is the end of one study) we want to rename the output files that
	// belong to the last study. (Note that these files are captured in outputFileNameArray)
	if( opt_renameOnEndOfStudy && ! lastStudySubdirectoryPathAndName.empty() )
		renameOnEndOfStudy();
	
	// if option --exec-on-eostudy is set and variable lastStudySubdirectoryPathAndName does
	// not equal NULL (i.e. we received all objects that belong to one study, or - in other
	// words - it is the end of one study) we want to execute a certain command which was
	// passed to the application
	if( opt_execOnEndOfStudy != NULL && ! lastStudySubdirectoryPathAndName.empty() )
		executeOnEndOfStudy();
	
	lastStudySubdirectoryPathAndName.clear();
}


static void executeOnReception()
/*
* This function deals with the execution of the command line which was passed
* to option --exec-on-reception of the storescp. This command line is captured
* in opt_execOnReception. Note that the command line can contain the placeholders
* PATH_PLACEHOLDER and FILENAME_PLACEHOLDER which need to be substituted before the command line is actually
* executed. PATH_PLACEHOLDER will be substituted by the path to the output directory into which
* the last file was written; FILENAME_PLACEHOLDER will be substituted by the filename of the last
* file which was written.
*
* Parameters:
*   none.
*/
{
	OFString cmd = opt_execOnReception;
	
	// in case a file was actually written
	if( !opt_ignore )
	{
		// perform substitution for placeholder #p; note that
		//  - in case option --sort-conc-studies is set, #p will be substituted by subdirectoryPathAndName
		//  - and in case option --sort-conc-studies is not set, #p will be substituted by opt_outputDirectory
		OFString dir = (opt_sortConcerningStudies == NULL) ? OFString(opt_outputDirectory) : subdirectoryPathAndName;
		cmd = replaceChars( cmd, OFString(PATH_PLACEHOLDER), dir );
		
		// perform substitution for placeholder #f; note that outputFileNameArray.back()
		// always contains the name of the file (without path) which was written last.
		OFString outputFileName = outputFileNameArray.back();
		cmd = replaceChars( cmd, OFString(FILENAME_PLACEHOLDER), outputFileName );
	}
	
	// perform substitution for placeholder #a
	cmd = replaceChars( cmd, OFString(CALLING_AETITLE_PLACEHOLDER), callingaetitle );
	
	// perform substitution for placeholder #c
	cmd = replaceChars( cmd, OFString(CALLED_AETITLE_PLACEHOLDER), calledaetitle );
	
	// Execute command in a new process
	executeCommand( cmd );
}



static void renameOnEndOfStudy()
/*
* This function deals with renaming the last study's output files. In detail, these file's
* current filenames will be changed to a filename that corresponds to the pattern [modality-
* prefix][consecutive-numbering]. The current filenames of all files that belong to the study
* are captured in outputFileNameArray. The new filenames will be calculated whithin this
* function: The [modality-prefix] will be taken from the old filename,
* [consecutive-numbering] is a consecutively numbered, 6 digit number which will be calculated
* starting from 000001.
*
* Parameters:
*   none.
*/
{
	int counter = 1;
	
	OFListIterator(OFString) first = outputFileNameArray.begin();
	OFListIterator(OFString) last = outputFileNameArray.end();
	
	// before we deal with all the filenames which are included in the array, we need to distinguish
	// two different cases: If endOfStudyThroughTimeoutEvent is not true, the last filename in the array
	// refers to a file that belongs to a new study of which the first object was just received. (In this
	// case there are at least two filenames in the array). Then, this last filename is - at the end of the
	// following loop - not supposed to be deleted from the array. If endOfStudyThroughTimeoutEvent is true,
	// all filenames that are captured in the array, refer to files that belong to the same study. Hence,
	// all of these files shall be renamed and all of the filenames within the array shall be deleted.
	if( ! endOfStudyThroughTimeoutEvent ) --last;
	
	// rename all files that belong to the last study
	while (first != last)
	{
		// determine the new file name: The first two characters of the old file name make up the [modality-prefix].
		// The value for [consecutive-numbering] will be determined using the counter variable.
		char modalityId[3];
		char newFileName[9];
		if (opt_timeNames)
		{
			// modality prefix are the first 2 characters after serial number (if present)
			size_t serialPos = (*first).find("_");
			if (serialPos != OFString_npos)
			{
				//serial present: copy modality prefix (skip serial: 1 digit "_" + 4 digits serial + 1 digit ".")
				OFStandard::strlcpy( modalityId, (*first).substr(serialPos+6, 2).c_str(), 3 );
			}
			else
			{
				//serial not present, copy starts directly after first "." (skip 17 for timestamp, one for ".")
				OFStandard::strlcpy( modalityId, (*first).substr(18, 2).c_str(), 3 );
			}
		}
		else
		{
			OFStandard::strlcpy( modalityId, (*first).c_str(), 3 );
		}
		sprintf( newFileName, "%s%06d", modalityId, counter );
		
		// create two strings containing path and file name for
		// the current filename and the future filename
		OFString oldPathAndFileName;
		oldPathAndFileName = lastStudySubdirectoryPathAndName;
		oldPathAndFileName += PATH_SEPARATOR;
		oldPathAndFileName += *first;
		
		OFString newPathAndFileName;
		newPathAndFileName = lastStudySubdirectoryPathAndName;
		newPathAndFileName += PATH_SEPARATOR;
		newPathAndFileName += newFileName;
		
		// rename file
		if( rename( oldPathAndFileName.c_str(), newPathAndFileName.c_str() ) != 0 )
			fprintf( stderr, "storescp: Cannot rename file '%s' to '%s'.\n", oldPathAndFileName.c_str(), newPathAndFileName.c_str() );
		
		// remove entry from list
		first = outputFileNameArray.erase(first);
		
		// increase counter
		counter++;
	}
}



static void executeOnEndOfStudy()
/*
* This function deals with the execution of the command line which was passed
* to option --exec-on-eostudy of the storescp. This command line is captured
* in opt_execOnEndOfStudy. Note that the command line can contain the placeholder
* PATH_PLACEHOLDER which needs to be substituted before the command line is actually executed.
* In detail, PATH_PLACEHOLDER will be substituted by the path to the output directory into which
* the files of the last study were written.
*
* Parameters:
*   none.
*/
{
	OFString cmd = opt_execOnEndOfStudy;
	
	// perform substitution for placeholder #p; #p will be substituted by lastStudySubdirectoryPathAndName
	cmd = replaceChars( cmd, OFString(PATH_PLACEHOLDER), lastStudySubdirectoryPathAndName );
	
	// perform substitution for placeholder #a
	cmd = replaceChars( cmd, OFString(CALLING_AETITLE_PLACEHOLDER), callingaetitle );
	
	// perform substitution for placeholder #c
	cmd = replaceChars( cmd, OFString(CALLED_AETITLE_PLACEHOLDER), calledaetitle );
	
	// Execute command in a new process
	executeCommand( cmd );
}



static OFString replaceChars( const OFString &srcstr, const OFString &pattern, const OFString &substitute )
/*
* This function replaces all occurrences of pattern in srcstr with substitute and returns
* the result as a new OFString variable. Note that srcstr itself will not be changed,
*
* Parameters:
*   srcstr     - [in] The source string.
*   pattern    - [in] The pattern string which shall be substituted.
*   substitute - [in] The substitute for pattern in srcstr.
*/
{
	OFString result = srcstr;
	size_t pos = 0;
	
	while( pos != OFString_npos )
	{
		pos = result.find( pattern, pos );
		
		if( pos != OFString_npos )
		{
			result.replace( pos, pattern.size(), substitute );
			pos += substitute.size();
		}
	}
	
	return( result );
}



static void executeCommand( const OFString &cmd )
/*
* This function executes the given command line. The execution will be
* performed in a new process which can be run in the background
* so that it does not slow down the execution of storescp.
*
* Parameters:
*   cmd - [in] The command which shall be executed.
*/
{
#ifdef HAVE_FORK
	pid_t pid = fork();
	if( pid < 0 )     // in case fork failed, dump an error message
		fprintf( stderr, "storescp: Cannot execute command '%s' (fork failed).\n", cmd.c_str() );
	else if (pid > 0)
	{
		/* we are the parent process */
		/* remove pending zombie child processes */
		cleanChildren(pid, OFTrue);
	}
	else // in case we are the child process, execute the command etc.
	{
		// execute command through execl will terminate the child process.
		// Since we only have a single command string and not a list of arguments,
		// we 'emulate' a call to system() by passing the command to /bin/sh
		// which hopefully exists on all Posix systems.
		
		if (execl( "/bin/sh", "/bin/sh", "-c", cmd.c_str(), NULL ) < 0)
			fprintf( stderr, "storescp: Cannot execute /bin/sh.\n" );
		
		// if execl succeeds, this part will not get executed.
		// if execl fails, there is not much we can do except bailing out.
		abort();
	}
#else
	PROCESS_INFORMATION procinfo;
	STARTUPINFO sinfo;
	OFBitmanipTemplate<char>::zeroMem((char *)&sinfo, sizeof(sinfo));
	sinfo.cb = sizeof(sinfo);
	
	// execute command (Attention: Do not pass DETACHED_PROCESS as sixth argument to the below
	// called function because in such a case the execution of batch-files is not going to work.)
	if( !CreateProcess(NULL, OFconst_cast(char *, cmd.c_str()), NULL, NULL, 0, 0, NULL, NULL, &sinfo, &procinfo) )
		fprintf( stderr, "storescp: Error while executing command '%s'.\n" , cmd.c_str() );
	
	if (opt_execSync)
	{
		// Wait until child process exits (makes execution synchronous).
		WaitForSingleObject(procinfo.hProcess, INFINITE);
	}
	
	// Close process and thread handles to avoid resource leak
	CloseHandle(procinfo.hProcess);
	CloseHandle(procinfo.hThread);
#endif
}

static void cleanChildren(pid_t pid, OFBool synch)
/*
* This function removes child processes that have terminated,
* i.e. converted to zombies. Should be called now and then.
*/
{
#ifdef HAVE_WAITPID
    int stat_loc;
#elif HAVE_WAIT3
    struct rusage rusage;
#if defined(__NeXT__)
    /* some systems need a union wait as argument to wait3 */
    union wait status;
#else
    int        status;
#endif
#endif
	
#if defined(HAVE_WAITPID) || defined(HAVE_WAIT3)
    int child = 1;
    int options = synch ? 0 : WNOHANG;
    while (child > 0)
    {
#ifdef HAVE_WAITPID
        child = OFstatic_cast(int, waitpid(pid, &stat_loc, options));
#elif defined(HAVE_WAIT3)
        child = wait3(&status, options, &rusage);
#endif
        if (child < 0)
        {
			if (errno != ECHILD) CERR << "wait for child failed: " << strerror(errno) << endl;
        }
		
        if (synch) child = -1; // break out of loop
    }
#endif
}

static
DUL_PRESENTATIONCONTEXT *
findPresentationContextID(LST_HEAD * head,
                          T_ASC_PresentationContextID presentationContextID)
{
    DUL_PRESENTATIONCONTEXT *pc;
    LST_HEAD **l;
    OFBool found = OFFalse;
	
    if (head == NULL)
        return NULL;
	
    l = &head;
    if (*l == NULL)
        return NULL;
	
    pc = OFstatic_cast(DUL_PRESENTATIONCONTEXT *, LST_Head(l));
    (void)LST_Position(l, OFstatic_cast(LST_NODE *, pc));
	
    while (pc && !found) {
        if (pc->presentationContextID == presentationContextID) {
            found = OFTrue;
        } else {
            pc = OFstatic_cast(DUL_PRESENTATIONCONTEXT *, LST_Next(l));
        }
    }
    return pc;
}

/** accept all presenstation contexts for unknown SOP classes,
*  i.e. UIDs appearing in the list of abstract syntaxes
*  where no corresponding name is defined in the UID dictionary.
*  @param params pointer to association parameters structure
*  @param transferSyntax transfer syntax to accept
*  @param acceptedRole SCU/SCP role to accept
*/
static OFCondition acceptUnknownContextsWithTransferSyntax(
														   T_ASC_Parameters * params,
														   const char* transferSyntax,
														   T_ASC_SC_ROLE acceptedRole)
{
    OFCondition cond = EC_Normal;
    int n, i, k;
    DUL_PRESENTATIONCONTEXT *dpc;
    T_ASC_PresentationContext pc;
    OFBool accepted = OFFalse;
    OFBool abstractOK = OFFalse;
	
    n = ASC_countPresentationContexts(params);
    for (i = 0; i < n; i++)
    {
        cond = ASC_getPresentationContext(params, i, &pc);
        if (cond.bad()) return cond;
        abstractOK = OFFalse;
        accepted = OFFalse;
		
        if (dcmFindNameOfUID(pc.abstractSyntax) == NULL)
        {
            abstractOK = OFTrue;
			
            /* check the transfer syntax */
            for (k = 0; (k < OFstatic_cast(int, pc.transferSyntaxCount)) && !accepted; k++)
            {
                if (strcmp(pc.proposedTransferSyntaxes[k], transferSyntax) == 0)
                {
                    accepted = OFTrue;
                }
            }
        }
		
        if (accepted)
        {
            cond = ASC_acceptPresentationContext(
                params, pc.presentationContextID,
                transferSyntax, acceptedRole);
            if (cond.bad()) return cond;
        } else {
            T_ASC_P_ResultReason reason;
			
            /* do not refuse if already accepted */
            dpc = findPresentationContextID(
				params->DULparams.acceptedPresentationContext,
				pc.presentationContextID);
            if ((dpc == NULL) ||
                ((dpc != NULL) && (dpc->result != ASC_P_ACCEPTANCE)))
            {
				
                if (abstractOK) {
                    reason = ASC_P_TRANSFERSYNTAXESNOTSUPPORTED;
                } else {
                    reason = ASC_P_ABSTRACTSYNTAXNOTSUPPORTED;
                }
                /*
				* If previously this presentation context was refused
				* because of bad transfer syntax let it stay that way.
				*/
                if ((dpc != NULL) &&
                    (dpc->result == ASC_P_TRANSFERSYNTAXESNOTSUPPORTED))
                    reason = ASC_P_TRANSFERSYNTAXESNOTSUPPORTED;
				
                cond = ASC_refusePresentationContext(params,
					pc.presentationContextID,
					reason);
                if (cond.bad()) return cond;
            }
        }
    }
    return EC_Normal;
}


/** accept all presenstation contexts for unknown SOP classes,
*  i.e. UIDs appearing in the list of abstract syntaxes
*  where no corresponding name is defined in the UID dictionary.
*  This method is passed a list of "preferred" transfer syntaxes.
*  @param params pointer to association parameters structure
*  @param transferSyntax transfer syntax to accept
*  @param acceptedRole SCU/SCP role to accept
*/
static OFCondition acceptUnknownContextsWithPreferredTransferSyntaxes(
																	  T_ASC_Parameters * params,
																	  const char* transferSyntaxes[], int transferSyntaxCount,
																	  T_ASC_SC_ROLE acceptedRole)
{
    OFCondition cond = EC_Normal;
    /*
    ** Accept in the order "least wanted" to "most wanted" transfer
    ** syntax.  Accepting a transfer syntax will override previously
    ** accepted transfer syntaxes.
    */
    for (int i=transferSyntaxCount-1; i>=0; i--)
    {
        cond = acceptUnknownContextsWithTransferSyntax(params, transferSyntaxes[i], acceptedRole);
        if (cond.bad()) return cond;
    }
    return cond;
}

#ifdef HAVE_CONFIG_H

static int makeTempFile()
{
    char tempfile[30];
    OFStandard::strlcpy(tempfile, "/tmp/storescp_XXXXXX", 30);
#ifdef HAVE_MKSTEMP
    return mkstemp(tempfile);
#else /* ! HAVE_MKSTEMP */
    mktemp(tempfile);
    return open(tempfile, O_WRONLY|O_CREAT|O_APPEND,0644);
#endif
}

#endif

//  [8/30/2011 Administrator]
static void TimeConvert2(MR_BIDateTime &datetime,const char *p,int flag)
{
	if(flag==2)//TM
	{
		char hour[2],min[2],sec[2];
		strncpy(hour,p,2);
		datetime.hour=atoi(hour);
		strncpy(min,p+2,2);
		datetime.minute=atoi(min);
		strncpy(sec,p+4,2);
		datetime.second=atoi(sec);
	}
	else if(flag==1)//DA
	{
		char year[4],mon[2],day[2];
		strncpy(year,p,4);
		datetime.year=atoi(year);
		strncpy(mon,p+4,2);
		datetime.month=atoi(mon);
		strncpy(day,p+6,2);
		datetime.day=atoi(day);
	}
}

static void TimeConvert2(CT_BIDateTime &datetime,const char *p,int flag)
{
	if(flag==2)//TM
	{
		char hour[2],min[2],sec[2];
		strncpy(hour,p,2);
		datetime.hour=atoi(hour);
		strncpy(min,p+2,2);
		datetime.minute=atoi(min);
		strncpy(sec,p+4,2);
		datetime.second=atoi(sec);
	}
	else if(flag==1)//DA
	{
		char year[4],mon[2],day[2];
		strncpy(year,p,4);
		datetime.year=atoi(year);
		strncpy(mon,p+4,2);
		datetime.month=atoi(mon);
		strncpy(day,p+6,2);
		datetime.day=atoi(day);
	}
}

static void NameConvert2(char BIname[], char DIname[], int vlength)
{
	int i=0;
	while(i<vlength&&i<31)
	{
		while(DIname[i]!='^'&&i<vlength&&i<31)
		{
			BIname[i]=DIname[i];
			i++;
		}
		if(i<vlength&&i<32)
		{
			BIname[i]=' ';
			i++;
		}
	}
	BIname[i]=0;
	if(BIname[vlength-1]==' ')
		BIname[vlength-1]=0x00;		
}

static unsigned char AgeConvert2(char DIage[])
{
	char age[3];
	strncpy(age,DIage,3);
	return atoi(age);
}

static int GetSeriesOption(char szSeriesName[])
{
	int nSeriesOption = 0 ;
	if (strncmp(szSeriesName, "SE", 2) == 0)
	{
		nSeriesOption = cnstSE ;
	}
	else if (strncmp(szSeriesName, "SEDWI", 5) == 0)
	{
		nSeriesOption = cnstSEDWI ;
	}
	else if (strncmp(szSeriesName, "IRSE", 4) == 0)
	{
		nSeriesOption = cnstIRSE ;
	}
	else if (strncmp(szSeriesName, "SE\\IR", 5) == 0)
	{
		nSeriesOption = cnstSTIR ;
	}
	else if (strncmp(szSeriesName, "IR", 2) == 0)
	{
		nSeriesOption = cnstFLAIR ;
	}
	else if (strncmp(szSeriesName, "GR", 2) == 0)
	{
		nSeriesOption = cnstGRE ;
	}
	else if (strncmp(szSeriesName, "LSDI", 4) == 0)
	{
		nSeriesOption = cnstLSDI ;
	}
	else
	{
		nSeriesOption = cnstTOF3D ;
	}
	return nSeriesOption ;
}

static void GetDSVM(char* lpTemp, int nVLength, double dVal[], int& nVM)
{
	char   *pszTemp ; 
	CString csT1, csT2 ;
	int     nPos = 0;
	int     nLen ;
	
	pszTemp = new char[nVLength + 1] ;
	memcpy(pszTemp, lpTemp, nVLength);
	nVM = 0;
	if (pszTemp[nVLength-1] == ' ')
	{
		pszTemp[nVLength-1] = 0 ;
	}
	else
	{
		pszTemp[nVLength] = 0 ;
	}
	
	csT1.Format("%s",pszTemp);
	nLen = csT1.GetLength();
	nPos = csT1.Find("\\" ,0);
	while (nPos > 0)
	{
		csT2 = csT1.Left(nPos);
		dVal[nVM] = atof(csT2.GetBuffer(30));
		csT1 = csT1.Right(nLen - nPos -1);
		nLen = csT1.GetLength();
		nPos = csT1.Find("\\" ,0);
		nVM++;
	}
	dVal[nVM++] = atof(csT1.GetBuffer(30));
	delete[] pszTemp ;
}

static void CalcBIImgDirec(double dDirCos[6], double dTopLeft[3] ,
									 BYTE iPatPos, BYTE iPatEntry,
									 unsigned short sFovX, unsigned short sFovY,
									 double dBIDirec[])
{
	double  dImgCenterX , dImgCenterY , dImgCenterZ ;
	dImgCenterX = dTopLeft[0] + ((double)sFovX*dDirCos[0] + (double)sFovY*dDirCos[3])/2.0 ;
	dImgCenterY = dTopLeft[1] + ((double)sFovX*dDirCos[1] + (double)sFovY*dDirCos[4])/2.0 ;
	dImgCenterZ = dTopLeft[2] + ((double)sFovX*dDirCos[2] + (double)sFovY*dDirCos[5])/2.0 ;

	if (iPatEntry == beHeadFirst)
	{
		switch (iPatPos)
		{
		case bpProne:
			dBIDirec[0] = -dDirCos[0] ;   //Read-x
			dBIDirec[1] = dDirCos[2] ;    //Read-y
			dBIDirec[2] = dDirCos[1] ;    //Read-z
			dBIDirec[3] = -dDirCos[3] ;   //phase-x
			dBIDirec[4] = dDirCos[5] ;    //phase-y
			dBIDirec[5] = dDirCos[4] ;    //phase-z

			dBIDirec[9]  = -dImgCenterX ;
			dBIDirec[10] =  dImgCenterZ ;
			dBIDirec[11] =  dImgCenterY ;
			break ;
		case bpDecubitusRight:
			dBIDirec[0] = dDirCos[1] ;
			dBIDirec[1] = dDirCos[2] ;
			dBIDirec[2] = dDirCos[0] ;
			dBIDirec[3] = dDirCos[4] ;
			dBIDirec[4] = dDirCos[5] ;
			dBIDirec[5] = dDirCos[3] ;

			dBIDirec[9]  = dImgCenterY ;
			dBIDirec[10] = dImgCenterZ ;
			dBIDirec[11] = dImgCenterX ;
			break ;
		case bpDecubitusLeft:
			dBIDirec[0] = -dDirCos[1] ;
			dBIDirec[1] = dDirCos[2] ;
			dBIDirec[2] = -dDirCos[0] ;
			dBIDirec[3] = -dDirCos[4] ;
			dBIDirec[4] = dDirCos[5] ;
			dBIDirec[5] = -dDirCos[3] ;

			dBIDirec[9]  = -dImgCenterY ;
			dBIDirec[10] =  dImgCenterZ ;
			dBIDirec[11] = -dImgCenterX ;
			break ;
		case bpSupine:
		default:
			dBIDirec[0] = dDirCos[0] ;
			dBIDirec[1] = dDirCos[2] ;
			dBIDirec[2] = -dDirCos[1] ;
			dBIDirec[3] = dDirCos[3] ;
			dBIDirec[4] = dDirCos[5] ;
			dBIDirec[5] = -dDirCos[4] ;

			dBIDirec[9]  =  dImgCenterX ;
			dBIDirec[10] =  dImgCenterZ ;
			dBIDirec[11] = -dImgCenterY ;
			break ;
		}
	}
	else
	{
		switch (iPatPos)
		{
		case bpProne:
			dBIDirec[0] =  dDirCos[0] ;     //Read-x
			dBIDirec[1] = -dDirCos[2] ;     //Read-y
			dBIDirec[2] =  dDirCos[1] ;     //Read-z
			dBIDirec[3] =  dDirCos[3] ;     //phase-x
			dBIDirec[4] = -dDirCos[5] ;     //phase-y
			dBIDirec[5] =  dDirCos[4] ;     //phase-z

			dBIDirec[9]  =  dImgCenterX ;
			dBIDirec[10] = -dImgCenterZ ;
			dBIDirec[11] =  dImgCenterY ;
			break ;
		case bpDecubitusRight:
			dBIDirec[0] = -dDirCos[1] ;
			dBIDirec[1] = -dDirCos[2] ;
			dBIDirec[2] =  dDirCos[0] ;
			dBIDirec[3] = -dDirCos[4] ;
			dBIDirec[4] = -dDirCos[5] ;
			dBIDirec[5] =  dDirCos[3] ;

			dBIDirec[9]  = -dImgCenterY ;
			dBIDirec[10] = -dImgCenterZ ;
			dBIDirec[11] =  dImgCenterX ;
			break ;
		case bpDecubitusLeft:
			dBIDirec[0] =  dDirCos[1] ;
			dBIDirec[1] = -dDirCos[2] ;
			dBIDirec[2] = -dDirCos[0] ;
			dBIDirec[3] =  dDirCos[4] ;
			dBIDirec[4] = -dDirCos[5] ;
			dBIDirec[5] = -dDirCos[3] ;

			dBIDirec[9]  =  dImgCenterY ;
			dBIDirec[10] = -dImgCenterZ ;
			dBIDirec[11] = -dImgCenterX ;
			break ;
		case bpSupine:
		default:
			dBIDirec[0] = -dDirCos[0] ;
			dBIDirec[1] = -dDirCos[2] ;
			dBIDirec[2] = -dDirCos[1] ;
			dBIDirec[3] = -dDirCos[3] ;
			dBIDirec[4] = -dDirCos[5] ;
			dBIDirec[5] = -dDirCos[4] ;

			dBIDirec[9]  = -dImgCenterX ;
			dBIDirec[10] = -dImgCenterZ ;
			dBIDirec[11] = -dImgCenterY ;
			break ;
		}
	}
    dBIDirec[6] = dBIDirec[1]*dBIDirec[5] - dBIDirec[2]*dBIDirec[4] ;    //slice-x
	dBIDirec[7] = dBIDirec[2]*dBIDirec[3] - dBIDirec[0]*dBIDirec[5] ;    //slice-y
	dBIDirec[8] = dBIDirec[0]*dBIDirec[4] - dBIDirec[1]*dBIDirec[3] ;    //slice-z
}
//  [8/30/2011 Administrator]
#endif