# Microsoft Developer Studio Project File - Name="pcbuilder" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# CM DSP Header file
# This file is read by the build system of cm, and is used as the top part of
# a microsoft project dsp header file
# IF this is in a dsp file, then it is not the header, but has
# already been used, so do not edit here...

# variables to REPLACE
# 
#  /I "..\..\..\config\include" /I "..\..\..\dcmdata\include" /I "..\..\..\ofstd\include" == include path
# ..\..\..\bin == override in output directory
#   == compiler defines
# dcmcrle  == name of output library
#  /LIBPATH:..\..\..\lib\$(INTDIR)  /LIBPATH:..\..\..\lib\  /LIBPATH:..\..\..\bin\$(INTDIR)  /LIBPATH:..\..\..\bin\  /LIBPATH:..\$(INTDIR)  /LIBPATH:..\  /LIBPATH:..\..\ofstd\$(INTDIR)  /LIBPATH:..\..\ofstd\  netapi32.lib wsock32.lib ofstd.lib dcmdata.lib  /STACK:10000000 /machine:X86 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib  == libraries linked in 
# TARGTYPE "Win32 (x86) Application" 0x0103

CFG=dcmcrle - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "dcmcrle.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "dcmcrle.mak" CFG="dcmcrle - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "dcmcrle - Win32 MinSizeRel" (based on "Win32 (x86) Application")
!MESSAGE "dcmcrle - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "dcmcrle - Win32 RelWithDebInfo" (based on "Win32 (x86) Application")
!MESSAGE "dcmcrle - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "dcmcrle - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\bin\Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo  /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /FD /c
# ADD CPP /nologo  /D "WIN32"  /D "NDEBUG" /D "_CONSOLE" /FD /c
# ADD CPP  /I "..\..\..\config\include" /I "..\..\..\dcmdata\include" /I "..\..\..\ofstd\include"   
# ADD CPP  /DWIN32 /D_WINDOWS /W3 /Zm1000 /GX /GR /D "_MBCS"  -D_REENTRANT
# ADD CPP /MD /O2 /Ob2 /D NDEBUG -DCMAKE_INTDIR=\"Release\"  
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC  /I "..\..\..\config\include" /I "..\..\..\dcmdata\include" /I "..\..\..\ofstd\include" /l 0x409 /d "NDEBUG"
# ADD RSC   -D_REENTRANT
# ADD RSC  
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32  /nologo /subsystem:console /machine:I386 /IGNORE:4089
# ADD LINK32  /nologo /subsystem:console /machine:I386 /IGNORE:4089 /version:0.0
# ADD LINK32 /out:"..\..\..\bin\Release\dcmcrle.exe" /implib:..\..\..\bin\Release\dcmcrle.lib
# ADD LINK32 /LIBPATH:..\$(IntDir) /LIBPATH:..
# ADD LINK32 /LIBPATH:..\..\ofstd\$(IntDir) /LIBPATH:..\..\ofstd
# ADD LINK32 /LIBPATH:..\..\..\lib\Release\$(IntDir) /LIBPATH:..\..\..\lib\Release
# ADD LINK32 netapi32.lib
# ADD LINK32 wsock32.lib
# ADD LINK32 ..\..\..\lib\Release\ofstd.lib
# ADD LINK32 ..\..\..\lib\Release\dcmdata.lib
# ADD LINK32  /STACK:10000000 /machine:X86 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib


# Begin Special Build Tool
# End Special Build Tool


!ELSEIF  "$(CFG)" == "dcmcrle - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\bin\Debug"
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""
# ADD BASE CPP   /D "WIN32" /D "_DEBUG" /D "_CONSOLE"  /FD /GZ /c
# ADD CPP /nologo  /D "WIN32"  /D "_DEBUG" /D "_CONSOLE" /FD /GZ /c
# ADD CPP  /I "..\..\..\config\include" /I "..\..\..\dcmdata\include" /I "..\..\..\ofstd\include"   
# ADD CPP  /DWIN32 /D_WINDOWS /W3 /Zm1000 /GX /GR /D "_MBCS"  -D_REENTRANT
# ADD CPP /D_DEBUG /MDd /Zi  /Ob0 /Od /GZ -DCMAKE_INTDIR=\"Debug\"  
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC  /I "..\..\..\config\include" /I "..\..\..\dcmdata\include" /I "..\..\..\ofstd\include" /l 0x409 /d "_DEBUG"
# ADD RSC   -D_REENTRANT
# ADD RSC  
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32  /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /IGNORE:4089
# ADD LINK32 /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /IGNORE:4089 /version:0.0
# ADD LINK32 /out:"..\..\..\bin\Debug\dcmcrle.exe" /implib:..\..\..\bin\Debug\dcmcrle.lib
# ADD LINK32 /LIBPATH:..\$(IntDir) /LIBPATH:..
# ADD LINK32 /LIBPATH:..\..\ofstd\$(IntDir) /LIBPATH:..\..\ofstd
# ADD LINK32 /LIBPATH:..\..\..\lib\Debug\$(IntDir) /LIBPATH:..\..\..\lib\Debug
# ADD LINK32 netapi32.lib
# ADD LINK32 wsock32.lib
# ADD LINK32 ..\..\..\lib\Debug\ofstd.lib
# ADD LINK32 ..\..\..\lib\Debug\dcmdata.lib
# ADD LINK32  /STACK:10000000 /machine:X86 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib


# Begin Special Build Tool
# End Special Build Tool


!ELSEIF  "$(CFG)" == "dcmcrle - Win32 MinSizeRel"
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MinSizeRel"
# PROP BASE Intermediate_Dir "MinSizeRel"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\bin\MinSizeRel"
# PROP Intermediate_Dir "MinSizeRel"
# PROP Target_Dir ""
# ADD BASE CPP /nologo  /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /FD /c
# ADD CPP /nologo  /D "WIN32"  /I "..\..\..\config\include" /I "..\..\..\dcmdata\include" /I "..\..\..\ofstd\include"   /D "NDEBUG" /D "_CONSOLE"  /FD /c
# ADD CPP  /I "..\..\..\config\include" /I "..\..\..\dcmdata\include" /I "..\..\..\ofstd\include"   
# ADD CPP  /DWIN32 /D_WINDOWS /W3 /Zm1000 /GX /GR /D "_MBCS"  -D_REENTRANT
# ADD CPP /MD /O1 /Ob1 /D NDEBUG -DCMAKE_INTDIR=\"MinSizeRel\"  
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC  /I "..\..\..\config\include" /I "..\..\..\dcmdata\include" /I "..\..\..\ofstd\include" /l 0x409 /d "NDEBUG"
# ADD RSC   -D_REENTRANT
# ADD RSC  REL
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:console /machine:I386 /IGNORE:4089
# ADD LINK32 /nologo /subsystem:console /machine:I386 /IGNORE:4089 /version:0.0
# ADD LINK32 /out:"..\..\..\bin\MinSizeRel\dcmcrle.exe" /implib:..\..\..\bin\MinSizeRel\dcmcrle.lib
# ADD LINK32 /LIBPATH:..\$(IntDir) /LIBPATH:..
# ADD LINK32 /LIBPATH:..\..\ofstd\$(IntDir) /LIBPATH:..\..\ofstd
# ADD LINK32 /LIBPATH:..\..\..\lib\MinSizeRel\$(IntDir) /LIBPATH:..\..\..\lib\MinSizeRel
# ADD LINK32 netapi32.lib
# ADD LINK32 wsock32.lib
# ADD LINK32 ..\..\..\lib\MinSizeRel\ofstd.lib
# ADD LINK32 ..\..\..\lib\MinSizeRel\dcmdata.lib
# ADD LINK32  /STACK:10000000 /machine:X86 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib


# Begin Special Build Tool
# End Special Build Tool


!ELSEIF  "$(CFG)" == "dcmcrle - Win32 RelWithDebInfo"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "RelWithDebInfo"
# PROP BASE Intermediate_Dir "RelWithDebInfo"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "..\..\..\bin\RelWithDebInfo"
# PROP Intermediate_Dir "RelWithDebInfo"
# PROP Target_Dir ""
# ADD BASE CPP /nologo  /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /FD /c
# ADD CPP /nologo  /D "WIN32"  /D "NDEBUG" /D "_CONSOLE" /FD /c
# ADD CPP  /I "..\..\..\config\include" /I "..\..\..\dcmdata\include" /I "..\..\..\ofstd\include"   
# ADD CPP  /DWIN32 /D_WINDOWS /W3 /Zm1000 /GX /GR /D "_MBCS"  -D_REENTRANT
# ADD CPP /MD /Zi /O2 /Ob1 /D NDEBUG -DCMAKE_INTDIR=\"RelWithDebInfo\"  
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC  /I "..\..\..\config\include" /I "..\..\..\dcmdata\include" /I "..\..\..\ofstd\include" /l 0x409 /d "NDEBUG"
# ADD RSC   -D_REENTRANT
# ADD RSC  
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:console /debug /machine:I386 /IGNORE:4089
# ADD LINK32  /nologo /subsystem:console /debug /machine:I386 /IGNORE:4089 /version:0.0
# ADD LINK32 /out:"..\..\..\bin\RelWithDebInfo\dcmcrle.exe" /implib:..\..\..\bin\RelWithDebInfo\dcmcrle.lib
# ADD LINK32 /LIBPATH:..\$(IntDir) /LIBPATH:..
# ADD LINK32 /LIBPATH:..\..\ofstd\$(IntDir) /LIBPATH:..\..\ofstd
# ADD LINK32 /LIBPATH:..\..\..\lib\RelWithDebInfo\$(IntDir) /LIBPATH:..\..\..\lib\RelWithDebInfo
# ADD LINK32 netapi32.lib
# ADD LINK32 wsock32.lib
# ADD LINK32 ..\..\..\lib\RelWithDebInfo\ofstd.lib
# ADD LINK32 ..\..\..\lib\RelWithDebInfo\dcmdata.lib
# ADD LINK32  /STACK:10000000 /machine:X86 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib


# Begin Special Build Tool
# End Special Build Tool


!ENDIF 

# Begin Target

# Name "dcmcrle - Win32 Release"
# Name "dcmcrle - Win32 Debug"
# Name "dcmcrle - Win32 MinSizeRel"
# Name "dcmcrle - Win32 RelWithDebInfo"
# Begin Group "Source Files"
# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\dcmdata\apps\dcmcrle.cxx

!IF  "$(CFG)" == "dcmcrle - Win32 Release"

# ADD CPP  /TP 

!ELSEIF  "$(CFG)" == "dcmcrle - Win32 Debug"

# ADD CPP  /TP 

!ELSEIF  "$(CFG)" == "dcmcrle - Win32 MinSizeRel"

# ADD CPP  /TP 

!ELSEIF  "$(CFG)" == "dcmcrle - Win32 RelWithDebInfo"

# ADD CPP  /TP 

!ENDIF

# End Source File
# End Group
# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
