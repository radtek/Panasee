# Microsoft Developer Studio Project File - Name="INSTALL" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Generic Project" 0x010a

CFG=INSTALL - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "INSTALL.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "INSTALL.mak" CFG="INSTALL - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "INSTALL - Win32 MinSizeRel" (based on "Win32 (x86) Generic Project")
!MESSAGE "INSTALL - Win32 Release" (based on "Win32 (x86) Generic Project")
!MESSAGE "INSTALL - Win32 RelWithDebInfo" (based on "Win32 (x86) Generic Project")
!MESSAGE "INSTALL - Win32 Debug" (based on "Win32 (x86) Generic Project")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
MTL=midl.exe

!IF  "$(CFG)" == "INSTALL - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""



!ELSEIF  "$(CFG)" == "INSTALL - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Intermediate_Dir "Debug"
# PROP Target_Dir ""



!ELSEIF  "$(CFG)" == "INSTALL - Win32 MinSizeRel"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "MinSizeRel"
# PROP BASE Intermediate_Dir "MinSizeRel"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Intermediate_Dir "MinSizeRel"
# PROP Target_Dir ""



!ELSEIF  "$(CFG)" == "INSTALL - Win32 RelWithDebInfo"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "RelWithDebInfo"
# PROP BASE Intermediate_Dir "RelWithDebInfo"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Intermediate_Dir "RelWithDebInfo"
# PROP Target_Dir ""



!ENDIF 

# Begin Target

# Name "INSTALL - Win32 Release"
# Name "INSTALL - Win32 Debug"
# Name "INSTALL - Win32 MinSizeRel"
# Name "INSTALL - Win32 RelWithDebInfo"
# Begin Group "CMake Rules"
# PROP Default_Filter ""
# Begin Source File

SOURCE=INSTALL_force_1.rule

!IF  "$(CFG)" == "INSTALL - Win32 Release"
USERDEP__HACK=
# PROP Ignore_Default_Tool 1
# Begin Custom Build -

INSTALL_force_1 :  "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	"D:\Program Files\CMake 2.8\bin\cmake.exe" -DBUILD_TYPE=$(IntDir) -P cmake_install.cmake

# End Custom Build

!ELSEIF  "$(CFG)" == "INSTALL - Win32 Debug"
USERDEP__HACK=
# PROP Ignore_Default_Tool 1
# Begin Custom Build -

INSTALL_force_1 :  "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	"D:\Program Files\CMake 2.8\bin\cmake.exe" -DBUILD_TYPE=$(IntDir) -P cmake_install.cmake

# End Custom Build

!ELSEIF  "$(CFG)" == "INSTALL - Win32 MinSizeRel"
USERDEP__HACK=
# PROP Ignore_Default_Tool 1
# Begin Custom Build -

INSTALL_force_1 :  "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	"D:\Program Files\CMake 2.8\bin\cmake.exe" -DBUILD_TYPE=$(IntDir) -P cmake_install.cmake

# End Custom Build

!ELSEIF  "$(CFG)" == "INSTALL - Win32 RelWithDebInfo"
USERDEP__HACK=
# PROP Ignore_Default_Tool 1
# Begin Custom Build -

INSTALL_force_1 :  "$(SOURCE)" "$(INTDIR)" "$(OUTDIR)"
	"D:\Program Files\CMake 2.8\bin\cmake.exe" -DBUILD_TYPE=$(IntDir) -P cmake_install.cmake

# End Custom Build

!ENDIF

# End Source File
# End Group
# End Target
# End Project
