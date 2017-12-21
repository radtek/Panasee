# Microsoft Developer Studio Project File - Name="DICOM" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Static Library" 0x0104

CFG=DICOM - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "DICOM.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DICOM.mak" CFG="DICOM - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DICOM - Win32 Release" (based on "Win32 (x86) Static Library")
!MESSAGE "DICOM - Win32 Debug" (based on "Win32 (x86) Static Library")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DICOM - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release\library"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_MBCS" /D "_LIB" /YX /FD /c
# ADD CPP /nologo /MD /W3 /GX /O2 /I "..\include" /D "NDEBUG" /D "WIN32" /D "_LIB" /D "JAS_WIN_MSVC_BUILD" /FD /c
# SUBTRACT CPP /YX
# ADD BASE RSC /l 0x804 /d "NDEBUG"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Release\DICOM.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy .\Release\DICOM.lib ..\LibR\.
# End Special Build Tool

!ELSEIF  "$(CFG)" == "DICOM - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_MBCS" /D "_LIB" /YX /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /I "..\include" /D "_DEBUG" /D "WINDOWS" /D "WIN32" /D "_LIB" /D "JAS_WIN_MSVC_BUILD" /Fr /YX /FD /GZ /c
# SUBTRACT CPP /WX
# ADD BASE RSC /l 0x804 /d "_DEBUG"
# ADD RSC /l 0x804 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LIB32=link.exe -lib
# ADD BASE LIB32 /nologo
# ADD LIB32 /nologo /out:"Debug\DICOMD.lib"
# Begin Special Build Tool
SOURCE="$(InputPath)"
PostBuild_Cmds=copy .\array.thh ..\include\DICOM\.	copy .\array.tcc ..\include\DICOM\.	copy .\pqueue.thh ..\include\DICOM\.	copy .\pqueue.tcc ..\include\DICOM\.	copy .\farray.thh ..\include\DICOM\.	copy .\buffer.thh ..\include\DICOM\.	copy .\endian.hpd ..\include\DICOM\.	copy .\Debug\DICOMD.lib ..\LibD\.	copy .\*.hpp ..\include\DICOM\.	copy .\*.h ..\include\DICOM\.
# End Special Build Tool

!ENDIF 

# Begin Target

# Name "DICOM - Win32 Release"
# Name "DICOM - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\aaac.cxx
# End Source File
# Begin Source File

SOURCE=.\aarj.cxx
# End Source File
# Begin Source File

SOURCE=.\aarq.cxx
# End Source File
# Begin Source File

SOURCE=.\buffer.cxx
# End Source File
# Begin Source File

SOURCE=.\CABS.CXX
# End Source File
# Begin Source File

SOURCE=.\CDICOMDIR.cpp
# End Source File
# Begin Source File

SOURCE=.\COMP.CPP
# End Source File
# Begin Source File

SOURCE=.\DC3.Dictionary.cpp
# End Source File
# Begin Source File

SOURCE=.\dcmprint.cpp
# End Source File
# Begin Source File

SOURCE=.\deivr.cxx
# End Source File
# Begin Source File

SOURCE=.\DICOMDIR.cpp
# End Source File
# Begin Source File

SOURCE=.\dimsec.cxx
# End Source File
# Begin Source File

SOURCE=.\dimsen.cxx
# End Source File
# Begin Source File

SOURCE=.\endian.cxx
# End Source File
# Begin Source File

SOURCE=.\filepdu.cxx
# End Source File
# Begin Source File

SOURCE=.\IODBASE.CPP
# End Source File
# Begin Source File

SOURCE=.\MyQueryRetrieve.cpp
# End Source File
# Begin Source File

SOURCE=.\MyQueryRetrievePDU.cpp
# End Source File
# Begin Source File

SOURCE=.\pdata.cxx
# End Source File
# Begin Source File

SOURCE=.\pdu.cxx
# End Source File
# Begin Source File

SOURCE=.\PrintImage.cpp
# End Source File
# Begin Source File

SOURCE=.\printsop.cxx
# End Source File
# Begin Source File

SOURCE=.\QRParam.cpp
# End Source File
# Begin Source File

SOURCE=.\qrsop.cxx
# End Source File
# Begin Source File

SOURCE=.\Query.cpp
# End Source File
# Begin Source File

SOURCE=.\QueryInformation.cpp
# End Source File
# Begin Source File

SOURCE=.\RetrieveImage.cpp
# End Source File
# Begin Source File

SOURCE=.\rtc.cxx
# End Source File
# Begin Source File

SOURCE=.\safemem.cxx
# End Source File
# Begin Source File

SOURCE=.\socket.cxx
# End Source File
# Begin Source File

SOURCE=.\storage.cxx
# End Source File
# Begin Source File

SOURCE=.\trnsyn.cxx
# End Source File
# Begin Source File

SOURCE=.\TVR.CXX
# End Source File
# Begin Source File

SOURCE=.\uniq.cxx
# End Source File
# Begin Source File

SOURCE=.\util.cxx
# End Source File
# Begin Source File

SOURCE=.\verify.cxx
# End Source File
# Begin Source File

SOURCE=.\VRREF.CXX
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\aaac.hpp
# End Source File
# Begin Source File

SOURCE=.\aarj.hpp
# End Source File
# Begin Source File

SOURCE=.\aarq.hpp
# End Source File
# Begin Source File

SOURCE=.\base.hpp
# End Source File
# Begin Source File

SOURCE=.\CCOC.HPP
# End Source File
# Begin Source File

SOURCE=.\cctypes.h
# End Source File
# Begin Source File

SOURCE=.\CDICOMDIR.hpp
# End Source File
# Begin Source File

SOURCE=.\COMP.HPP
# End Source File
# Begin Source File

SOURCE=.\constant.h
# End Source File
# Begin Source File

SOURCE=.\deivr.hpp
# End Source File
# Begin Source File

SOURCE=.\dicom.hpp
# End Source File
# Begin Source File

SOURCE=.\DICOMDIR.h
# End Source File
# Begin Source File

SOURCE=.\DICOMEnums.h
# End Source File
# Begin Source File

SOURCE=.\dimsec.hpp
# End Source File
# Begin Source File

SOURCE=.\dimsen.hpp
# End Source File
# Begin Source File

SOURCE=.\endian.hpp
# End Source File
# Begin Source File

SOURCE=.\errdescr.hpp
# End Source File
# Begin Source File

SOURCE=.\IOD.HPP
# End Source File
# Begin Source File

SOURCE=.\IODBASE.HPP
# End Source File
# Begin Source File

SOURCE=.\MyQueryRetrieve.h
# End Source File
# Begin Source File

SOURCE=.\MyQueryRetrievePDU.h
# End Source File
# Begin Source File

SOURCE=.\pdata.hpp
# End Source File
# Begin Source File

SOURCE=.\pdu.hpp
# End Source File
# Begin Source File

SOURCE=.\PrintImage.h
# End Source File
# Begin Source File

SOURCE=.\printsop.hpp
# End Source File
# Begin Source File

SOURCE=.\PrnParam.h
# End Source File
# Begin Source File

SOURCE=.\QRParam.h
# End Source File
# Begin Source File

SOURCE=.\qrsop.hpp
# End Source File
# Begin Source File

SOURCE=.\Query.h
# End Source File
# Begin Source File

SOURCE=.\QueryInformation.h
# End Source File
# Begin Source File

SOURCE=.\Regexp.h
# End Source File
# Begin Source File

SOURCE=.\RetrieveImage.h
# End Source File
# Begin Source File

SOURCE=.\rtc.hpp
# End Source File
# Begin Source File

SOURCE=.\safemem.h
# End Source File
# Begin Source File

SOURCE=.\ServiceQueue.h
# End Source File
# Begin Source File

SOURCE=.\socket.hpp
# End Source File
# Begin Source File

SOURCE=.\storage.hpp
# End Source File
# Begin Source File

SOURCE=.\TOKEN.H
# End Source File
# Begin Source File

SOURCE=.\TVR.HPP
# End Source File
# Begin Source File

SOURCE=.\unixsock.h
# End Source File
# Begin Source File

SOURCE=.\util.h
# End Source File
# Begin Source File

SOURCE=.\verify.hpp
# End Source File
# Begin Source File

SOURCE=.\version.h
# End Source File
# Begin Source File

SOURCE=.\VRREF.HPP
# End Source File
# End Group
# End Target
# End Project
