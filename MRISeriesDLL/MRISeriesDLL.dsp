# Microsoft Developer Studio Project File - Name="MRISeriesDLL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=MRISeriesDLL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "MRISeriesDLL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "MRISeriesDLL.mak" CFG="MRISeriesDLL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "MRISeriesDLL - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "MRISeriesDLL - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "MRISeriesDLL - Win32 Release"

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
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\Include" /I "..\Include\Modeling\Common" /I "..\Include\Modeling\Model" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "_BUILD_MRISERIES_DLL" /FR /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 GeometryDll.lib Trimath.lib ImriDataAccess.lib /nologo /subsystem:windows /dll /machine:I386 /libpath:".\Orient_DataAccess" /libpath:"..\LibR"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy .\Release\MRISeriesDLL.dll ..\Release	copy .\Release\MRISeriesDLL.lib ..\LibR	copy .\MRISeries.h ..\Include	copy .\MRIImage.h ..\Include
# End Special Build Tool

!ELSEIF  "$(CFG)" == "MRISeriesDLL - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\Include" /I "..\Include\ImgHeader" /I "..\Include\Modeling\Common" /I "..\Include\Modeling\Model" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /D "_BUILD_MRISERIES_DLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x409 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 GeometryDllD.lib TrimathD.lib ImriDataAccessD.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"Debug/MRISeriesDLLD.dll" /pdbtype:sept /libpath:".\Orient_DataAccess" /libpath:"..\LibD"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy .\Debug\MRISeriesDLLD.dll ..\BinD	copy .\Debug\MRISeriesDLLD.lib ..\LibD	copy .\MRISeries.h ..\Include	copy .\MRIImage.h ..\Include	copy .\LUT.h ..\Include\.	copy .\MultiUsedObject.h ..\include	copy .\buffer.h ..\include
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "MRISeriesDLL - Win32 Release"
# Name "MRISeriesDLL - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DataProcessor.cpp
# End Source File
# Begin Source File

SOURCE=.\LUT.cpp
# End Source File
# Begin Source File

SOURCE=.\MRIImage.cpp
# End Source File
# Begin Source File

SOURCE=.\MRISeries.cpp
# End Source File
# Begin Source File

SOURCE=.\MRISeriesDLL.cpp
# End Source File
# Begin Source File

SOURCE=.\MRISeriesDLL.def
# End Source File
# Begin Source File

SOURCE=.\MRISeriesDLL.rc
# End Source File
# Begin Source File

SOURCE=.\MultiUsedObject.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DataProcessor.h
# End Source File
# Begin Source File

SOURCE=.\LUT.h
# End Source File
# Begin Source File

SOURCE=.\MRIImage.h
# End Source File
# Begin Source File

SOURCE=.\MRISeries.h
# End Source File
# Begin Source File

SOURCE=.\MultiUsedObject.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\MRISeriesDLL.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
