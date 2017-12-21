# Microsoft Developer Studio Project File - Name="ImgConvertor" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Dynamic-Link Library" 0x0102

CFG=ImgConvertor - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ImgConvertor.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ImgConvertor.mak" CFG="ImgConvertor - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ImgConvertor - Win32 Release" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE "ImgConvertor - Win32 Debug" (based on "Win32 (x86) Dynamic-Link Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ImgConvertor - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MT /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "IMGCONVERTOR_EXPORTS" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "E:\Panasee2.3_Last\Include" /I "E:\Panasee2.3_Last\Include\ImgHeader" /I "E:\Panasee2.3_Last\Include\DcmTk\config\include" /I "E:\Panasee2.3_Last\Include\DcmTk\dcmdata\include" /I "E:\Panasee2.3_Last\Include\DcmTk\dcmnet\include" /I "E:\Panasee2.3_Last\Include\DcmTk\ofstd\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /D "IMGCONVERTOR_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /machine:I386
# ADD LINK32 wsock32.lib netapi32.lib ofstd.lib dcmdata.lib dcmnet.lib /nologo /dll /machine:I386 /nodefaultlib:"msvcrt.lib" /libpath:"E:\Panasee2.3_Last\DcmTk\vc6\lib\Release"
# SUBTRACT LINK32 /force
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy .\Release\ImgConvertor.dll ..\Release\.	copy .\Release\ImgConvertor.lib ..\LibR\.	copy .\DICOM2XINAO.h ..\include\.
# End Special Build Tool

!ELSEIF  "$(CFG)" == "ImgConvertor - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 2
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MTd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "_USRDLL" /D "IMGCONVERTOR_EXPORTS" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "E:\Panasee2.3_Last\Include" /I "E:\Panasee2.3_Last\Include\ImgHeader" /I "E:\Panasee2.3_Last\Include\DcmTk\config\include" /I "E:\Panasee2.3_Last\Include\DcmTk\dcmdata\include" /I "E:\Panasee2.3_Last\Include\DcmTk\dcmnet\include" /I "E:\Panasee2.3_Last\Include\DcmTk\ofstd\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /D "IMGCONVERTOR_EXPORTS" /D "_WINDLL" /D "_AFXDLL" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /dll /debug /machine:I386 /pdbtype:sept
# ADD LINK32 wsock32.lib netapi32.lib ofstd.lib dcmdata.lib dcmnet.lib /nologo /dll /debug /machine:I386 /nodefaultlib:"LIBCMT.lib" /out:"Debug/ImgConvertorD.dll" /pdbtype:sept /libpath:"E:\Panasee2.3_Last\DcmTk\vc6\lib\Debug"
# SUBTRACT LINK32 /force
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy .\Debug\ImgConvertorD.dll ..\BinD\.	copy .\Debug\ImgConvertorD.lib ..\LibD\.	copy .\DICOM2XINAO.h ..\include\.
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "ImgConvertor - Win32 Release"
# Name "ImgConvertor - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DICOM2XINAO.cpp
# End Source File
# Begin Source File

SOURCE=.\ImgConvertor.cpp
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DICOM2XINAO.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# End Group
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# End Target
# End Project
