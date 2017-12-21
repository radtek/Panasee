# Microsoft Developer Studio Project File - Name="Tps" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=Tps - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Tps.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Tps.mak" CFG="Tps - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Tps - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "Tps - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Tps - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\include\btnst" /I "..\include\xtk" /I "..\include\UtilDll" /I "..\include" /I "..\include\panaskin" /I "..\include\PngLib" /I "..\include\vtk" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "_XTP_STATICLINK" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /i "..\include\xtk" /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 trimath.lib opengl32.lib glu32.lib vtkexpat.lib vtksys.lib vtkCommon.lib vtkftgl.lib vtkzlib.lib vtkfreetype.lib vtkftgl.lib vtkMFC.lib vtkWidgets.lib vtkHybrid.lib vtkRendering.lib vtkio.lib vtkGraphics.lib vtkImaging.lib vtkpng.lib vtklibxml2.lib vtkjpeg.lib vtktiff.lib vtkFiltering.lib vtkDICOMParser.lib vtkVolumeRendering.lib ButtonST.lib MRISeriesDLL.lib Netapi32.lib Version.lib vImage.lib /nologo /subsystem:windows /dll /machine:I386 /libpath:"..\libR" /libpath:"..\libR\vtk"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy release\Tps.dll ..\Release	copy release\Tps.lib ..\libR
# End Special Build Tool

!ELSEIF  "$(CFG)" == "Tps - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\include\btnst" /I "..\include\xtk" /I "..\include\UtilDll" /I "..\include" /I "..\include\panaskin" /I "..\include\PngLib" /I "..\include\vtk" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "_XTP_STATICLINK" /FR /FD /GZ /Zm1000 /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /i "..\include\xtk" /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 trimathD.lib opengl32.lib glu32.lib vtkexpat.lib vtksys.lib vtkCommon.lib vtkftgl.lib vtkzlib.lib vtkfreetype.lib vtkftgl.lib vtkMFC.lib vtkWidgets.lib vtkHybrid.lib vtkRendering.lib vtkio.lib vtkGraphics.lib vtkImaging.lib vtkpng.lib vtklibxml2.lib vtkjpeg.lib vtktiff.lib vtkFiltering.lib vtkDICOMParser.lib vtkVolumeRendering.lib ButtonSTD.lib MRISeriesDLLD.lib LicenseApiD.lib EncodeD.lib GetHardDeviceInfoD.lib Netapi32.lib LicenseLibD.lib Version.lib vImageD.lib MRICalibrationDllD.lib /nologo /stack:0x3d0900 /subsystem:windows /dll /debug /machine:I386 /out:"Debug/TpsD.dll" /pdbtype:sept /libpath:"..\libd" /libpath:"..\libd\vtk"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy debug\TpsD.dll ..\binD	copy TpsFrmMgr.h ..\include	copy debug\TpsD.lib ..\libD
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "Tps - Win32 Release"
# Name "Tps - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ConfigMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\ControlDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CREDITSTATIC.CPP
# End Source File
# Begin Source File

SOURCE=.\CustomItems.cpp
# End Source File
# Begin Source File

SOURCE=.\DisplayDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgAutoTrace.cpp
# End Source File
# Begin Source File

SOURCE=.\DoseDistribute.cpp
# End Source File
# Begin Source File

SOURCE=.\EvaluateDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ExtendVTKClass.cpp
# End Source File
# Begin Source File

SOURCE=.\GalilIceBallDistribute.cpp
# End Source File
# Begin Source File

SOURCE=.\GlobalManager.cpp
# End Source File
# Begin Source File

SOURCE=.\IceBallDistribute.cpp
# End Source File
# Begin Source File

SOURCE=.\MeshWriter.cxx
# End Source File
# Begin Source File

SOURCE=.\Modal.cpp
# End Source File
# Begin Source File

SOURCE=.\ModalContour.cpp
# End Source File
# Begin Source File

SOURCE=.\ModalDrag.cpp
# End Source File
# Begin Source File

SOURCE=.\ModalPropDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ModalSeg.cpp
# End Source File
# Begin Source File

SOURCE=.\ModalVolume.cpp
# End Source File
# Begin Source File

SOURCE=.\MPRView.cpp
# End Source File
# Begin Source File

SOURCE=.\OleDropTargetEx.cpp
# End Source File
# Begin Source File

SOURCE=.\Points2Mesh.cpp
# End Source File
# Begin Source File

SOURCE=.\ProbeControlDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\ProbePropDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Prop3DTree.cpp
# End Source File
# Begin Source File

SOURCE=.\SaveActor2Obj.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\Tps.cpp
# End Source File
# Begin Source File

SOURCE=.\Tps.rc
# End Source File
# Begin Source File

SOURCE=.\TPS2DView.cpp
# End Source File
# Begin Source File

SOURCE=.\TPS3DView.cpp
# End Source File
# Begin Source File

SOURCE=.\TpsCryoProbe.cpp
# End Source File
# Begin Source File

SOURCE=.\TPSFrmMgr.cpp
# End Source File
# Begin Source File

SOURCE=.\TpsTool.cpp
# End Source File
# Begin Source File

SOURCE=.\TracerPropDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\Volume.cpp
# End Source File
# Begin Source File

SOURCE=.\vtkPowerCrustSurfaceReconstruction.cxx
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\ConfigMgr.h
# End Source File
# Begin Source File

SOURCE=.\ControlDlg.h
# End Source File
# Begin Source File

SOURCE=.\DisplayDlg.h
# End Source File
# Begin Source File

SOURCE=.\DlgAutoTrace.h
# End Source File
# Begin Source File

SOURCE=.\DoseDistribute.h
# End Source File
# Begin Source File

SOURCE=.\EvaluateDlg.h
# End Source File
# Begin Source File

SOURCE=.\GalilIceBallDistribute.h
# End Source File
# Begin Source File

SOURCE=.\GlobalManager.h
# End Source File
# Begin Source File

SOURCE=.\IceBallDistribute.h
# End Source File
# Begin Source File

SOURCE=.\MeshWriter.h
# End Source File
# Begin Source File

SOURCE=.\Modal.h
# End Source File
# Begin Source File

SOURCE=.\ModalContour.h
# End Source File
# Begin Source File

SOURCE=.\ModalDrag.h
# End Source File
# Begin Source File

SOURCE=.\ModalPropDlg.h
# End Source File
# Begin Source File

SOURCE=.\ModalSeg.h
# End Source File
# Begin Source File

SOURCE=.\ModalVolume.h
# End Source File
# Begin Source File

SOURCE=.\MPRView.h
# End Source File
# Begin Source File

SOURCE=.\Points2Mesh.h
# End Source File
# Begin Source File

SOURCE=.\ProbeControlDlg.h
# End Source File
# Begin Source File

SOURCE=.\ProbePropDlg.h
# End Source File
# Begin Source File

SOURCE=.\Prop3DTree.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SaveActor2Obj.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\TPS2DView.h
# End Source File
# Begin Source File

SOURCE=.\TPS3DView.h
# End Source File
# Begin Source File

SOURCE=.\TpsCryoProbe.h
# End Source File
# Begin Source File

SOURCE=.\TPSFrmMgr.h
# End Source File
# Begin Source File

SOURCE=.\TpsTool.h
# End Source File
# Begin Source File

SOURCE=.\TracerPropDlg.h
# End Source File
# Begin Source File

SOURCE=.\Volume.h
# End Source File
# Begin Source File

SOURCE=.\vtkPowerCrustSurfaceReconstruction.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\019.bmp
# End Source File
# Begin Source File

SOURCE=.\res\030307system058.bmp
# End Source File
# Begin Source File

SOURCE=.\res\030307system1009.bmp
# End Source File
# Begin Source File

SOURCE=.\res\030307system233.bmp
# End Source File
# Begin Source File

SOURCE=.\res\030307system276.bmp
# End Source File
# Begin Source File

SOURCE=.\res\030307system669.bmp
# End Source File
# Begin Source File

SOURCE=.\res\bg.bmp
# End Source File
# Begin Source File

SOURCE=".\res\bluewhales 039.ico"
# End Source File
# Begin Source File

SOURCE=".\res\bluewhales 047.ico"
# End Source File
# Begin Source File

SOURCE=.\res\bowei.bmp
# End Source File
# Begin Source File

SOURCE=".\res\Core IP theme Icon 14.ico"
# End Source File
# Begin Source File

SOURCE=.\res\ico00001.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00002.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00003.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00004.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00005.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00006.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00007.ico
# End Source File
# Begin Source File

SOURCE=.\res\ico00008.ico
# End Source File
# Begin Source File

SOURCE=.\res\icon1.ico
# End Source File
# Begin Source File

SOURCE=.\res\img_restree.bmp
# End Source File
# Begin Source File

SOURCE=.\res\LedOff.ico
# End Source File
# Begin Source File

SOURCE=.\res\LedOn.ico
# End Source File
# Begin Source File

SOURCE=.\res\li.bmp
# End Source File
# Begin Source File

SOURCE=.\res\liu.bmp
# End Source File
# Begin Source File

SOURCE=.\res\measure.bmp
# End Source File
# Begin Source File

SOURCE=.\res\MEASURE2.bmp
# End Source File
# Begin Source File

SOURCE=.\res\ning.bmp
# End Source File
# Begin Source File

SOURCE=.\res\pro3.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PropertyIcons.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Tps.rc2
# End Source File
# Begin Source File

SOURCE=.\res\TPWare.ico
# End Source File
# Begin Source File

SOURCE=.\res\wei.bmp
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
