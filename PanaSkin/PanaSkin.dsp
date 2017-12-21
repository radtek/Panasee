# Microsoft Developer Studio Project File - Name="PanaSkin" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=PanaSkin - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PanaSkin.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PanaSkin.mak" CFG="PanaSkin - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PanaSkin - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PanaSkin - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PanaSkin - Win32 Release"

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
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\include" /I "..\include\pngLib" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "_BUILD_PANASKIN_DLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 msimg32.lib /nologo /subsystem:windows /dll /machine:I386 /libpath:"..\LibR"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy release\panaskin.dll ..\release\	copy release\panaskin.lib ..\libR
# End Special Build Tool

!ELSEIF  "$(CFG)" == "PanaSkin - Win32 Debug"

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
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\include" /I "..\include\pngLib" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "_BUILD_PANASKIN_DLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 msimg32.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"Debug/PanaSkinD.dll" /pdbtype:sept /libpath:"..\LibD"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy debug\panaskinD.dll ..\binD\	copy debug\panaskinD.lib ..\libD	copy autofont.h ..\include\panaskin\	copy imagebutton.h ..\include\panaskin\	copy memdc.h ..\include\panaskin\	copy menuex.h ..\include\panaskin\	copy panaedit.h ..\include\panaskin\	copy panaseedialog.h ..\include\panaskin\	copy showimage.h ..\include\panaskin\	copy moveablectrl.h ..\include\panaskin	copy panaskin.h ..\include\panaskin	copy DlgMainSkin.h ..\include\panaskin	copy DlgTitleSkin.h ..\include\panaskin	copy DlgToolSkin.h ..\include\panaskin	copy DlgMriCtrlSkin.h ..\include\panaskin	copy PanaStatic.h ..\include\panaskin	copy DlgInfoSkin.h ..\include\panaskin	copy DlgTimeSkin.h ..\include\panaskin	copy DlgLoginSkin.h ..\include\panaskin	copy DlgWizardSkin.h ..\include\panaskin	copy DlgImageControlSkin.h ..\include\panaskin	copy PanaListCtrl.h ..\include\panaskin	copy DlgPatientSkin.h ..\include\panaskin	copy DlgSeriesSkin.h ..\include\panaskin	copy DlgTPSSkin.h ..\include\panaskin	copy DlgTPSViewSkin.h ..\include\panaskin	copy PanaMetrics.h ..\include\panaskin	copy PanaGlobal.h ..\include\panaskin	copy DlgNavigateSkin.h ..\include\Panaskin	copy DlgMPRSkin.h ..\include\Panaskin	copy\
      DlgSegMPRSkin.h ..\include\Panaskin
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "PanaSkin - Win32 Release"
# Name "PanaSkin - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\AutoFont.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgImageControlSkin.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgInfoSkin.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgLoginSkin.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMainSkin.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMPRSkin.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMriCtrlSkin.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgNavigateSkin.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgPatientSkin.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSegMPRSkin.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgSeriesSkin.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTimeSkin.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTitleSkin.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgToolSkin.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTPSSkin.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgTPSViewSkin.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgWaiting.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgWizardSkin.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageButton.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageManager.cpp
# End Source File
# Begin Source File

SOURCE=.\MemDC.cpp
# End Source File
# Begin Source File

SOURCE=.\MenuEx.cpp
# End Source File
# Begin Source File

SOURCE=.\MoveableCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\PanaCheckBox.cpp
# End Source File
# Begin Source File

SOURCE=.\PanaEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\PanaGlobal.cpp
# End Source File
# Begin Source File

SOURCE=.\PanaListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\PanaMetrics.cpp
# End Source File
# Begin Source File

SOURCE=.\PanaMsgBox.cpp
# End Source File
# Begin Source File

SOURCE=.\PanaRadioButton.cpp
# End Source File
# Begin Source File

SOURCE=.\PanaseeDialog.cpp
# End Source File
# Begin Source File

SOURCE=.\PanaSkin.cpp
# End Source File
# Begin Source File

SOURCE=.\PanaSkin.rc
# End Source File
# Begin Source File

SOURCE=.\PanaStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\ShowImage.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\AutoFont.h
# End Source File
# Begin Source File

SOURCE=.\DlgImageControlSkin.h
# End Source File
# Begin Source File

SOURCE=.\DlgInfoSkin.h
# End Source File
# Begin Source File

SOURCE=.\DlgLoginSkin.h
# End Source File
# Begin Source File

SOURCE=.\DlgMainSkin.h
# End Source File
# Begin Source File

SOURCE=.\DlgMPRSkin.h
# End Source File
# Begin Source File

SOURCE=.\DlgMriCtrlSkin.h
# End Source File
# Begin Source File

SOURCE=.\DlgNavigateSkin.h
# End Source File
# Begin Source File

SOURCE=.\DlgPatientSkin.h
# End Source File
# Begin Source File

SOURCE=.\DlgSegMPRSkin.h
# End Source File
# Begin Source File

SOURCE=.\DlgSeriesSkin.h
# End Source File
# Begin Source File

SOURCE=.\DlgTimeSkin.h
# End Source File
# Begin Source File

SOURCE=.\DlgTitleSkin.h
# End Source File
# Begin Source File

SOURCE=.\DlgToolSkin.h
# End Source File
# Begin Source File

SOURCE=.\DlgTPSSkin.h
# End Source File
# Begin Source File

SOURCE=.\DlgTPSViewSkin.h
# End Source File
# Begin Source File

SOURCE=.\DlgWaiting.h
# End Source File
# Begin Source File

SOURCE=.\DlgWizardSkin.h
# End Source File
# Begin Source File

SOURCE=.\ImageButton.h
# End Source File
# Begin Source File

SOURCE=.\ImageManager.h
# End Source File
# Begin Source File

SOURCE=.\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\MenuEx.h
# End Source File
# Begin Source File

SOURCE=.\MoveableCtrl.h
# End Source File
# Begin Source File

SOURCE=.\PanaCheckBox.h
# End Source File
# Begin Source File

SOURCE=.\PanaEdit.h
# End Source File
# Begin Source File

SOURCE=.\PanaGlobal.h
# End Source File
# Begin Source File

SOURCE=.\PanaListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\PanaMetrics.h
# End Source File
# Begin Source File

SOURCE=.\PanaMsgBox.h
# End Source File
# Begin Source File

SOURCE=.\PanaRadioButton.h
# End Source File
# Begin Source File

SOURCE=.\PanaseeDialog.h
# End Source File
# Begin Source File

SOURCE=.\PanaSkin.h
# End Source File
# Begin Source File

SOURCE=.\PanaStatic.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\ShowImage.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\Base.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bitmap_b.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00001.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00002.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00003.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bmp00004.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnArrow.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnCycle.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnCycleStop.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MriControl\BtnCycleStop.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnDownArrow.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnImage3D.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ImageControl\BtnImage3D.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnImageFirst.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ImageControl\BtnImageFirst.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnImageLast.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ImageControl\BtnImageLast.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnImageMax.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ImageControl\BtnImageMax.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnImageNext.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ImageControl\BtnImageNext.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnImagePre.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ImageControl\BtnImagePre.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnLeft.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MriControl\BtnLeft.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnLoad.bmp
# End Source File
# Begin Source File

SOURCE=.\res\btnLoginSelected.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Login\btnLoginSelected.bmp
# End Source File
# Begin Source File

SOURCE=.\res\btnLoginSelectedFrame.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Login\btnLoginSelectedFrame.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnMriControl.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnNext.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnNormal.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MriControl\BtnNormal.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnOrient1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnOrient2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnOrient3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnPatient.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnPosition1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnPosition2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnPre.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnPrescan.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnPrescanScan.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnPushAdd.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnPushDel.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnQuit.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnRight.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MriControl\BtnRight.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnScan.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnScroll.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MriControl\BtnScroll.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnSeries.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnStop.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnSwap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnSwap2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnWizardPreStep.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnWizardQuit.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnWizardSelected.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Wizard\BtnWizardSelected.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnWizardSelectedArrow.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Wizard\BtnWizardSelectedArrow.bmp
# End Source File
# Begin Source File

SOURCE=.\res\BtnWizardUnselected.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Wizard\BtnWizardUnselected.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Button.bmp
# End Source File
# Begin Source File

SOURCE=.\res\CheckFlag.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Config2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DialogSkin.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DialogSkin2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DialogSkin2Mask.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DialogSkinMask.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DlgInfomationSkin.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DlgLoginSkin.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Login\DlgLoginSkin.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DlgMainSkin.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DlgPanelSkin.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DlgTimeSkin.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DlgTitleSkin.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DlgToolSkin.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolBar\DlgToolSkin.bmp
# End Source File
# Begin Source File

SOURCE=.\res\DlgWizardSkin.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Wizard\DlgWizardSkin.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Wizard\DlgWizardSkinCT.bmp
# End Source File
# Begin Source File

SOURCE=.\res\HeaderCtrlSkin.bmp
# End Source File
# Begin Source File

SOURCE=.\res\IconApplication.bmp
# End Source File
# Begin Source File

SOURCE=.\res\IconConfig.bmp
# End Source File
# Begin Source File

SOURCE=.\res\IconConfig2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\IconErr.bmp
# End Source File
# Begin Source File

SOURCE=.\res\IconInformation.bmp
# End Source File
# Begin Source File

SOURCE=.\res\IconPatientDB.bmp
# End Source File
# Begin Source File

SOURCE=.\res\IconTOOLS.bmp
# End Source File
# Begin Source File

SOURCE=.\res\IconTPS.bmp
# End Source File
# Begin Source File

SOURCE=.\res\IconWarning.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MenuSkin.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PanaSkin.rc2
# End Source File
# Begin Source File

SOURCE=.\res\png4.bin
# End Source File
# Begin Source File

SOURCE=.\res\Probe.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ProbeFrame.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ScanFrame.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ScrollBar.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Seprate.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolBar\ToolBrowseImage.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolBrowseImage.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolBar\ToolCamera.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolCamera.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolBar\ToolCapture.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolCapture.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolBar\ToolCompareImage.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolCompareImage.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolBar\ToolMeasure.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolMeasure.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolBar\ToolMoveCamera.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolMoveCamera.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolBar\ToolPatientTable.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolPatientTable.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolBar\ToolSystem.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolSystem.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolBar\ToolTarget.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ToolTarget.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\res\Time\0.png
# End Source File
# Begin Source File

SOURCE=.\res\ImageControl\1.png
# End Source File
# Begin Source File

SOURCE=.\res\Time\1.png
# End Source File
# Begin Source File

SOURCE=.\res\ImageControl\2.png
# End Source File
# Begin Source File

SOURCE=.\res\Time\2.png
# End Source File
# Begin Source File

SOURCE=.\res\ImageControl\3.png
# End Source File
# Begin Source File

SOURCE=.\res\Time\3.png
# End Source File
# Begin Source File

SOURCE=.\res\Time\4.png
# End Source File
# Begin Source File

SOURCE=.\res\Time\5.png
# End Source File
# Begin Source File

SOURCE=.\res\Time\6.png
# End Source File
# Begin Source File

SOURCE=.\res\Time\7.png
# End Source File
# Begin Source File

SOURCE=.\res\Time\8.png
# End Source File
# Begin Source File

SOURCE=.\res\Time\9.png
# End Source File
# Begin Source File

SOURCE=.\res\Login\Application.png
# End Source File
# Begin Source File

SOURCE=.\res\Login\Application2.png
# End Source File
# Begin Source File

SOURCE=.\res\TPS\btkBK.png
# End Source File
# Begin Source File

SOURCE=.\res\MriControl\BtnArrow.png
# End Source File
# Begin Source File

SOURCE=.\res\MriControl\btnBothRound.png
# End Source File
# Begin Source File

SOURCE=.\res\MriControl\btnBothRoundInvert.png
# End Source File
# Begin Source File

SOURCE=.\res\MriControl\BtnCycle.png
# End Source File
# Begin Source File

SOURCE=.\res\MriControl\BtnDownArrow.png
# End Source File
# Begin Source File

SOURCE=.\res\ImageControl\BtnImage3D.png
# End Source File
# Begin Source File

SOURCE=.\res\ImageControl\BtnImageFirst.png
# End Source File
# Begin Source File

SOURCE=.\res\ImageControl\BtnImageLast.png
# End Source File
# Begin Source File

SOURCE=.\res\ImageControl\BtnImageMax.png
# End Source File
# Begin Source File

SOURCE=.\res\ImageControl\BtnImageNext.png
# End Source File
# Begin Source File

SOURCE=.\res\ImageControl\BtnImagePre.png
# End Source File
# Begin Source File

SOURCE=.\res\MriControl\btnLeftRound.png
# End Source File
# Begin Source File

SOURCE=.\res\MriControl\BtnLoad.png
# End Source File
# Begin Source File

SOURCE=.\res\Login\btnLoginSelectedFrame.png
# End Source File
# Begin Source File

SOURCE=.\res\MriControl\btnNormal.png
# End Source File
# Begin Source File

SOURCE=.\res\MriControl\BtnOrient1.png
# End Source File
# Begin Source File

SOURCE=.\res\MriControl\BtnOrient2.png
# End Source File
# Begin Source File

SOURCE=.\res\MriControl\BtnOrient3.png
# End Source File
# Begin Source File

SOURCE=.\res\BtnPause.png
# End Source File
# Begin Source File

SOURCE=.\res\MriControl\BtnPause.png
# End Source File
# Begin Source File

SOURCE=.\res\MriControl\BtnPosition1.png
# End Source File
# Begin Source File

SOURCE=.\res\MriControl\BtnPosition2.png
# End Source File
# Begin Source File

SOURCE=.\res\MriControl\BtnPrescan.png
# End Source File
# Begin Source File

SOURCE=.\res\MriControl\BtnPrescanScan.png
# End Source File
# Begin Source File

SOURCE=.\res\MriControl\BtnPushAdd.png
# End Source File
# Begin Source File

SOURCE=.\res\MriControl\BtnPushDel.png
# End Source File
# Begin Source File

SOURCE=.\res\Login\BtnQuit.png
# End Source File
# Begin Source File

SOURCE=.\res\MriControl\btnRightRound.png
# End Source File
# Begin Source File

SOURCE=.\res\MriControl\btnRightRoundInvert.png
# End Source File
# Begin Source File

SOURCE=.\res\MriControl\BtnScan.png
# End Source File
# Begin Source File

SOURCE=.\res\MriControl\BtnStop.png
# End Source File
# Begin Source File

SOURCE=.\res\MriControl\BtnSwap1.png
# End Source File
# Begin Source File

SOURCE=.\res\MriControl\BtnSwap2.png
# End Source File
# Begin Source File

SOURCE=.\res\Wizard\BtnWizardPreStep.png
# End Source File
# Begin Source File

SOURCE=.\res\Wizard\BtnWizardQuit.png
# End Source File
# Begin Source File

SOURCE=.\res\Wizard\BtnWizardSelectedArrow.png
# End Source File
# Begin Source File

SOURCE=".\res\Wizard\Buton---上一步.png"
# End Source File
# Begin Source File

SOURCE=.\res\Login\Config.png
# End Source File
# Begin Source File

SOURCE=.\res\Login\Config2.png
# End Source File
# Begin Source File

SOURCE=.\res\DialogSkin.png
# End Source File
# Begin Source File

SOURCE=.\res\DialogSkin2.png
# End Source File
# Begin Source File

SOURCE=.\res\Time\Dot.png
# End Source File
# Begin Source File

SOURCE=.\res\LoveFlag.png
# End Source File
# Begin Source File

SOURCE=.\res\MriOption.png
# End Source File
# Begin Source File

SOURCE=".\res\Login\Patient DB.png"
# End Source File
# Begin Source File

SOURCE=".\res\Login\Patient DB2.png"
# End Source File
# Begin Source File

SOURCE=.\res\patient.png
# End Source File
# Begin Source File

SOURCE=.\res\MriControl\plane1.png
# End Source File
# Begin Source File

SOURCE=.\res\MriControl\plane2.png
# End Source File
# Begin Source File

SOURCE=.\res\MriControl\plane3.png
# End Source File
# Begin Source File

SOURCE=".\res\MriControl\Probe Data  1.png"
# End Source File
# Begin Source File

SOURCE=".\res\MriControl\Probe Data  2.png"
# End Source File
# Begin Source File

SOURCE=".\res\MriControl\Probe--Delete.png"
# End Source File
# Begin Source File

SOURCE=".\res\MriControl\Probe--New.png"
# End Source File
# Begin Source File

SOURCE=".\res\Probe--New.png"
# End Source File
# Begin Source File

SOURCE=".\res\MriControl\Probe--Simulate.png"
# End Source File
# Begin Source File

SOURCE=.\res\MriControl\Protocal.png
# End Source File
# Begin Source File

SOURCE=".\res\MriControl\Push--放大.png"
# End Source File
# Begin Source File

SOURCE=".\res\MriControl\Push--滑块.png"
# End Source File
# Begin Source File

SOURCE=".\res\MriControl\Push--缩小.png"
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE=".\res\MriControl\Scan(scan).png"
# End Source File
# Begin Source File

SOURCE=".\res\MriControl\Scan(scan+prescan).png"
# End Source File
# Begin Source File

SOURCE=.\res\series.png
# End Source File
# Begin Source File

SOURCE=.\res\MriControl\Swap1.png
# End Source File
# Begin Source File

SOURCE=.\res\MriControl\Swap2.png
# End Source File
# Begin Source File

SOURCE=.\res\ToolBar\ToolBrowseImage.png
# End Source File
# Begin Source File

SOURCE=.\res\ToolBar\ToolCamera.png
# End Source File
# Begin Source File

SOURCE=.\res\ToolBar\ToolCapture.png
# End Source File
# Begin Source File

SOURCE=.\res\ToolBar\ToolCompareImage.png
# End Source File
# Begin Source File

SOURCE=.\res\ToolBar\ToolMeasure.png
# End Source File
# Begin Source File

SOURCE=.\res\ToolBar\ToolMoveCamera.png
# End Source File
# Begin Source File

SOURCE=.\res\ToolBar\ToolPatientTable.png
# End Source File
# Begin Source File

SOURCE=.\res\Login\TOOLS.png
# End Source File
# Begin Source File

SOURCE=.\res\Login\TOOLS2.png
# End Source File
# Begin Source File

SOURCE=.\res\ToolBar\ToolSystem.png
# End Source File
# Begin Source File

SOURCE=.\res\ToolBar\ToolTarget.png
# End Source File
# Begin Source File

SOURCE=.\res\Login\TPS.png
# End Source File
# Begin Source File

SOURCE=.\res\TPS.png
# End Source File
# Begin Source File

SOURCE=.\res\Login\TPS2.png
# End Source File
# Begin Source File

SOURCE=.\res\TPS\TpsBk.png
# End Source File
# Begin Source File

SOURCE=.\res\TpsBk.png
# End Source File
# Begin Source File

SOURCE=.\res\ImageControl\up.png
# End Source File
# Begin Source File

SOURCE=.\res\View1.png
# End Source File
# Begin Source File

SOURCE=.\res\View2.png
# End Source File
# Begin Source File

SOURCE=.\res\View3.png
# End Source File
# Begin Source File

SOURCE=.\res\View4.png
# End Source File
# Begin Source File

SOURCE=".\res\TPS\保存.png"
# End Source File
# Begin Source File

SOURCE=".\res\ToolBar\测量.png"
# End Source File
# Begin Source File

SOURCE=".\res\TPS\产生冷冻针.png"
# End Source File
# Begin Source File

SOURCE=".\res\TPS\打开.png"
# End Source File
# Begin Source File

SOURCE=".\res\Login\分割线.png"
# End Source File
# Begin Source File

SOURCE=".\res\TPS\模拟效果2.png"
# End Source File
# Begin Source File

SOURCE=".\res\ToolBar\屏幕捕捉.png"
# End Source File
# Begin Source File

SOURCE=".\res\Wizard\失败.png"
# End Source File
# Begin Source File

SOURCE=".\res\ToolBar\图像对比显示.png"
# End Source File
# Begin Source File

SOURCE=".\res\ToolBar\图像显示.png"
# End Source File
# Begin Source File

SOURCE=".\res\Login\退出.png"
# End Source File
# Begin Source File

SOURCE=".\res\ToolBar\相机开启和停止.png"
# End Source File
# Begin Source File

SOURCE=".\res\ToolBar\相机位置移动状态.png"
# End Source File
# Begin Source File

SOURCE=".\res\ToolBar\移动病床.png"
# End Source File
# Begin Source File

SOURCE=".\res\指示灯（BASE）.png"
# End Source File
# Begin Source File

SOURCE=".\res\指示灯（针）.png"
# End Source File
# Begin Source File

SOURCE=".\res\ToolBar\状态切换.png"
# End Source File
# End Target
# End Project
