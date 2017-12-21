# Microsoft Developer Studio Project File - Name="Surgery" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Surgery - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Surgery.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Surgery.mak" CFG="Surgery - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Surgery - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Surgery - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Surgery - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\include\UtilDll" /I "../include" /I "../include/UI1" /I "../include/modeling" /I "../include/Modeling/Common" /I "../include/Modeling/Model" /I "..\include\DICOM" /I "../include/Panaskin" /I "../include/xtk" /I "..\DcmTk\config\include" /I "..\DcmTk\dcmdata\include" /I "..\DcmTk\dcmnet\include" /I "..\DcmTk\ofstd\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /D "_XTP_STATICLINK" /FR /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 opengl32.lib glu32.lib Tps.lib ImriDataAccess.lib UIUtilDll.lib trimath.lib TrackControl.Lib Tracker.lib GeometryDll.lib DisplayDll.Lib DataManager.lib MriCalibrationDll.lib MRISeriesDLL.lib GeoAnalysisDll.lib ImageDisplay.lib TCPCom.lib ImageAnalysisDll.lib MyTimer.lib PatInfoMrg.Lib LogDll.lib ImgConvertor.lib ImriUlServer.lib LicenseApi.lib Encode.lib GetHardDeviceInfo.lib Netapi32.lib LicenseLib.lib PanaSkin.lib version.lib msimg32.lib vImage.lib imgFusion.lib XBCStoreSvr.lib DCMTK.lib /nologo /subsystem:windows /machine:I386 /out:"../Release/Surgery.exe" /libpath:"../LibR"
# SUBTRACT LINK32 /pdb:none

!ELSEIF  "$(CFG)" == "Surgery - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\include\UtilDll" /I "..\include" /I "..\Include\ImgHeader" /I "../include/UI1" /I "../include/modeling" /I "../include/Modeling/Common" /I "../include/Modeling/Model" /I "..\include\DICOM" /I "../include/Panaskin" /I "../include/xtk" /I "..\DcmTk\config\include" /I "..\DcmTk\dcmdata\include" /I "..\DcmTk\dcmnet\include" /I "..\DcmTk\ofstd\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_XTP_STATICLINK" /D "_AFXDLL" /FR /FD /GZ /Zm500 /c
# SUBTRACT CPP /X
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 opengl32.lib glu32.lib TpsD.lib ImriDataAccessD.lib UIUtilDllD.lib trimathD.lib TrackControlD.lib TrackerD.lib GeometryDllD.lib DisplayDllD.Lib DataManagerD.lib MriCalibrationDllD.lib MRISeriesDLLD.lib GeoAnalysisDllD.lib ImageDisplayD.lib TCPComD.lib ImageAnalysisDllD.lib MyTimer.lib PatInfoMrgD.Lib LogDllD.lib ImgConvertorD.lib ImriUlServerD.lib LicenseApiD.lib EncodeD.lib GetHardDeviceInfoD.lib Netapi32.lib LicenseLibD.lib PanaSkinD.lib version.lib msimg32.lib vImageD.lib imgFusionD.lib XBCStoreSvrD.lib DCMTK.lib /nologo /subsystem:windows /debug /machine:I386 /out:"..\BinD\SurgeryD.exe" /pdbtype:sept /libpath:"../LibD"
# SUBTRACT LINK32 /pdb:none

!ENDIF 

# Begin Target

# Name "Surgery - Win32 Release"
# Name "Surgery - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\BiopsyProbeSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\CompareFrame.cpp
# End Source File
# Begin Source File

SOURCE=.\ComSetPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ConfigMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\CrashRstore.cpp
# End Source File
# Begin Source File

SOURCE=.\CryotherapyDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CryotherapyProbeSetting.cpp
# End Source File
# Begin Source File

SOURCE=.\DataPool.cpp
# End Source File
# Begin Source File

SOURCE=.\Dispaly4Dlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DisplayDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBaseCalibration.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgBaseSelection.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCaliModeSel.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCaliPT.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCmdBar.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgCtrlCompare.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDICOMSrv.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDispCompare.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDisplayControl.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgDisplayWrap.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgFusion.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgImportPatient.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgInfoHist.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMain.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgManualBaseCali.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgManualCalibrationCP.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMovePT.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMRI2DDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMRIBrowse.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMriScanOptions.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMRIWrap.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMyAbout.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgNavigate.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPatient.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPatientInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgScannerControl.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSelectSeries.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSeries.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSetupLegend.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSimulate.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTargeting.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTime.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTitle.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTps.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTpsCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTpsDisp.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTracker.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTracking.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTrackingStatus.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgUSDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgWizard.cpp
# End Source File
# Begin Source File

SOURCE=.\GlobalColor.cpp
# End Source File
# Begin Source File

SOURCE=.\GlobalFuncs.cpp
# End Source File
# Begin Source File

SOURCE=.\GradientCorrection.cpp
# End Source File
# Begin Source File

SOURCE=.\GradientCorrection1.cpp
# End Source File
# Begin Source File

SOURCE=.\IGSCPManager.cpp
# End Source File
# Begin Source File

SOURCE=.\IMRIScene.cpp
# End Source File
# Begin Source File

SOURCE=.\MRIBuffer.cpp
# End Source File
# Begin Source File

SOURCE=.\MRIRectangle.cpp
# End Source File
# Begin Source File

SOURCE=.\MRIRectSeries.cpp
# End Source File
# Begin Source File

SOURCE=.\OleDropTargetEx.cpp
# End Source File
# Begin Source File

SOURCE=.\PopupMenu.cpp
# End Source File
# Begin Source File

SOURCE=.\Probe.cpp
# End Source File
# Begin Source File

SOURCE=.\ProbePool.cpp
# End Source File
# Begin Source File

SOURCE=.\ProtocolManager.cpp
# End Source File
# Begin Source File

SOURCE=.\ScrollDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\SeriesLoader.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Surgery.cpp
# End Source File
# Begin Source File

SOURCE=.\Surgery.rc
# End Source File
# Begin Source File

SOURCE=.\TargetPool.cpp
# End Source File
# Begin Source File

SOURCE=.\TrackTool.cpp
# End Source File
# Begin Source File

SOURCE=.\USRectangle.cpp
# End Source File
# Begin Source File

SOURCE=.\VirtualTarget.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\BiopsyProbeSetting.h
# End Source File
# Begin Source File

SOURCE=..\Include\CommandHandling.h
# End Source File
# Begin Source File

SOURCE=.\CompareFrame.h
# End Source File
# Begin Source File

SOURCE=.\ComSetPage.h
# End Source File
# Begin Source File

SOURCE=.\Config.h
# End Source File
# Begin Source File

SOURCE=.\ConfigMgr.h
# End Source File
# Begin Source File

SOURCE=.\CrashRstore.h
# End Source File
# Begin Source File

SOURCE=.\CryotherapyDlg.h
# End Source File
# Begin Source File

SOURCE=.\CryotherapyProbeSetting.h
# End Source File
# Begin Source File

SOURCE=..\Include\DataManager.h
# End Source File
# Begin Source File

SOURCE=.\DataPool.h
# End Source File
# Begin Source File

SOURCE=.\Dispaly4Dlg.h
# End Source File
# Begin Source File

SOURCE=.\DisplayDlg.h
# End Source File
# Begin Source File

SOURCE=.\DlgBaseCalibration.h
# End Source File
# Begin Source File

SOURCE=.\DlgBaseSelection.h
# End Source File
# Begin Source File

SOURCE=.\DlgCaliModeSel.h
# End Source File
# Begin Source File

SOURCE=.\DlgCaliPT.h
# End Source File
# Begin Source File

SOURCE=.\DlgCmdBar.h
# End Source File
# Begin Source File

SOURCE=.\DlgCtrlCompare.h
# End Source File
# Begin Source File

SOURCE=.\DlgDICOMSrv.h
# End Source File
# Begin Source File

SOURCE=.\DlgDispCompare.h
# End Source File
# Begin Source File

SOURCE=.\DlgDisplayControl.h
# End Source File
# Begin Source File

SOURCE=.\DlgDisplayWrap.h
# End Source File
# Begin Source File

SOURCE=.\DlgFusion.h
# End Source File
# Begin Source File

SOURCE=.\DlgImportPatient.h
# End Source File
# Begin Source File

SOURCE=.\DlgInfoHist.h
# End Source File
# Begin Source File

SOURCE=.\DlgMain.h
# End Source File
# Begin Source File

SOURCE=.\DlgManualBaseCali.h
# End Source File
# Begin Source File

SOURCE=.\DlgManualCalibrationCP.h
# End Source File
# Begin Source File

SOURCE=.\DlgMovePT.h
# End Source File
# Begin Source File

SOURCE=.\DlgMRI2DDisplay.h
# End Source File
# Begin Source File

SOURCE=.\DlgMRIBrowse.h
# End Source File
# Begin Source File

SOURCE=.\DlgMriScanOptions.h
# End Source File
# Begin Source File

SOURCE=.\DlgMRIWrap.h
# End Source File
# Begin Source File

SOURCE=.\DlgMyAbout.h
# End Source File
# Begin Source File

SOURCE=.\DlgNavigate.h
# End Source File
# Begin Source File

SOURCE=.\DlgPatient.h
# End Source File
# Begin Source File

SOURCE=.\DlgPatientInfo.h
# End Source File
# Begin Source File

SOURCE=.\DlgScannerControl.h
# End Source File
# Begin Source File

SOURCE=.\DlgSelectSeries.h
# End Source File
# Begin Source File

SOURCE=.\DlgSeries.h
# End Source File
# Begin Source File

SOURCE=.\DlgSetupLegend.h
# End Source File
# Begin Source File

SOURCE=.\DlgSimulate.h
# End Source File
# Begin Source File

SOURCE=.\DlgTargeting.h
# End Source File
# Begin Source File

SOURCE=.\DlgTime.h
# End Source File
# Begin Source File

SOURCE=.\DlgTitle.h
# End Source File
# Begin Source File

SOURCE=.\DlgTps.h
# End Source File
# Begin Source File

SOURCE=.\DlgTpsCtrl.h
# End Source File
# Begin Source File

SOURCE=.\DlgTpsDisp.h
# End Source File
# Begin Source File

SOURCE=.\DlgTracker.h
# End Source File
# Begin Source File

SOURCE=.\DlgTracking.h
# End Source File
# Begin Source File

SOURCE=.\DlgTrackingStatus.h
# End Source File
# Begin Source File

SOURCE=.\DlgUSDisplay.h
# End Source File
# Begin Source File

SOURCE=.\DlgWizard.h
# End Source File
# Begin Source File

SOURCE=.\GlobalColor.h
# End Source File
# Begin Source File

SOURCE=.\GlobalFuncs.h
# End Source File
# Begin Source File

SOURCE=.\GradientCorrection.h
# End Source File
# Begin Source File

SOURCE=.\GradientCorrection1.h
# End Source File
# Begin Source File

SOURCE=.\header.h
# End Source File
# Begin Source File

SOURCE=.\IGSCPManager.h
# End Source File
# Begin Source File

SOURCE=.\IMRIScene.h
# End Source File
# Begin Source File

SOURCE=.\MRIBuffer.h
# End Source File
# Begin Source File

SOURCE=.\MRIRectangle.h
# End Source File
# Begin Source File

SOURCE=.\MRIRectSeries.h
# End Source File
# Begin Source File

SOURCE=..\Include\Modeling\MyGeometry.h
# End Source File
# Begin Source File

SOURCE=.\NavigationMode.h
# End Source File
# Begin Source File

SOURCE=.\OleDropTargetEx.h
# End Source File
# Begin Source File

SOURCE=.\PopupMenu.h
# End Source File
# Begin Source File

SOURCE=.\Probe.h
# End Source File
# Begin Source File

SOURCE=.\ProbePool.h
# End Source File
# Begin Source File

SOURCE=.\ProtocolManager.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\Restore.h
# End Source File
# Begin Source File

SOURCE=.\ScrollDialog.h
# End Source File
# Begin Source File

SOURCE=.\SeriesLoader.h
# End Source File
# Begin Source File

SOURCE=.\SplitOverride.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\Surgery.h
# End Source File
# Begin Source File

SOURCE=.\TargetPool.h
# End Source File
# Begin Source File

SOURCE=.\TrackTool.h
# End Source File
# Begin Source File

SOURCE=.\Shimming\TuneShimming.h
# End Source File
# Begin Source File

SOURCE=.\USRectangle.h
# End Source File
# Begin Source File

SOURCE=.\ViewCompareCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ViewCompareDisp.h
# End Source File
# Begin Source File

SOURCE=.\VirtualTarget.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=".\res\001¸±±¾.bmp"
# End Source File
# Begin Source File

SOURCE=.\res\011.bmp
# End Source File
# Begin Source File

SOURCE=.\res\1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\11.bmp
# End Source File
# Begin Source File

SOURCE=.\arrow.cur
# End Source File
# Begin Source File

SOURCE=.\arrowcop.cur
# End Source File
# Begin Source File

SOURCE=.\res\base.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\button_normal.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CAMMOVE.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Exit.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Exit10.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Exit11.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Exit2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Exit20.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Exit21.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Exit3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Exit30.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Exit31.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Exit4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Exit5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\FullDisplay.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Help.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\IconWarning.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ImgComp.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ImgDisplay.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LedOff.ico
# End Source File
# Begin Source File

SOURCE=.\res\LedOn.ico
# End Source File
# Begin Source File

SOURCE=.\res\logo.bmp
# End Source File
# Begin Source File

SOURCE=.\res\lwe.cur
# End Source File
# Begin Source File

SOURCE=.\res\measure.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MEASURE2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MovePT.bmp
# End Source File
# Begin Source File

SOURCE=.\nodrop.cur
# End Source File
# Begin Source File

SOURCE=.\res\ScreenSnap.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Seprate.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Surgery.ico
# End Source File
# Begin Source File

SOURCE=.\res\Surgery.rc2
# End Source File
# Begin Source File

SOURCE=.\Surgery.rgs
# End Source File
# Begin Source File

SOURCE=.\res\Surgery2.ico
# End Source File
# Begin Source File

SOURCE=.\res\Surgery3.ico
# End Source File
# Begin Source File

SOURCE=.\res\SurgeryDoc.ico
# End Source File
# Begin Source File

SOURCE=.\res\t1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\t2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\t3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\t4.bmp
# End Source File
# Begin Source File

SOURCE=.\res\t5.bmp
# End Source File
# Begin Source File

SOURCE=.\res\target.bmp
# End Source File
# Begin Source File

SOURCE=.\res\target.cur
# End Source File
# Begin Source File

SOURCE=.\res\target2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\toolbar1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Track.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Track2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Tracking.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Tracking2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WBG.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WBOTTOM.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WIZ_BAS.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WIZ_BAS_DONE.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WIZ_BAS_FAIL.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WIZ_BAS_ING.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WIZ_BG2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WIZ_CAL.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WIZ_CAL_DONE.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WIZ_CAL_FAIL.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WIZ_CAL_ING.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WIZ_CAM.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WIZ_CAM_DONE.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WIZ_CAM_FAIL.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WIZ_CAM_ING.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WIZ_MRI.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WIZ_MRI_DONE.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WIZ_MRI_FAIL.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WIZ_MRI_ING.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WIZ_PAT.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WIZ_PAT_DONE.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WIZ_PAT_FAIL.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WIZ_PAT_ING.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WT1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WT2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WT3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\WT4.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\base1.png
# End Source File
# Begin Source File

SOURCE=.\res\base2.png
# End Source File
# Begin Source File

SOURCE=.\res\base3.png
# End Source File
# Begin Source File

SOURCE=.\res\base4.png
# End Source File
# Begin Source File

SOURCE=.\res\BlackGround.png
# End Source File
# Begin Source File

SOURCE=.\res\btnNormal.png
# End Source File
# Begin Source File

SOURCE=.\res\Circle.png
# End Source File
# Begin Source File

SOURCE=.\res\down.png
# End Source File
# Begin Source File

SOURCE=.\res\f1.png
# End Source File
# Begin Source File

SOURCE=.\res\f2.png
# End Source File
# Begin Source File

SOURCE=.\res\f3.png
# End Source File
# Begin Source File

SOURCE=.\res\f4.png
# End Source File
# Begin Source File

SOURCE=.\res\f5.png
# End Source File
# Begin Source File

SOURCE=.\res\f6.png
# End Source File
# Begin Source File

SOURCE=.\res\f7.png
# End Source File
# Begin Source File

SOURCE=.\res\f8.png
# End Source File
# Begin Source File

SOURCE=.\res\f9.png
# End Source File
# Begin Source File

SOURCE=.\res\GeneralBK.png
# End Source File
# Begin Source File

SOURCE=.\res\Handwriting.ani
# End Source File
# Begin Source File

SOURCE=.\res\InfoPrompt.png
# End Source File
# Begin Source File

SOURCE=.\res\left.png
# End Source File
# Begin Source File

SOURCE=.\res\logo.png
# End Source File
# Begin Source File

SOURCE=.\res\Logo1.png
# End Source File
# Begin Source File

SOURCE=.\res\manifest.xml
# End Source File
# Begin Source File

SOURCE=.\res\PrtScreen.png
# End Source File
# Begin Source File

SOURCE=.\res\Quit.png
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=.\res\right.png
# End Source File
# Begin Source File

SOURCE=.\res\targeting.png
# End Source File
# Begin Source File

SOURCE=.\res\up.png
# End Source File
# End Target
# End Project
# Section Surgery : {C51C0599-C8B5-460B-A730-B75024DBD4FD}
# 	1:14:IDD_DLG_FILTER:108
# 	2:16:Resource Include:resource.h
# 	2:19:DlgChooseFilter.cpp:DlgChooseFilter.cpp
# 	2:10:ENUM: enum:enum
# 	2:23:CLASS: CDlgChooseFilter:CDlgChooseFilter
# 	2:14:IDD_DLG_FILTER:IDD_DLG_FILTER
# 	2:19:Application Include:surgery.h
# 	2:17:DlgChooseFilter.h:DlgChooseFilter.h
# End Section
# Section Surgery : {59DAA4AB-0006-11CF-841D-0020AFD9150E}
# 	2:5:Class:CHelper
# 	2:10:HeaderFile:helper.h
# 	2:8:ImplFile:helper.cpp
# End Section
# Section Surgery : {4A11CAE6-0A22-4CC8-AA56-D7D23C5603D7}
# 	2:5:Class:CHelperScript
# 	2:10:HeaderFile:helperscript.h
# 	2:8:ImplFile:helperscript.cpp
# End Section
# Section Surgery : {59DAA4AA-0006-11CF-841D-0020AFD9150E}
# 	2:21:DefaultSinkHeaderFile:helper.h
# 	2:16:DefaultSinkClass:CHelper
# End Section
