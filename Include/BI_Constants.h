/********************************************************************
File Name:	BI_Constants.h
Written by: Zhao Lei

Copyright(C) XinaoMDT, 2001 -

Purpose:
	This file defines all the constants which are used by BI image file header.

History:
    2002/6/11   Modified some constants and flags.
        -- Li Jianqi, Xie Haibin, Yang Guang
    2002/6/1    Modified by Yang Guang.
        Changed all the type names and enumeration value names to meet 
    the coding standard, which is also eaier to remember.
	2001/?/?	Created.
*********************************************************************/

#ifndef _BI_CONSVAR_H_
#define _BI_CONSVAR_H_

/***************** Constants ************************/
/* Size Constants */

const short SIZETITLE   =   5;
const short SIZEVERSION =	6;
const short SIZEID		=	16;
const short SIZEDESC	=	128;
const short SIZENAME	=	32;
const short SIZEPARAM	=	8;
const short SIZEUSERVAR	=	64;

/* Sex */
const char	MALE	='M';
const char	FEMAIL	='F';
const char  UNKNOWN ='N';

/* Nuclei Type */
enum ENucleiType 
{
	ntH1 = 0,
	ntHe3,
	ntC13,
    ntF19,
    ntNa23,
    ntP31,
    ntK39,
    ntXe129,
};

//! Body position.
enum EBodyPosition
{
	bpSupine,
	bpProne,
	bpDecubitusRight,
	bpDecubitusLeft,
};

//! Body entry.
enum EBodyEntry
{
	beHeadFirst,
	beFootFirst, 
};
 
//! Body part examined.
enum EBodyPart
{
	bpHead,
	bpNeck,
	bpShoulder, 
	bpCervicalSpine,
	bpThoracicSpine, 
	bpLumberSpine,
	bpSpine,
	bpChest,
	bpBreast, 
	bpAbdomen, 
	bpPelvis, 
	bpHip, 
    bpElbow, 
	bpWrist,
    bpArm,
    bpHand,
    bpKnee,
    bpLeg,
    bpAnkle,
    bpFoot,
    bpHeart,
    bpVascularHead,
    bpVascularNeck,
    bpVascularBody,
    bpVascularPeripheral,
};

//! Scanned image plane.
enum EImagePlane
{
	ipAxial,
    ipCoronal,
    ipSagittal,
    ipOblique,
};

enum ELandmarkPosition
{
	lpNasion,
    lpSternal,
    lpNotch,
    lpXyphoid,
    lpIlliacCrest,
    lpSymphPublis,
};

enum EImageMode 
{
	im1D, 
	im2D,
	im3D,
	imCine,
};

enum EHumanCoordinateDirection
{
	hcdLR,		//!< left-right
	hcdAP,		//!< anterior-posterior
	hcdSI		//!< superior-inferior
};

// 预饱和带方向
enum EPresaturationDirection
{
	pdNone,
	pdRead,
	pdPhase,
	pdSlice,
	pdAll
};

//! Used to denote presaturation Direction.
enum EScancubeDirection
{
	sdPRead,
	sdNRead,
	sdPPhase,
	sdNPhase,
	sdPSlice,
	sdNSlice
};
//! \name Image data type related flags.
//@{
const int cnstRaw               = 1;    //! Raw data.
const int cnstMagnitudeImage    = 2;	//! magnitude image data.
const int cnstPhase             = 4;    //! Phase map.
const int cnstReal              = 8;	//! Real part.
const int cnstImaginary         = 16;   //! Imaginary part.
//@}

//! \name Pulse sequence type and pulse sequence options.
/*!
    \remarks SeriesFIelds.PulseOptions holds all bits for following flags.
*/
//@{
const int cnstBasicSequenceTypeMask = 255;

const int cnstGRE		= 1;
const int cnstSE		= 2;
const int cnstFSE		= 3;
const int cnstFLAIR     = 4;	
const int cnstSTIR		= 5; 
const int cnstSSFSE		= 6;
const int cnstSEDWI		= 7;
const int cnstSPGR		= 8;
const int cnstTOF3D		= 9;
const int cnstIRSE	    = 10;
const int cnstTOF2D     = 11;
const int cnstEPI		= 12;
const int cnstSSFP      = 13;
const int cnstEPIDWI    = 14;
//const int cnstFATWATER= 14;

const int cnstSPGR2D	= 15;
const int cnstSPGR2DBH	= 16;
const int cnstLSDI		= 17;
const int cnstSSFSE3D	= 18;
const int cnstDeGhost	= 19;
const int cnstWFI		= 20;
//Added by Tom, --2007.08.10
const int cnstSSFP3D	= 21;

const int cnstShimming  = 254; //LZ 2004-8-25  Shimming

//20070213.liuxuan.for shimming.8
const int cnstCalibrationShimming = 253;
const int cnstAutoCalibration = 252;
//20070213.liuxuan.for shimming.8

const int cnstRespiratoryTriggering		= 0x800;
const int cnstECGGating					= 0x1000;
const int cnstPeripheralGating			= 0x2000;
const int cnstMTC						= 0x4000;
const int cnstIRPreparation				= 0x8000;
const int cnstDEPreparation				= 0x10000;
const int cnstPOMP						= 0x20000;
const int cnstChop						= 0x40000;
const int cnstMetalArtifactsRemoval		= 0x80000; // option for SE2D
//@}

//! \name Options do not influence pulse sequence
/*! 
    \remarks SeriesFields.OtherOptions holds all bits for following flags.
*/
//@{
const int cnstSliceInterleave			= 0x001;
const int cnstSingleSliceMethod			= 0x002;
const int cnstHalfEcho					= 0x004;

// following flag is removed because Li and Xie think we can use 
// HalfKSpaceRatio's value as an indicator.
// const int cnstHalfKSpace				= 0x008; 

const int cnstNoPhaseWrap				= 0x010;
const int cnstZeroFilling512			= 0x020;
const int cnstZeroFilling1024			= 0x040;
const int cnstSliceZeroFilling2			= 0x080;
const int cnstSliceZeroFilling4			= 0x100;
const int cnstAngioFlag					= 0x200;
const int cnstDefaultFrequencyDir		= 0x400;
//Added by Tom, --2007.08.10
const int cnstCycleScanFlag				= 0x100000;	// for cycling scaning
//@}

/* Saturation applied */
enum ESaturationType
{
	stNone,
	stFat,
	stWater,
};

/* Phase Encoding Order */
enum EPhaseEncodingOrder 
{
	peoSequential,
    peoCentrical,
};

/* RF pulse type */
enum ERFPulseType
{
	rptHard,
	rptTwoLobeSinc,
};

/* Image Type */
enum EImageType
{
	itMPR,
	itProjection,
    itImage,
    itT1Map,
    itT2Map,
    itDiffusionMap,
    itDensityMap,
    itPhaseMap,
    itVelocityMap,
    itImageAddition,
    itPhaseSubtract,
    itModulusSubtract,
    itOther,
};

/***************** Variables ************************/
enum ELogicalChannel
{
	lcX,
    lcY,
    lcZ,
    lcRF,
    lcAcq,
};

//! Type of the coil.
/*!
    YG: This may be out of sync with our system.
	\remarks The maximum RFA of each coil is stored in the hardware database.
	Currently, CConfigManager is used to maintain this database. If new 
	enumeration values are added to the ECoilType, the CConfigManager must be
	modified to reflect the change.
*/
enum ECoilType
{
	// XB coils
	ctSystemCoil		= 0,
	ctHeadSolonoid		= 1, // ok
    ctBodySolonoidL		= 2,
    ctBodySolonoidM		= 3,
    ctBodySolonoidS		= 4, // ok
	ctNeckSolonoid		= 5,
	ctHeadQD			= 6,
	ctKneeQD			= 7, // ok
	ctBodyQDL			= 8,
	ctBodyQDM			= 9,
	ctBodyQDS			= 10,
    ctSurfaceNeck		= 11,
    ctSurfaceElbow		= 12,
    ctSurfaceShoulder	= 13,
    ctPhaseArraySpine	= 14,
    ctPhaseArrayCardiac	= 15,

	// 3rd party coils
	ctHeadRecv			= 128,
	ctBodyRecvMid		= 129,
	ctBodyRecvSmall		= 130,
	ctNeckRecv			= 131,
	ctKneeRecv			= 132,

	//瑞孚泰线圈
	ctHeadCoilQD        = 150,
	ctBodyCoilQDA       = 151,
	ctBodyCoilQDB       = 152,
	ctBodyCoilQDC       = 153,
	ctKneeCoilQD        = 154,
	ctSpineCoilS        = 155,
	ctNeckCoilQD        = 156,
	ctFootCoilS         = 157,
	ctWristCoilS        = 158,
	ctBreastCoilS       = 159,
	ctShoulderCoilS     = 160,
	//Added by Tom, --2007.08.10
	ctSoftHeadCoilS     = 161
};

//! Type of flow compensation applied.
enum EFlowCompensation
{
	fcNone,
	fcRead,
	fcSlice,
};

//! Type of respiratory compensation applied.
enum ERespiratoryCompensation
{
	rcNone,         //! No respiratory compensation applied.
	rcHighSort,
	rcLowSort,
};

#endif
