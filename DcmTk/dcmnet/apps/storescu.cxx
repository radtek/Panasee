/*
*
*  Copyright (C) 1994-2005, OFFIS
*
*  This software and supporting documentation were developed by
*
*    Kuratorium OFFIS e.V.
*    Healthcare Information and Communication Systems
*    Escherweg 2
*    D-26121 Oldenburg, Germany
*
*  THIS SOFTWARE IS MADE AVAILABLE,  AS IS,  AND OFFIS MAKES NO  WARRANTY
*  REGARDING  THE  SOFTWARE,  ITS  PERFORMANCE,  ITS  MERCHANTABILITY  OR
*  FITNESS FOR ANY PARTICULAR USE, FREEDOM FROM ANY COMPUTER DISEASES  OR
*  ITS CONFORMITY TO ANY SPECIFICATION. THE ENTIRE RISK AS TO QUALITY AND
*  PERFORMANCE OF THE SOFTWARE IS WITH THE USER.
*
*  Module:  dcmnet
*
*  Author:  Andrew Hewett
*
*  Purpose: Storage Service Class User (C-STORE operation)
*
*  Last Update:      $Author: meichel $
*  Update Date:      $Date: 2005/12/08 15:44:22 $
*  Source File:      $Source: /share/dicom/cvs-depot/dcmtk/dcmnet/apps/storescu.cc,v $
*  CVS/RCS Revision: $Revision: 1.64 $
*  Status:           $State: Exp $
*
*  CVS/RCS Log at end of file
*
*/

#include "dcmtk/config/osconfig.h" /* make sure OS specific configuration is included first */

#define INCLUDE_CSTDLIB
#define INCLUDE_CSTDIO
#define INCLUDE_CSTRING
#define INCLUDE_CERRNO
#define INCLUDE_CSTDARG
#define INCLUDE_CCTYPE
#include "dcmtk/ofstd/ofstdinc.h"

BEGIN_EXTERN_C
#ifdef HAVE_SYS_FILE_H
#include <sys/file.h>
#endif
END_EXTERN_C

#ifdef HAVE_GUSI_H
#include <GUSI.h>
#endif

#include "dcmtk/ofstd/ofstring.h"
#include "dcmtk/dcmnet/dimse.h"
#include "dcmtk/dcmnet/diutil.h"
#include "dcmtk/dcmdata/dcdatset.h"
#include "dcmtk/dcmdata/dcmetinf.h"
#include "dcmtk/dcmdata/dcfilefo.h"
#include "dcmtk/dcmdata/dcdebug.h"
#include "dcmtk/dcmdata/dcuid.h"
#include "dcmtk/dcmdata/dcdict.h"
#include "dcmtk/dcmdata/dcdeftag.h"
#include "dcmtk/dcmdata/cmdlnarg.h"
#include "dcmtk/ofstd/ofconapp.h"
#include "dcmtk/dcmdata/dcuid.h"     /* for dcmtk version name */
#include "dcmtk/dcmnet/dicom.h"     /* for DICOM_APPLICATION_REQUESTOR */
#include "dcmtk/dcmdata/dcostrmz.h"  /* for dcmZlibCompressionLevel */
#include "dcmtk/dcmnet/dcasccfg.h"  /* for class DcmAssociationConfiguration */
#include "dcmtk/dcmnet/dcasccff.h"  /* for class DcmAssociationConfigurationFile */

#ifdef ON_THE_FLY_COMPRESSION
#include "dcmtk/dcmjpeg/djdecode.h"  /* for dcmjpeg decoders */
#include "dcmtk/dcmjpeg/djencode.h"  /* for dcmjpeg encoders */
#include "dcmtk/dcmdata/dcrledrg.h"  /* for DcmRLEDecoderRegistration */
#include "dcmtk/dcmdata/dcrleerg.h"  /* for DcmRLEEncoderRegistration */
#endif

#ifdef WITH_OPENSSL
#include "dcmtk/dcmtls/tlstrans.h"
#include "dcmtk/dcmtls/tlslayer.h"
#endif

#ifdef WITH_ZLIB
#include <zlib.h>          /* for zlibVersion() */
#endif

#define OFFIS_CONSOLE_APPLICATION "storescu"

static char rcsid[] = "$dcmtk: " OFFIS_CONSOLE_APPLICATION " v"
OFFIS_DCMTK_VERSION " " OFFIS_DCMTK_RELEASEDATE " $";

/* default application titles */
#define APPLICATIONTITLE        "STORESCU"
#define PEERAPPLICATIONTITLE    "ANY-SCP"
/* default application filepathDir was send */  
#define FILE_PATH_DIR_SEND "E:\Data\Dcm Data\PET_DICOM"

static OFBool opt_verbose = OFFalse;
static OFBool opt_showPresentationContexts = OFFalse;
static OFBool opt_debug = OFFalse;
static OFBool opt_abortAssociation = OFFalse;
static OFCmdUnsignedInt opt_maxReceivePDULength = ASC_DEFAULTMAXPDU;
static OFCmdUnsignedInt opt_maxSendPDULength = 0;
static E_TransferSyntax opt_networkTransferSyntax = EXS_Unknown;

static OFBool opt_haltOnUnsuccessfulStore = OFTrue;
static OFBool unsuccessfulStoreEncountered = OFFalse;
static int lastStatusCode = STATUS_Success;

static OFBool opt_proposeOnlyRequiredPresentationContexts = OFFalse;
static OFBool opt_combineProposedTransferSyntaxes = OFFalse;

static OFCmdUnsignedInt opt_repeatCount = 1;
static OFCmdUnsignedInt opt_inventPatientCount = 25;
static OFCmdUnsignedInt opt_inventStudyCount = 50;
static OFCmdUnsignedInt opt_inventSeriesCount = 100;
static OFBool opt_inventSOPInstanceInformation = OFFalse;
static OFBool opt_correctUIDPadding = OFFalse;
static OFString patientNamePrefix("OFFIS^TEST_PN_");   // PatientName is PN (maximum 16 chars)
static OFString patientIDPrefix("PID_"); // PatientID is LO (maximum 64 chars)
static OFString studyIDPrefix("SID_");   // StudyID is SH (maximum 16 chars)
static OFString accessionNumberPrefix;  // AccessionNumber is SH (maximum 16 chars)
static OFBool opt_secureConnection = OFFalse; /* default: no secure connection */
static const char *opt_configFile = NULL;
static const char *opt_profileName = NULL;
T_DIMSE_BlockingMode opt_blockMode = DIMSE_BLOCKING;
int opt_dimse_timeout = 0;
int opt_acse_timeout = 30;

#ifdef WITH_ZLIB
static OFCmdUnsignedInt opt_compressionLevel = 0;
#endif

#ifdef WITH_OPENSSL
static int         opt_keyFileFormat = SSL_FILETYPE_PEM;
static OFBool      opt_doAuthenticate = OFFalse;
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


static void
errmsg(const char *msg,...)
{
    va_list args;
	
    fprintf(stderr, "%s: ", OFFIS_CONSOLE_APPLICATION);
    va_start(args, msg);
    vfprintf(stderr, msg, args);
    va_end(args);
    fprintf(stderr, "\n");
}


static OFCondition
addStoragePresentationContexts(T_ASC_Parameters *params, OFList<OFString>& sopClasses);

static OFCondition
cstore(T_ASC_Association *assoc, const OFString& fname);

static void
TraverseFilePathDir(const OFString fpath);

#define SHORTCOL 4
#define LONGCOL 19

int
main(int argc, char *argv[])
{
    const char *opt_peer = NULL;
    OFCmdUnsignedInt opt_port = 104;
    const char *opt_peerTitle = PEERAPPLICATIONTITLE;
    const char *opt_ourTitle = APPLICATIONTITLE;
	
    OFList<OFString> fileNameList;       // list of files to transfer to SCP
    OFList<OFString> sopClassUIDList;    // the list of sop classes
    OFList<OFString> sopInstanceUIDList; // the list of sop instances
	
    T_ASC_Network *net;
    T_ASC_Parameters *params;
    DIC_NODENAME localHost;
    DIC_NODENAME peerHost;
    T_ASC_Association *assoc;
    DcmAssociationConfiguration asccfg; // handler for association configuration profiles
	
#ifdef HAVE_WINSOCK_H
    WSAData winSockData;
    /* we need at least version 1.1 */
    WORD winSockVersionNeeded = MAKEWORD( 1, 1 );
    WSAStartup(winSockVersionNeeded, &winSockData);
#endif

    /* make sure data dictionary is loaded */
    if (!dcmDataDict.isDictionaryLoaded()) {
        fprintf(stderr, "Warning: no data dictionary loaded, check environment variable: %s\n",
			DCM_DICT_ENVIRONMENT_VARIABLE);
    }
	
    /* initialize network, i.e. create an instance of T_ASC_Network*. */
    OFCondition cond = ASC_initializeNetwork(NET_REQUESTOR, 0, opt_acse_timeout, &net);
    if (cond.bad()) {
        DimseCondition::dump(cond);
        return 1;
    }
	
    /* initialize asscociation parameters, i.e. create an instance of T_ASC_Parameters*. */
    cond = ASC_createAssociationParameters(&params, opt_maxReceivePDULength);
    if (cond.bad()) {
        DimseCondition::dump(cond);
        return 1;
    }
    /* sets this application's title and the called application's title in the params */

    ASC_setAPTitles(params, opt_ourTitle, opt_peerTitle, NULL);
	
    /* Set the transport layer type (type of network connection) in the params */
    /* strucutre. The default is an insecure connection; where OpenSSL is  */
    /* available the user is able to request an encrypted,secure connection. */
    cond = ASC_setTransportLayerType(params, opt_secureConnection);
    if (cond.bad()) {
        DimseCondition::dump(cond);
        return 1;
    }
	
	opt_peer = "127.0.0.1";
	opt_port = 104;	
	fileNameList.push_back("E:/Data/IM1.dcm");
	fileNameList.push_back("E:/Data/IM2.dcm");
	fileNameList.push_back("E:/Data/IM3.dcm");
	fileNameList.push_back("E:/Data/IM4.dcm");

    /* Figure out the presentation addresses and copy the */
    /* corresponding values into the association parameters.*/
    gethostname(localHost, sizeof(localHost) - 1);
    sprintf(peerHost, "%s:%d", opt_peer, (int)opt_port);
    ASC_setPresentationAddresses(params, localHost, peerHost);
	
    if (opt_profileName)
    {
		/* perform name mangling for config file key */
		OFString sprofile;
		const char *c = opt_profileName;
		while (*c)
		{
			if (! isspace(*c)) sprofile += (char) (toupper(*c));
			++c;
		}
		
		/* set presentation contexts as defined in config file */
		cond = asccfg.setAssociationParameters(sprofile.c_str(), *params);
    }
    else
    {
		/* Set the presentation contexts which will be negotiated */
		/* when the network connection will be established */
		cond = addStoragePresentationContexts(params, sopClassUIDList);
    }
	
    if (cond.bad()) {
        DimseCondition::dump(cond);
        return 1;
    }
	
    /* dump presentation contexts if required */
    if (opt_showPresentationContexts || opt_debug) {
        printf("Request Parameters:\n");
        ASC_dumpParameters(params, COUT);
    }
	
    /* create association, i.e. try to establish a network connection to another */
    /* DICOM application. This call creates an instance of T_ASC_Association*. */
    if (opt_verbose)
        printf("Requesting Association\n");
    cond = ASC_requestAssociation(net, params, &assoc);
    if (cond.bad()) {
        if (cond == DUL_ASSOCIATIONREJECTED) {
            T_ASC_RejectParameters rej;
			
            ASC_getRejectParameters(params, &rej);
            errmsg("Association Rejected:");
            ASC_printRejectParameters(stderr, &rej);
            return 1;
        } else {
            errmsg("Association Request Failed:");
            DimseCondition::dump(cond);
            return 1;
        }
    }
	
    /* dump the connection parameters if in debug mode*/
    if (opt_debug)
    {
        ostream& out = ofConsole.lockCout();     
        ASC_dumpConnectionParameters(assoc, out);
        ofConsole.unlockCout();
    }
	
    /* dump the presentation contexts which have been accepted/refused */
    if (opt_showPresentationContexts || opt_debug) {
        printf("Association Parameters Negotiated:\n");
        ASC_dumpParameters(params, COUT);
    }
	
    /* count the presentation contexts which have been accepted by the SCP */
    /* If there are none, finish the execution */
    if (ASC_countAcceptedPresentationContexts(params) == 0) {
        errmsg("No Acceptable Presentation Contexts");
        return 1;
    }
	
    /* dump general information concerning the establishment of the network connection if required */
    if (opt_verbose) {
        printf("Association Accepted (Max Send PDV: %lu)\n",
			assoc->sendPDVLength);
    }
	
    /* do the real work, i.e. for all files which were specified in the */
    /* command line, transmit the encapsulated DICOM objects to the SCP. */
    cond = EC_Normal;
    OFListIterator(OFString) iter = fileNameList.begin();
    OFListIterator(OFString) enditer = fileNameList.end();
	
    while ((iter != enditer) && (cond == EC_Normal)) // compare with EC_Normal since DUL_PEERREQUESTEDRELEASE is also good()
    {
        cond = cstore(assoc, *iter);
        ++iter;
    }
	
    /* tear down association, i.e. terminate network connection to SCP */
    if (cond == EC_Normal)
    {
        if (opt_abortAssociation) {
            if (opt_verbose)
                printf("Aborting Association\n");
            cond = ASC_abortAssociation(assoc);
            if (cond.bad()) {
                errmsg("Association Abort Failed:");
                DimseCondition::dump(cond);
                return 1;
            }
        } else {
            /* release association */
            if (opt_verbose)
                printf("Releasing Association\n");
            cond = ASC_releaseAssociation(assoc);
            if (cond.bad())
            {
                errmsg("Association Release Failed:");
                DimseCondition::dump(cond);
                return 1;
            }
        }
    }
    else if (cond == DUL_PEERREQUESTEDRELEASE)
    {
        errmsg("Protocol Error: peer requested release (Aborting)");
        if (opt_verbose)
            printf("Aborting Association\n");
        cond = ASC_abortAssociation(assoc);
        if (cond.bad()) {
            errmsg("Association Abort Failed:");
            DimseCondition::dump(cond);
            return 1;
        }
    }
    else if (cond == DUL_PEERABORTEDASSOCIATION)
    {
        if (opt_verbose) printf("Peer Aborted Association\n");
    }
    else
    {
        errmsg("SCU Failed:");
        DimseCondition::dump(cond);
        if (opt_verbose)
            printf("Aborting Association\n");
        cond = ASC_abortAssociation(assoc);
        if (cond.bad()) {
            errmsg("Association Abort Failed:");
            DimseCondition::dump(cond);
            return 1;
        }
    }
	
    /* destroy the association, i.e. free memory of T_ASC_Association* structure. This */
    /* call is the counterpart of ASC_requestAssociation(...) which was called above. */
    cond = ASC_destroyAssociation(&assoc);
    if (cond.bad()) {
        DimseCondition::dump(cond);
        return 1;
    }
    /* drop the network, i.e. free memory of T_ASC_Network* structure. This call */
    /* is the counterpart of ASC_initializeNetwork(...) which was called above. */
    cond = ASC_dropNetwork(&net);
    if (cond.bad()) {
        DimseCondition::dump(cond);
        return 1;
    }
	
#ifdef HAVE_WINSOCK_H
    WSACleanup();
#endif
	
#ifdef WITH_OPENSSL
    if (tLayer && opt_writeSeedFile)
    {
		if (tLayer->canWriteRandomSeed())
		{
			if (!tLayer->writeRandomSeed(opt_writeSeedFile))
			{
				CERR << "Error while writing random seed file '" << opt_writeSeedFile << "', ignoring." << endl;
			}
		} else {
			CERR << "Warning: cannot write random seed, ignoring." << endl;
		}
    }
    delete tLayer;
#endif
	
    int exitCode = 0;
    if (opt_haltOnUnsuccessfulStore && unsuccessfulStoreEncountered) {
        if (lastStatusCode == STATUS_Success) {
            // there must have been some kind of general network error
            exitCode = 0xff;
        } else {
            exitCode = (lastStatusCode >> 8); // only the least significant byte is relevant as exit code
        }
    }
	
#ifdef ON_THE_FLY_COMPRESSION
    // deregister JPEG codecs
    DJDecoderRegistration::cleanup();
    DJEncoderRegistration::cleanup();
	
    // deregister RLE codecs
    DcmRLEDecoderRegistration::cleanup();
    DcmRLEEncoderRegistration::cleanup();
#endif
	
#ifdef DEBUG
    dcmDataDict.clear();  /* useful for debugging with dmalloc */
#endif
    return exitCode;
}


static OFBool
isaListMember(OFList<OFString>& lst, OFString& s)
{
    OFListIterator(OFString) cur = lst.begin();
    OFListIterator(OFString) end = lst.end();
	
    OFBool found = OFFalse;
	
    while (cur != end && !found) {
		
        found = (s == *cur);
		
        ++cur;
    }
	
    return found;
}

static OFCondition
addPresentationContext(T_ASC_Parameters *params,
					   int presentationContextId, const OFString& abstractSyntax,
					   const OFString& transferSyntax,
					   T_ASC_SC_ROLE proposedRole = ASC_SC_ROLE_DEFAULT)
{
    const char* c_p = transferSyntax.c_str();
    OFCondition cond = ASC_addPresentationContext(params, presentationContextId,
        abstractSyntax.c_str(), &c_p, 1, proposedRole);
    return cond;
}

static OFCondition
addPresentationContext(T_ASC_Parameters *params,
					   int presentationContextId, const OFString& abstractSyntax,
					   const OFList<OFString>& transferSyntaxList,
					   T_ASC_SC_ROLE proposedRole = ASC_SC_ROLE_DEFAULT)
{
    // create an array of supported/possible transfer syntaxes
    const char** transferSyntaxes = new const char*[transferSyntaxList.size()];
    int transferSyntaxCount = 0;
    OFListConstIterator(OFString) s_cur = transferSyntaxList.begin();
    OFListConstIterator(OFString) s_end = transferSyntaxList.end();
    while (s_cur != s_end) {
        transferSyntaxes[transferSyntaxCount++] = (*s_cur).c_str();
        ++s_cur;
    }
	
    OFCondition cond = ASC_addPresentationContext(params, presentationContextId,
        abstractSyntax.c_str(), transferSyntaxes, transferSyntaxCount, proposedRole);
	
    delete[] transferSyntaxes;
    return cond;
}

static OFCondition
addStoragePresentationContexts(T_ASC_Parameters *params, OFList<OFString>& sopClasses)
{
/*
* Each SOP Class will be proposed in two presentation contexts (unless
* the opt_combineProposedTransferSyntaxes global variable is true).
* The command line specified a preferred transfer syntax to use.
* This prefered transfer syntax will be proposed in one
* presentation context and a set of alternative (fallback) transfer
* syntaxes will be proposed in a different presentation context.
*
* Generally, we prefer to use Explicitly encoded transfer syntaxes
* and if running on a Little Endian machine we prefer
* LittleEndianExplicitTransferSyntax to BigEndianTransferSyntax.
* Some SCP implementations will just select the first transfer
* syntax they support (this is not part of the standard) so
* organise the proposed transfer syntaxes to take advantage
* of such behaviour.
	*/
	
    // Which transfer syntax was preferred on the command line
    OFString preferredTransferSyntax;
    if (opt_networkTransferSyntax == EXS_Unknown) {
        /* gLocalByteOrder is defined in dcxfer.h */
        if (gLocalByteOrder == EBO_LittleEndian) {
            /* we are on a little endian machine */
            preferredTransferSyntax = UID_LittleEndianExplicitTransferSyntax;
        } else {
            /* we are on a big endian machine */
            preferredTransferSyntax = UID_BigEndianExplicitTransferSyntax;
        }
    } else {
        DcmXfer xfer(opt_networkTransferSyntax);
        preferredTransferSyntax = xfer.getXferID();
    }
	
    OFListIterator(OFString) s_cur;
    OFListIterator(OFString) s_end;
	
	
    OFList<OFString> fallbackSyntaxes;
    fallbackSyntaxes.push_back(UID_LittleEndianExplicitTransferSyntax);
    fallbackSyntaxes.push_back(UID_BigEndianExplicitTransferSyntax);
    fallbackSyntaxes.push_back(UID_LittleEndianImplicitTransferSyntax);
    // Remove the preferred syntax from the fallback list
    fallbackSyntaxes.remove(preferredTransferSyntax);
    // If little endian implicit is preferred then we don't need any fallback syntaxes
    // because it is the default transfer syntax and all applications must support it.
    if (opt_networkTransferSyntax == EXS_LittleEndianImplicit) {
        fallbackSyntaxes.clear();
    }
	
    // created a list of transfer syntaxes combined from the preferred and fallback syntaxes
    OFList<OFString> combinedSyntaxes;
    s_cur = fallbackSyntaxes.begin();
    s_end = fallbackSyntaxes.end();
    combinedSyntaxes.push_back(preferredTransferSyntax);
    while (s_cur != s_end)
    {
        if (!isaListMember(combinedSyntaxes, *s_cur)) combinedSyntaxes.push_back(*s_cur);
        ++s_cur;
    }
	
    if (!opt_proposeOnlyRequiredPresentationContexts) {
        // add the (short list of) known storage sop classes to the list
        // the array of Storage SOP Class UIDs comes from dcuid.h
        for (int i=0; i<numberOfDcmShortSCUStorageSOPClassUIDs; i++) {
            sopClasses.push_back(dcmShortSCUStorageSOPClassUIDs[i]);
        }
    }
	
    // thin out the sop classes to remove any duplicates.
    OFList<OFString> sops;
    s_cur = sopClasses.begin();
    s_end = sopClasses.end();
    while (s_cur != s_end) {
        if (!isaListMember(sops, *s_cur)) {
            sops.push_back(*s_cur);
        }
        ++s_cur;
    }
	
    // add a presentations context for each sop class / transfer syntax pair
    OFCondition cond = EC_Normal;
    int pid = 1; // presentation context id
    s_cur = sops.begin();
    s_end = sops.end();
    while (s_cur != s_end && cond.good()) {
		
        if (pid > 255) {
            errmsg("Too many presentation contexts");
            return ASC_BADPRESENTATIONCONTEXTID;
        }
		
        if (opt_combineProposedTransferSyntaxes) {
            cond = addPresentationContext(params, pid, *s_cur, combinedSyntaxes);
            pid += 2;   /* only odd presentation context id's */
        } else {
			
            // sop class with preferred transfer syntax
            cond = addPresentationContext(params, pid, *s_cur, preferredTransferSyntax);
            pid += 2;   /* only odd presentation context id's */
			
            if (fallbackSyntaxes.size() > 0) {
                if (pid > 255) {
                    errmsg("Too many presentation contexts");
                    return ASC_BADPRESENTATIONCONTEXTID;
                }
				
                // sop class with fallback transfer syntax
                cond = addPresentationContext(params, pid, *s_cur, fallbackSyntaxes);
                pid += 2;       /* only odd presentation context id's */
            }
        }
        ++s_cur;
    }
	
    return cond;
}

static int
secondsSince1970()
{
    time_t t = time(NULL);
    return (int)t;
}

static OFString
intToString(int i)
{
    char numbuf[32];
    sprintf(numbuf, "%d", i);
    return numbuf;
}

static OFString
makeUID(OFString basePrefix, int counter)
{
    OFString prefix = basePrefix + "." + intToString(counter);
    char uidbuf[65];
    OFString uid = dcmGenerateUniqueIdentifier(uidbuf, prefix.c_str());
    return uid;
}

static OFBool
updateStringAttributeValue(DcmItem* dataset, const DcmTagKey& key, OFString& value)
{
    DcmStack stack;
    DcmTag tag(key);
	
    OFCondition cond = EC_Normal;
    cond = dataset->search(key, stack, ESM_fromHere, OFFalse);
    if (cond != EC_Normal) {
        CERR << "error: updateStringAttributeValue: cannot find: " << tag.getTagName()
			<< " " << key << ": "
			<< cond.text() << endl;
        return OFFalse;
    }
	
    DcmElement* elem = (DcmElement*) stack.top();
	
    DcmVR vr(elem->ident());
    if (elem->getLength() > vr.getMaxValueLength()) {
        CERR << "error: updateStringAttributeValue: INTERNAL ERROR: " << tag.getTagName()
			<< " " << key << ": value too large (max "
            << vr.getMaxValueLength() << ") for " << vr.getVRName() << " value: " << value << endl;
        return OFFalse;
    }
	
    cond = elem->putOFStringArray(value);
    if (cond != EC_Normal) {
        CERR << "error: updateStringAttributeValue: cannot put string in attribute: " << tag.getTagName()
			<< " " << key << ": "
			<< cond.text() << endl;
        return OFFalse;
    }
	
    return OFTrue;
}

static void
replaceSOPInstanceInformation(DcmDataset* dataset)
{
    static OFCmdUnsignedInt patientCounter = 0;
    static OFCmdUnsignedInt studyCounter = 0;
    static OFCmdUnsignedInt seriesCounter = 0;
    static OFCmdUnsignedInt imageCounter = 0;
    static OFString seriesInstanceUID;
    static OFString seriesNumber;
    static OFString studyInstanceUID;
    static OFString studyID;
    static OFString accessionNumber;
    static OFString patientID;
    static OFString patientName;
	
    if (seriesInstanceUID.length() == 0) seriesInstanceUID=makeUID(SITE_SERIES_UID_ROOT, (int)seriesCounter);
    if (seriesNumber.length() == 0) seriesNumber = intToString((int)seriesCounter);
    if (studyInstanceUID.length() == 0) studyInstanceUID = makeUID(SITE_STUDY_UID_ROOT, (int)studyCounter);
    if (studyID.length() == 0) studyID = studyIDPrefix + intToString((int)secondsSince1970()) + intToString((int)studyCounter);
    if (accessionNumber.length() == 0) accessionNumber = accessionNumberPrefix + intToString(secondsSince1970()) + intToString((int)studyCounter);
    if (patientID.length() == 0) patientID = patientIDPrefix + intToString(secondsSince1970()) + intToString((int)patientCounter);
    if (patientName.length() == 0) patientName = patientNamePrefix + intToString(secondsSince1970()) + intToString((int)patientCounter);
	
    if (imageCounter >= opt_inventSeriesCount) {
        imageCounter = 0;
        seriesCounter++;
        seriesInstanceUID = makeUID(SITE_SERIES_UID_ROOT, (int)seriesCounter);
        seriesNumber = intToString((int)seriesCounter);
    }
    if (seriesCounter >= opt_inventStudyCount) {
        seriesCounter = 0;
        studyCounter++;
        studyInstanceUID = makeUID(SITE_STUDY_UID_ROOT, (int)studyCounter);
        studyID = studyIDPrefix + intToString(secondsSince1970()) + intToString((int)studyCounter);
        accessionNumber = accessionNumberPrefix + intToString(secondsSince1970()) + intToString((int)studyCounter);
    }
    if (studyCounter >= opt_inventPatientCount) {
        // we create as many patients as necessary */
        studyCounter = 0;
        patientCounter++;
        patientID = patientIDPrefix + intToString(secondsSince1970()) + intToString((int)patientCounter);
        patientName = patientNamePrefix + intToString(secondsSince1970()) + intToString((int)patientCounter);
    }
	
    OFString sopInstanceUID = makeUID(SITE_INSTANCE_UID_ROOT, (int)imageCounter);
    OFString imageNumber = intToString((int)imageCounter);
	
    if (opt_verbose) {
        COUT << "Inventing Identifying Information (" <<
            "pa" << patientCounter << ", st" << studyCounter <<
            ", se" << seriesCounter << ", im" << imageCounter << "): " << endl;
        COUT << "  PatientName=" << patientName << endl;
        COUT << "  PatientID=" << patientID << endl;
        COUT << "  StudyInstanceUID=" << studyInstanceUID << endl;
        COUT << "  StudyID=" << studyID << endl;
        COUT << "  SeriesInstanceUID=" << seriesInstanceUID << endl;
        COUT << "  SeriesNumber=" << seriesNumber << endl;
        COUT << "  SOPInstanceUID=" << sopInstanceUID << endl;
        COUT << "  ImageNumber=" << imageNumber << endl;
    }
	
    updateStringAttributeValue(dataset, DCM_PatientsName, patientName);
    updateStringAttributeValue(dataset, DCM_PatientID, patientID);
    updateStringAttributeValue(dataset, DCM_StudyInstanceUID, studyInstanceUID);
    updateStringAttributeValue(dataset, DCM_StudyID, studyID);
    updateStringAttributeValue(dataset, DCM_SeriesInstanceUID, seriesInstanceUID);
    updateStringAttributeValue(dataset, DCM_SeriesNumber, seriesNumber);
    updateStringAttributeValue(dataset, DCM_SOPInstanceUID, sopInstanceUID);
    updateStringAttributeValue(dataset, DCM_InstanceNumber, imageNumber);
	
    imageCounter++;
}

static void
progressCallback(void * /*callbackData*/,
				 T_DIMSE_StoreProgress *progress,
				 T_DIMSE_C_StoreRQ * /*req*/)
{
    if (opt_verbose) {
        switch (progress->state) {
        case DIMSE_StoreBegin:
            printf("XMIT:"); break;
        case DIMSE_StoreEnd:
            printf("\n"); break;
        default:
            putchar('.'); break;
        }
        fflush(stdout);
    }
}

static OFCondition
storeSCU(T_ASC_Association * assoc, const char *fname)
/*
* This function will read all the information from the given file,
* figure out a corresponding presentation context which will be used
* to transmit the information over the network to the SCP, and it
* will finally initiate the transmission of all data to the SCP.
*
* Parameters:
*   assoc - [in] The association (network connection to another DICOM application).
*   fname - [in] Name of the file which shall be processed.
*/
{
    DIC_US msgId = assoc->nextMsgID++;
    T_ASC_PresentationContextID presId;
    T_DIMSE_C_StoreRQ req;
    T_DIMSE_C_StoreRSP rsp;
    DIC_UI sopClass;
    DIC_UI sopInstance;
    DcmDataset *statusDetail = NULL;
	
    unsuccessfulStoreEncountered = OFTrue; // assumption
	
    if (opt_verbose) {
        printf("--------------------------\n");
        printf("Sending file: %s\n", fname);
    }
	
    /* read information from file. After the call to DcmFileFormat::loadFile(...) the information */
    /* which is encapsulated in the file will be available through the DcmFileFormat object. */
    /* In detail, it will be available through calls to DcmFileFormat::getMetaInfo() (for */
    /* meta header information) and DcmFileFormat::getDataset() (for data set information). */
    DcmFileFormat dcmff;
    OFCondition cond = dcmff.loadFile(fname);
	
    /* figure out if an error occured while the file was read*/
    if (cond.bad()) {
        errmsg("Bad DICOM file: %s: %s", fname, cond.text());
        return cond;
    }
	
    /* if required, invent new SOP instance information for the current data set (user option) */
    if (opt_inventSOPInstanceInformation) {
        replaceSOPInstanceInformation(dcmff.getDataset());
    }
	
    /* figure out which SOP class and SOP instance is encapsulated in the file */
    if (!DU_findSOPClassAndInstanceInDataSet(dcmff.getDataset(),
        sopClass, sopInstance, opt_correctUIDPadding)) {
        errmsg("No SOP Class & Instance UIDs in file: %s", fname);
        return DIMSE_BADDATA;
    }
	
    /* figure out which of the accepted presentation contexts should be used */
    DcmXfer filexfer(dcmff.getDataset()->getOriginalXfer());
	
    /* special case: if the file uses an unencapsulated transfer syntax (uncompressed
	* or deflated explicit VR) and we prefer deflated explicit VR, then try
	* to find a presentation context for deflated explicit VR first.
	*/
    if (filexfer.isNotEncapsulated() &&
        opt_networkTransferSyntax == EXS_DeflatedLittleEndianExplicit)
    {
        filexfer = EXS_DeflatedLittleEndianExplicit;
    }
	
    if (filexfer.getXfer() != EXS_Unknown) presId = ASC_findAcceptedPresentationContextID(assoc, sopClass, filexfer.getXferID());
    else presId = ASC_findAcceptedPresentationContextID(assoc, sopClass);
    if (presId == 0) {
        const char *modalityName = dcmSOPClassUIDToModality(sopClass);
        if (!modalityName) modalityName = dcmFindNameOfUID(sopClass);
        if (!modalityName) modalityName = "unknown SOP class";
        errmsg("No presentation context for: (%s) %s", modalityName, sopClass);
        return DIMSE_NOVALIDPRESENTATIONCONTEXTID;
    }
	
    /* if required, dump general information concerning transfer syntaxes */
    if (opt_verbose) {
        DcmXfer fileTransfer(dcmff.getDataset()->getOriginalXfer());
        T_ASC_PresentationContext pc;
        ASC_findAcceptedPresentationContext(assoc->params, presId, &pc);
        DcmXfer netTransfer(pc.acceptedTransferSyntax);
        printf("Transfer: %s -> %s\n",
            dcmFindNameOfUID(fileTransfer.getXferID()), dcmFindNameOfUID(netTransfer.getXferID()));
    }
	
    /* prepare the transmission of data */
    bzero((char*)&req, sizeof(req));
    req.MessageID = msgId;
    strcpy(req.AffectedSOPClassUID, sopClass);
    strcpy(req.AffectedSOPInstanceUID, sopInstance);
    req.DataSetType = DIMSE_DATASET_PRESENT;
    req.Priority = DIMSE_PRIORITY_LOW;
	
    /* if required, dump some more general information */
    if (opt_verbose) {
        printf("Store SCU RQ: MsgID %d, (%s)\n", msgId, dcmSOPClassUIDToModality(sopClass));
    }
	
    /* finally conduct transmission of data */
    cond = DIMSE_storeUser(assoc, presId, &req,
        NULL, dcmff.getDataset(), progressCallback, NULL,
        opt_blockMode, opt_dimse_timeout,
        &rsp, &statusDetail, NULL, DU_fileSize(fname));
	
		/*
		* If store command completed normally, with a status
		* of success or some warning then the image was accepted.
	*/
    if (cond == EC_Normal && (rsp.DimseStatus == STATUS_Success || DICOM_WARNING_STATUS(rsp.DimseStatus))) {
        unsuccessfulStoreEncountered = OFFalse;
    }
	
    /* remember the response's status for later transmissions of data */
    lastStatusCode = rsp.DimseStatus;
	
    /* dump some more general information */
    if (cond == EC_Normal)
    {
        if (opt_verbose) {
            DIMSE_printCStoreRSP(stdout, &rsp);
        }
    }
    else
    {
        errmsg("Store Failed, file: %s:", fname);
        DimseCondition::dump(cond);
    }
	
    /* dump status detail information if there is some */
    if (statusDetail != NULL) {
        printf("  Status Detail:\n");
        statusDetail->print(COUT);
        delete statusDetail;
    }
    /* return */
    return cond;
}


static OFCondition
cstore(T_ASC_Association * assoc, const OFString& fname)
/*
* This function will process the given file as often as is specified by opt_repeatCount.
* "Process" in this case means "read file, send C-STORE-RQ, receive C-STORE-RSP".
*
* Parameters:
*   assoc - [in] The association (network connection to another DICOM application).
*   fname - [in] Name of the file which shall be processed.
*/
{
    OFCondition cond = EC_Normal;
	
    /* opt_repeatCount specifies how many times a certain file shall be processed */
    int n = (int)opt_repeatCount;
	
    /* as long as no error occured and the counter does not equal 0 */
    while ((cond.good()) && n-- && !(opt_haltOnUnsuccessfulStore && unsuccessfulStoreEncountered))
    {
        /* process file (read file, send C-STORE-RQ, receive C-STORE-RSP) */
        cond = storeSCU(assoc, fname.c_str());
    }
	
    // we don't want to return an error code if --no-halt was specified.
    if (! opt_haltOnUnsuccessfulStore)
    {
        cond = EC_Normal;
    }
	
    /* return result value */
    return cond;
}

static void
TraverseFilePathDir(const OFString fpath)
{
/*	CFileFind OneFile;
	CString FNameTitle,DirName;
	BOOL BeWorking; 
//	m_SeriesNameTitleVect.clear();
	
	DirName = fpath+"\\*.*";
	BeWorking = OneFile.FindFile( DirName );
	while ( BeWorking ) 
	{  				
		BeWorking = OneFile.FindNextFile();
		
		if ( OneFile.IsDirectory() && !OneFile.IsDots() ) 
		{	
			DirName = OneFile.GetFilePath();
			
			TraverseFilePathDir(DirName);			
		}
		
		if ( !OneFile.IsDirectory() && !OneFile.IsDots() ) 
		{
			FNameTitle = OneFile.GetFilePath();//.GetFileTitle();
			
			int nTemp=atoi((LPTSTR)(LPCTSTR)FNameTitle);
//			m_SeriesNameTitleVect.push_back(nTemp);
		}		
	}
	
	OneFile.Close();	*/					
}

/*
** CVS Log
** $Log: storescu.cc,v $
** Revision 1.64  2005/12/08 15:44:22  meichel
** Changed include path schema for all DCMTK header files
**
** Revision 1.63  2005/11/23 16:10:23  meichel
** Added support for AES ciphersuites in TLS module. All TLS-enabled
**   tools now support the "AES TLS Secure Transport Connection Profile".
**
** Revision 1.62  2005/11/17 13:45:16  meichel
** Added command line options for DIMSE and ACSE timeouts
**
** Revision 1.61  2005/11/16 14:58:07  meichel
** Set association timeout in ASC_initializeNetwork to 30 seconds. This improves
**   the responsiveness of the tools if the peer blocks during assoc negotiation.
**
** Revision 1.60  2005/11/11 16:09:01  onken
** Added options for JPEG2000 support (lossy and lossless)
**
** Revision 1.59  2005/10/25 08:55:43  meichel
** Updated list of UIDs and added support for new transfer syntaxes
**   and storage SOP classes.
**
** Revision 1.58  2004/01/21 10:18:39  meichel
** StoreSCU with --no-halt option now also continues if errors other than a
**   missing presentation context occur, e.g. attempt to load non-DICOM file.
**
** Revision 1.57  2003/08/11 18:31:15  joergr
** Included "ctype" header file required for gcc 3.2.3 on Debian Linux.
**
** Revision 1.56  2003/07/03 15:43:37  meichel
** Adapted for use of OFListConstIterator, needed for compiling with HAVE_STL.
**
** Revision 1.55  2003/06/11 15:47:10  meichel
** Extended documentation, fixed typos
**
** Revision 1.54  2003/06/11 13:02:52  meichel
** Added support for configuration file based association negotiation
**   profiles
**
** Revision 1.53  2003/06/02 16:44:10  meichel
** Renamed local variables to avoid name clashes with STL
**
** Revision 1.52  2002/11/29 09:15:51  meichel
** Introduced new command line option --timeout for controlling the
**   connection request timeout.
**
** Revision 1.51  2002/11/27 13:04:32  meichel
** Adapted module dcmnet to use of new header file ofstdinc.h
**
** Revision 1.50  2002/11/26 08:43:22  meichel
** Replaced all includes for "zlib.h" with <zlib.h>
**   to avoid inclusion of zlib.h in the makefile dependencies.
**
** Revision 1.49  2002/11/25 18:00:20  meichel
** Converted compile time option to leniently handle space padded UIDs
**   in the Storage Service Class into command line / config file option.
**
** Revision 1.48  2002/09/23 17:53:48  joergr
** Added new command line option "--version" which prints the name and version
** number of external libraries used (incl. preparation for future support of
** 'config.guess' host identifiers).
**
** Revision 1.47  2002/09/10 16:02:06  meichel
** Added --max-send-pdu option that allows to restrict the size of
**   outgoing P-DATA PDUs
**
** Revision 1.46  2002/08/29 16:02:19  meichel
** Added --propose-deflated and --compression-level options to storescu
**
** Revision 1.45  2002/08/20 12:21:22  meichel
** Adapted code to new loadFile and saveFile methods, thus removing direct
**   use of the DICOM stream classes.
**
** Revision 1.44  2002/01/08 10:27:29  joergr
** Corrected spelling of function dcmGenerateUniqueIdentifier().
**
** Revision 1.43  2001/12/06 14:11:13  joergr
** Made description and layout of command line options more consistent.
**
** Revision 1.42  2001/11/09 15:56:25  joergr
** Renamed some of the getValue/getParam methods to avoid ambiguities reported
** by certain compilers.
**
** Revision 1.41  2001/11/01 14:38:59  wilkens
** Added lots of comments.
**
** Revision 1.40  2001/10/12 10:18:22  meichel
** Replaced the CONDITION types, constants and functions in the dcmnet module
**   by an OFCondition based implementation which eliminates the global condition
**   stack.  This is a major change, caveat emptor!
**
** Revision 1.39  2001/09/28 13:21:42  joergr
** Replaced "cerr" by "CERR".
**
** Revision 1.38  2001/09/26 12:28:56  meichel
** Implemented changes in dcmnet required by the adaptation of dcmdata
**   to class OFCondition.  Removed some unused code.
**
** Revision 1.37  2001/06/01 15:50:03  meichel
** Updated copyright header
**
** Revision 1.36  2000/08/10 14:50:49  meichel
** Added initial OpenSSL support.
**
** Revision 1.35  2000/06/07 13:56:18  meichel
** Output stream now passed as mandatory parameter to ASC_dumpParameters.
**
** Revision 1.34  2000/06/07 08:58:10  meichel
** added optional paramter to DIMSE_storeUser that enables precise file size
**   information inside the store user callback.
**
** Revision 1.33  2000/04/14 16:29:27  meichel
** Removed default value from output stream passed to print() method.
**   Required for use in multi-thread environments.
**
** Revision 1.32  2000/03/08 16:43:17  meichel
** Updated copyright header.
**
** Revision 1.31  2000/03/03 14:11:12  meichel
** iImplemented library support for redirecting error messages into memory
**   instead of printing them to stdout/stderr for GUI applications.
**
** Revision 1.30  2000/02/29 11:49:50  meichel
** Removed support for VS value representation. This was proposed in CP 101
**   but never became part of the standard.
**
** Revision 1.29  2000/02/23 15:12:22  meichel
** Corrected macro for Borland C++ Builder 4 workaround.
**
** Revision 1.28  2000/02/03 11:50:09  meichel
** Moved UID related functions from dcmnet (diutil.h) to dcmdata (dcuid.h)
**   where they belong. Renamed access functions to dcmSOPClassUIDToModality
**   and dcmGuessModalityBytes.
**
** Revision 1.27  2000/02/02 14:41:19  joergr
** Replaced 'delete' statements by 'delete[]' for objects created with 'new[]'.
**
** Revision 1.26  2000/02/01 10:24:03  meichel
** Avoiding to include <stdlib.h> as extern "C" on Borland C++ Builder 4,
**   workaround for bug in compiler header files.
**
** Revision 1.25  1999/07/14 12:03:35  meichel
** Updated data dictionary for supplement 29, 39, 33_lb, CP packet 4 and 5.
**   Corrected dcmtk applications for changes in attribute name constants.
**
** Revision 1.24  1999/04/30 16:40:23  meichel
** Minor code purifications to keep Sun CC 2.0.1 quiet
**
** Revision 1.23  1999/04/27 17:24:24  meichel
** Updated storescu and storescp for minor changes is command line class.
**
** Revision 1.22  1999/04/27 12:27:00  meichel
** Adapted storescu to new command line option scheme. Added support for
**   transmission of compressed images and on-the-fly creation of new UIDs.
**
** Revision 1.21  1999/03/29 11:19:56  meichel
** Cleaned up dcmnet code for char* to const char* assignments.
**
** Revision 1.20  1998/10/20 08:20:22  meichel
** Closed some memory leaks in dcmdata and dcmnet libraries.
**
** Revision 1.19  1998/04/02 14:46:46  joergr
** Corrected bug in command line parser of storescu.
**
** Revision 1.18  1998/01/14 14:35:17  hewett
** Added command line option (-ti) to restrict association negotiation
** to only propose the default Little Endian Implicit VR Transfer Syntax.
** Modified existing -u command line option to also disable generation
** of UT and VS (previously just disabled generation of UN).
**
** Revision 1.17  1997/08/06 12:23:01  andreas
** Change needed version number of WINSOCK to 1.1 to support WINDOWS 95
**
** Revision 1.16  1997/07/21 08:37:04  andreas
** - Replace all boolean types (BOOLEAN, CTNBOOLEAN, DICOM_BOOL, BOOL)
**   with one unique boolean type OFBool.
**
** Revision 1.15  1997/05/29 15:52:58  meichel
** Added constant for dcmtk release date in dcuid.h.
** All dcmtk applications now contain a version string
** which is displayed with the command line options ("usage" message)
** and which can be queried in the binary with the "ident" command.
**
** Revision 1.14  1997/05/28 12:03:26  meichel
** Corrected spacing in usage message.
**
** Revision 1.13  1997/05/23 10:43:04  meichel
** Fixed a bug in storescu application: -u option did not work correctly.
**
** Revision 1.12  1997/05/22 13:30:00  hewett
** Modified the test for presence of a data dictionary to use the
** method DcmDataDictionary::isDictionaryLoaded().
**
** Revision 1.11  1997/05/20 10:17:06  meichel
** *** empty log message ***
**
** Revision 1.10  1997/05/20 10:00:24  meichel
** Fixed bug in storescu which caused a segmentation fault when transmission
** of a non-image IOD (i.e. visit IOD) was attempted.
**
** Revision 1.9  1997/03/27 16:11:28  hewett
** Added command line switches allowing generation of UN to
** be disabled (it is enabled by default).
**
** Revision 1.8  1996/12/16 15:14:01  hewett
** Added bugfix for WINSOCK support.  The required WINSOCK version
** number was being incorrectly set to version 0.1.  The fixed
** WINSOCK initialisation now uses the MAKEWORD macro to correctly
** set the required version number. This bugfix was contributed
** by Dr. Yongjian Bao of Innomed GmbH, Germany.
**
** Revision 1.7  1996/09/27 14:05:06  hewett
** Added calls to initialise WINSOCK library for Win32 environment.  Only
** compiled in if HAVE_WINSOCK_H
**
** Revision 1.6  1996/09/27 08:24:30  hewett
** System header files now enclosed with BEGIN_EXTERN_C/END_EXTERN_C
**
** Revision 1.5  1996/09/24 16:21:17  hewett
** Now uses global table of Storage SOP Class UIDs (from dcuid.h).
** Added preliminary support for the Macintosh environment (GUSI library).
**
** Revision 1.4  1996/09/03 11:39:03  hewett
** Added copyright information.
**
** Revision 1.3  1996/04/25 16:19:18  hewett
** Added char* parameter casts to bzero() calls.
**
** Revision 1.2  1996/04/22 09:56:20  hewett
** Delete unused application conditions.
**
** Revision 1.1.1.1  1996/03/26 18:38:44  hewett
** Initial Release.
**
*/

