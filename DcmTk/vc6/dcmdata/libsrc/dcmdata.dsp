# Microsoft Developer Studio Project File - Name="dcmdata" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=dcmdata - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "dcmdata.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "dcmdata.mak" CFG="dcmdata - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "dcmdata - Win32 MinSizeRel" (based on "Win32 (x86) Static Library")
!MESSAGE "dcmdata - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "dcmdata - Win32 RelWithDebInfo" (based on "Win32 (x86) Static Library")
!MESSAGE "dcmdata - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "dcmdata - Win32 MinSizeRel"

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
# ADD CPP /nologo /MD /W3 /GR /GX /O1 /I "..\..\..\config\include" /I "..\..\..\dcmdata\include" /I "..\..\..\ofstd\include" /D "NDEBUG" /D "_ATL_DLL" /D "WIN32" /D "_WINDOWS" /D "_USRDLL" /D "_MBCS" /D "_REENTRANT" /D CMAKE_INTDIR=\"MinSizeRel\" /FD /Zm1000  /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "..\..\..\config\include" /i "..\..\..\dcmdata\include" /i "..\..\..\ofstd\include" /d "NDEBUG" /d "_REENTRANT" REL
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

!ELSEIF  "$(CFG)" == "dcmdata - Win32 Release"

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
# ADD CPP /nologo /MDd /W3 /GR /GX /O2 /Ob2 /I "..\..\..\config\include" /I "..\..\..\dcmdata\include" /I "..\..\..\ofstd\include" /D "NDEBUG" /D "WIN32" /D "_LIB" /D "_WINDOWS" /D "_MBCS" /D "_REENTRANT" /D CMAKE_INTDIR=\"Release\" /FD /Zm1000  /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "..\..\..\config\include" /i "..\..\..\dcmdata\include" /i "..\..\..\ofstd\include" /d "NDEBUG" /d "_REENTRANT"
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

!ELSEIF  "$(CFG)" == "dcmdata - Win32 RelWithDebInfo"

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
# ADD CPP /nologo /MD /W3 /GR /GX /Zi /O2 /I "..\..\..\config\include" /I "..\..\..\dcmdata\include" /I "..\..\..\ofstd\include" /D "NDEBUG" /D "WIN32" /D "_LIB" /D "_WINDOWS" /D "_MBCS" /D "_REENTRANT" /D CMAKE_INTDIR=\"RelWithDebInfo\" /FD /Zm1000  /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "..\..\..\config\include" /i "..\..\..\dcmdata\include" /i "..\..\..\ofstd\include" /d "NDEBUG" /d "_REENTRANT"
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

!ELSEIF  "$(CFG)" == "dcmdata - Win32 Debug"

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
# ADD CPP /nologo /MT /W3 /GR /GX /Zi /Od /I "..\..\..\config\include" /I "..\..\..\dcmdata\include" /I "..\..\..\ofstd\include" /D "_DEBUG" /D "WIN32" /D "_LIB" /D "_WINDOWS" /D "_MBCS" /D "_REENTRANT" /D CMAKE_INTDIR=\"Debug\" /FD /GZ   /Zm1000  /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i "..\..\..\config\include" /i "..\..\..\dcmdata\include" /i "..\..\..\ofstd\include" /d "_DEBUG" /d "_REENTRANT"
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

# Name "dcmdata - Win32 MinSizeRel"
# Name "dcmdata - Win32 Release"
# Name "dcmdata - Win32 RelWithDebInfo"
# Name "dcmdata - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\cmdlnarg.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcbytstr.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcchrstr.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dccodec.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcdatset.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcddirif.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcdebug.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcdicdir.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcdicent.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcdict.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcdictzz.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcdirrec.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcelem.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcerror.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcfilefo.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dchashdi.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcistrma.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcistrmb.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcistrmf.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcistrmz.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcitem.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dclist.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcmetinf.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcobject.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcostrma.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcostrmb.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcostrmf.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcostrmz.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcpcache.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcpixel.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcpixseq.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcpxitem.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcrleccd.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcrlecce.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcrlecp.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcrledrg.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcrleerg.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcrlerp.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcsequen.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcstack.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcswap.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dctag.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dctagkey.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dctypes.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcuid.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcvm.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcvr.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcvrae.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcvras.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcvrat.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcvrcs.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcvrda.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcvrds.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcvrdt.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcvrfd.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcvrfl.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcvris.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcvrlo.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcvrlt.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcvrobow.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcvrof.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcvrpn.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcvrpobw.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcvrsh.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcvrsl.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcvrss.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcvrst.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcvrtm.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcvrui.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcvrul.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcvrulup.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcvrus.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcvrut.cxx
# ADD CPP /TP
# End Source File
# Begin Source File

SOURCE=..\..\..\dcmdata\libsrc\dcxfer.cxx
# ADD CPP /TP
# End Source File
# End Group
# End Target
# End Project
