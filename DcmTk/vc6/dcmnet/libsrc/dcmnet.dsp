# Microsoft Developer Studio Project File - Name="dcmnet" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=dcmnet - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "dcmnet.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "dcmnet.mak" CFG="dcmnet - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "dcmnet - Win32 MinSizeRel" (based on "Win32 (x86) Static Library")
!MESSAGE "dcmnet - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "dcmnet - Win32 RelWithDebInfo" (based on "Win32 (x86) Static Library")
!MESSAGE "dcmnet - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "dcmnet - Win32 MinSizeRel"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MinSizeRel"
# PROP BASE Intermediate_Dir "MinSizeRel"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\lib\MinSizeRel"
# PROP Intermediate_Dir "MinSizeRel"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_USRDLL" /D "_ATL_DLL" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O1 /I "..\..\..\config\include" /I "..\..\..\dcmnet\include" /I "..\..\..\ofstd\include" /I "..\..\..\dcmdata\include" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_MBCS" /D "_REENTRANT" /D CMAKE_INTDIR=\"MinSizeRel\" /FD /Zm1000  /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "..\..\..\config\include" /i "..\..\..\dcmnet\include" /i "..\..\..\ofstd\include" /i "..\..\..\dcmdata\include" /d "NDEBUG" /d "_REENTRANT" REL
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Special Build Tool
SOURCE="$(InputPath)"
PreLink_Cmds="D:\Program Files\CMake 2.8\bin\cmake.exe" -E make_directory E:/DcmTK/dcmtk-3.5.4/lib/MinSizeRel
# End Special Build Tool

!ELSEIF  "$(CFG)" == "dcmnet - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\lib\Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /D "WIN32" /D "NDEBUG" /D "_LIB" /FD /c
# ADD CPP /nologo /MDd /W3 /GR /GX /O2 /Ob2 /I "..\..\..\config\include" /I "..\..\..\dcmnet\include" /I "..\..\..\ofstd\include" /I "..\..\..\dcmdata\include" /D "NDEBUG" /D "WIN32" /D "_LIB" /D "_WINDOWS" /D "_MBCS" /D "_REENTRANT" /D CMAKE_INTDIR=\"Release\" /FD /Zm1000  /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "..\..\..\config\include" /i "..\..\..\dcmnet\include" /i "..\..\..\ofstd\include" /i "..\..\..\dcmdata\include" /d "NDEBUG" /d "_REENTRANT"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Special Build Tool
SOURCE="$(InputPath)"
PreLink_Cmds="D:\Program Files\CMake 2.8\bin\cmake.exe" -E make_directory E:/DcmTK/dcmtk-3.5.4/lib/Release
# End Special Build Tool

!ELSEIF  "$(CFG)" == "dcmnet - Win32 RelWithDebInfo"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "RelWithDebInfo"
# PROP BASE Intermediate_Dir "RelWithDebInfo"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\lib\RelWithDebInfo"
# PROP Intermediate_Dir "RelWithDebInfo"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /D "WIN32" /D "NDEBUG" /D "_LIB" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /Zi /O2 /I "..\..\..\config\include" /I "..\..\..\dcmnet\include" /I "..\..\..\ofstd\include" /I "..\..\..\dcmdata\include" /D "NDEBUG" /D "WIN32" /D "_LIB" /D "_WINDOWS" /D "_MBCS" /D "_REENTRANT" /D CMAKE_INTDIR=\"RelWithDebInfo\" /FD /Zm1000  /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "..\..\..\config\include" /i "..\..\..\dcmnet\include" /i "..\..\..\ofstd\include" /i "..\..\..\dcmdata\include" /d "NDEBUG" /d "_REENTRANT"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Special Build Tool
SOURCE="$(InputPath)"
PreLink_Cmds="D:\Program Files\CMake 2.8\bin\cmake.exe" -E make_directory E:/DcmTK/dcmtk-3.5.4/lib/RelWithDebInfo
# End Special Build Tool

!ELSEIF  "$(CFG)" == "dcmnet - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\lib\Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /D "WIN32" /D "_DEBUG" /D "_LIB" /FD /c
# ADD CPP /nologo /MT /W3 /GR /GX /Zi /Od /I "..\..\..\config\include" /I "..\..\..\dcmnet\include" /I "..\..\..\ofstd\include" /I "..\..\..\dcmdata\include" /D "_DEBUG" /D "WIN32" /D "_LIB" /D "_WINDOWS" /D "_MBCS" /D "_REENTRANT" /D CMAKE_INTDIR=\"Debug\" /FR /FD /GZ   /Zm1000  /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i "..\..\..\config\include" /i "..\..\..\dcmnet\include" /i "..\..\..\ofstd\include" /i "..\..\..\dcmdata\include" /d "_DEBUG" /d "_REENTRANT"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo
# Begin Special Build Tool
SOURCE="$(InputPath)"
PreLink_Cmds="D:\Program Files\CMake 2.8\bin\cmake.exe" -E make_directory E:/DcmTK/dcmtk-3.5.4/lib/Debug
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "dcmnet - Win32 MinSizeRel"
# Name "dcmnet - Win32 Release"
# Name "dcmnet - Win32 RelWithDebInfo"
# Name "dcmnet - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\dcmnet\libsrc\assoc.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmnet\libsrc\cond.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmnet\libsrc\dcasccff.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmnet\libsrc\dcasccfg.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmnet\libsrc\dccfenmp.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmnet\libsrc\dccfpcmp.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmnet\libsrc\dccfprmp.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmnet\libsrc\dccfrsmp.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmnet\libsrc\dccftsmp.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmnet\libsrc\dccfuidh.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmnet\libsrc\dcmlayer.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmnet\libsrc\dcmtrans.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmnet\libsrc\dcompat.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmnet\libsrc\dimcancl.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmnet\libsrc\dimcmd.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmnet\libsrc\dimdump.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmnet\libsrc\dimecho.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmnet\libsrc\dimfind.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmnet\libsrc\dimget.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmnet\libsrc\dimmove.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmnet\libsrc\dimse.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmnet\libsrc\dimstore.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmnet\libsrc\diutil.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmnet\libsrc\dul.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmnet\libsrc\dulconst.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmnet\libsrc\dulextra.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmnet\libsrc\dulfsm.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmnet\libsrc\dulparse.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmnet\libsrc\dulpres.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmnet\libsrc\extneg.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmnet\libsrc\lst.cxx
# ADD CPP /TP
# End Source File
# End Group
# End Target
# End Project
