# Microsoft Developer Studio Project File - Name="storescu" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=storescu - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "storescu.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "storescu.mak" CFG="storescu - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "storescu - Win32 MinSizeRel" (based on "Win32 (x86) Application")
!MESSAGE "storescu - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "storescu - Win32 RelWithDebInfo" (based on "Win32 (x86) Application")
!MESSAGE "storescu - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "storescu - Win32 MinSizeRel"

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
# ADD BASE CPP /nologo /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O1 /I "..\..\..\config\include" /I "..\..\..\dcmnet\include" /I "..\..\..\ofstd\include" /I "..\..\..\dcmdata\include" /I "..\..\..\dcmtls\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_WINDOWS" /D "_MBCS" /D "_REENTRANT" /D CMAKE_INTDIR=\"MinSizeRel\" /FD /Zm1000  /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "..\..\..\config\include" /i "..\..\..\dcmnet\include" /i "..\..\..\ofstd\include" /i "..\..\..\dcmdata\include" /i "..\..\..\dcmtls\include" /d "NDEBUG" /d "_REENTRANT" REL
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:console /machine:I386 /IGNORE:4089
# ADD LINK32 netapi32.lib  wsock32.lib  ..\..\..\lib\MinSizeRel\ofstd.lib  ..\..\..\lib\MinSizeRel\dcmdata.lib  ..\..\..\lib\MinSizeRel\dcmnet.lib  ..\..\..\lib\MinSizeRel\dcmtls.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /version:0.0 /stack:0x989680 /subsystem:console /machine:I386 /implib:"..\..\..\bin\MinSizeRel\storescu.lib" /libpath:"..\$(IntDir)" /libpath:".." /libpath:"..\..\ofstd\$(IntDir)" /libpath:"..\..\ofstd" /libpath:"..\..\dcmdata\$(IntDir)" /libpath:"..\..\dcmdata" /libpath:"..\..\dcmtls\$(IntDir)" /libpath:"..\..\dcmtls" /libpath:"..\..\..\lib\MinSizeRel\$(IntDir)" /libpath:"..\..\..\lib\MinSizeRel" /IGNORE:4089              /machine:X86

!ELSEIF  "$(CFG)" == "storescu - Win32 Release"

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
# ADD BASE CPP /nologo /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /O2 /Ob2 /I "..\..\..\config\include" /I "..\..\..\dcmnet\include" /I "..\..\..\ofstd\include" /I "..\..\..\dcmdata\include" /I "..\..\..\dcmtls\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_WINDOWS" /D "_MBCS" /D "_REENTRANT" /D CMAKE_INTDIR=\"Release\" /FD /Zm1000  /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "..\..\..\config\include" /i "..\..\..\dcmnet\include" /i "..\..\..\ofstd\include" /i "..\..\..\dcmdata\include" /i "..\..\..\dcmtls\include" /d "NDEBUG" /d "_REENTRANT"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:console /machine:I386 /IGNORE:4089
# ADD LINK32 netapi32.lib  wsock32.lib  ..\..\..\lib\Release\ofstd.lib  ..\..\..\lib\Release\dcmdata.lib  ..\..\..\lib\Release\dcmnet.lib  ..\..\..\lib\Release\dcmtls.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /version:0.0 /stack:0x989680 /subsystem:console /machine:I386 /implib:"..\..\..\bin\Release\storescu.lib" /libpath:"..\$(IntDir)" /libpath:".." /libpath:"..\..\ofstd\$(IntDir)" /libpath:"..\..\ofstd" /libpath:"..\..\dcmdata\$(IntDir)" /libpath:"..\..\dcmdata" /libpath:"..\..\dcmtls\$(IntDir)" /libpath:"..\..\dcmtls" /libpath:"..\..\..\lib\Release\$(IntDir)" /libpath:"..\..\..\lib\Release" /IGNORE:4089              /machine:X86

!ELSEIF  "$(CFG)" == "storescu - Win32 RelWithDebInfo"

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
# ADD BASE CPP /nologo /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /FD /c
# ADD CPP /nologo /MD /W3 /GR /GX /Zi /O2 /I "..\..\..\config\include" /I "..\..\..\dcmnet\include" /I "..\..\..\ofstd\include" /I "..\..\..\dcmdata\include" /I "..\..\..\dcmtls\include" /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_WINDOWS" /D "_MBCS" /D "_REENTRANT" /D CMAKE_INTDIR=\"RelWithDebInfo\" /FD /Zm1000  /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /i "..\..\..\config\include" /i "..\..\..\dcmnet\include" /i "..\..\..\ofstd\include" /i "..\..\..\dcmdata\include" /i "..\..\..\dcmtls\include" /d "NDEBUG" /d "_REENTRANT"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:console /debug /machine:I386 /IGNORE:4089
# ADD LINK32 netapi32.lib  wsock32.lib  ..\..\..\lib\RelWithDebInfo\ofstd.lib  ..\..\..\lib\RelWithDebInfo\dcmdata.lib  ..\..\..\lib\RelWithDebInfo\dcmnet.lib  ..\..\..\lib\RelWithDebInfo\dcmtls.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /version:0.0 /stack:0x989680 /subsystem:console /debug /machine:I386 /implib:"..\..\..\bin\RelWithDebInfo\storescu.lib" /libpath:"..\$(IntDir)" /libpath:".." /libpath:"..\..\ofstd\$(IntDir)" /libpath:"..\..\ofstd" /libpath:"..\..\dcmdata\$(IntDir)" /libpath:"..\..\dcmdata" /libpath:"..\..\dcmtls\$(IntDir)" /libpath:"..\..\dcmtls" /libpath:"..\..\..\lib\RelWithDebInfo\$(IntDir)" /libpath:"..\..\..\lib\RelWithDebInfo" /IGNORE:4089              /machine:X86

!ELSEIF  "$(CFG)" == "storescu - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "..\..\..\bin\Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /GR /GX /Zi /Od /I "..\..\..\config\include" /I "..\..\..\dcmnet\include" /I "..\..\..\ofstd\include" /I "..\..\..\dcmdata\include" /I "..\..\..\dcmtls\include" /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_WINDOWS" /D "_MBCS" /D "_REENTRANT" /D CMAKE_INTDIR=\"Debug\" /FD /GZ   /Zm1000  /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /i "..\..\..\config\include" /i "..\..\..\dcmnet\include" /i "..\..\..\ofstd\include" /i "..\..\..\dcmdata\include" /i "..\..\..\dcmtls\include" /d "_DEBUG" /d "_REENTRANT"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept /IGNORE:4089
# ADD LINK32 netapi32.lib  wsock32.lib  ..\..\..\lib\Debug\ofstd.lib  ..\..\..\lib\Debug\dcmdata.lib  ..\..\..\lib\Debug\dcmnet.lib  ..\..\..\lib\Debug\dcmtls.lib  kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /version:0.0 /stack:0x989680 /subsystem:console /debug /machine:I386 /implib:"..\..\..\bin\Debug\storescu.lib" /pdbtype:sept /libpath:"..\$(IntDir)" /libpath:".." /libpath:"..\..\ofstd\$(IntDir)" /libpath:"..\..\ofstd" /libpath:"..\..\dcmdata\$(IntDir)" /libpath:"..\..\dcmdata" /libpath:"..\..\dcmtls\$(IntDir)" /libpath:"..\..\dcmtls" /libpath:"..\..\..\lib\Debug\$(IntDir)" /libpath:"..\..\..\lib\Debug" /IGNORE:4089              /machine:X86

!ENDIF 

# Begin Target

# Name "storescu - Win32 MinSizeRel"
# Name "storescu - Win32 Release"
# Name "storescu - Win32 RelWithDebInfo"
# Name "storescu - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\..\..\dcmnet\apps\storescu.cxx
# ADD CPP /TP
# End Source File
# End Group
# End Target
# End Project
