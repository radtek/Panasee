# Microsoft Developer Studio Project File - Name="PatInfoMrg" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=PatInfoMrg - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "PatInfoMrg.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "PatInfoMrg.mak" CFG="PatInfoMrg - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "PatInfoMrg - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "PatInfoMrg - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "PatInfoMrg - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "../include" /I "../include/UI1" /I "../include/Panaskin" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_AFXEXT" /D "_WINDLL" /FD /c
# SUBTRACT CPP /YX /Yc /Yu
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /machine:I386
# ADD LINK32 MRISeriesDLL.lib ImageDisplay.lib Shlwapi.lib /nologo /subsystem:windows /dll /machine:I386 /libpath:"../LibR"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy .\Release\PatInfoMrg.lib ..\LibR\.	copy .\Release\PatInfoMrg.dll ..\Release\.
# End Special Build Tool

!ELSEIF  "$(CFG)" == "PatInfoMrg - Win32 Debug"

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
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "../include" /I "../include/UI1" /I "../include/Panaskin" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_WINDLL" /D "_AFXDLL" /D "_MBCS" /D "_AFXEXT" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 MRISeriesDLLD.lib ImageDisplayD.lib Shlwapi.lib /nologo /subsystem:windows /dll /debug /machine:I386 /out:"./Debug/PatInfoMrgD.dll" /pdbtype:sept /libpath:"../LibD"
# SUBTRACT LINK32 /pdb:none
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy .\Debug\PatInfoMrgD.lib ..\LibD\.	copy .\Debug\PatInfoMrgD.dll ..\BinD\.	copy .\ImriDBI.h ..\Include\.	copy .\PatientInfoDlg.h ..\Include\.	copy .\PatientInfoMrgDlg.h ..\Include\.	copy .\DatabaseConfig.h ..\Include\.	copy .\PatInfoEditorDlg.h ..\include\.
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "PatInfoMrg - Win32 Release"
# Name "PatInfoMrg - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DatabaseConfig.cpp
# End Source File
# Begin Source File

SOURCE=.\DlgMRI2DDisplay.cpp
# End Source File
# Begin Source File

SOURCE=.\ImriDBI.cpp
# End Source File
# Begin Source File

SOURCE=.\PatientInfoDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PatientInfoMrgDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PatInfoEditorDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PatInfoMrg.cpp
# End Source File
# Begin Source File

SOURCE=.\PatInfoMrg.def
# End Source File
# Begin Source File

SOURCE=.\PatInfoMrg.rc
# End Source File
# Begin Source File

SOURCE=.\Registry.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DatabaseConfig.h
# End Source File
# Begin Source File

SOURCE=.\DlgMRI2DDisplay.h
# End Source File
# Begin Source File

SOURCE=.\ImriDBI.h
# End Source File
# Begin Source File

SOURCE=.\PatientInfoDlg.h
# End Source File
# Begin Source File

SOURCE=.\PatientInfoMrgDlg.h
# End Source File
# Begin Source File

SOURCE=.\PatInfoEditorDlg.h
# End Source File
# Begin Source File

SOURCE=.\Registry.h
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

SOURCE=.\02_top.bmp
# End Source File
# Begin Source File

SOURCE=.\bitmap1.bmp
# End Source File
# Begin Source File

SOURCE=.\res\PatInfoMrg.rc2
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
