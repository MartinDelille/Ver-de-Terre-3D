# Microsoft Developer Studio Project File - Name="Ver de Terre 3D" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Ver de Terre 3D - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Ver de Terre 3D.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Ver de Terre 3D.mak" CFG="Ver de Terre 3D - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Ver de Terre 3D - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Ver de Terre 3D - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Ver de Terre 3D - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x40c /d "NDEBUG"
# ADD RSC /l 0x40c /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 winmm.lib ddraw.lib d3dim.lib dsound.lib dinput.lib dxguid.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386

!ELSEIF  "$(CFG)" == "Ver de Terre 3D - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /Zi /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /FR /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /o "NUL" /win32
# ADD BASE RSC /l 0x40c /d "_DEBUG"
# ADD RSC /l 0x40c /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 winmm.lib ddraw.lib d3dim.lib dsound.lib dinput.lib dxguid.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "Ver de Terre 3D - Win32 Release"
# Name "Ver de Terre 3D - Win32 Debug"
# Begin Group "DirectX"

# PROP Default_Filter ""
# Begin Source File

SOURCE=..\Utiles\d3d.cpp
# End Source File
# Begin Source File

SOURCE=..\Utiles\d3dutils.cpp
# End Source File
# Begin Source File

SOURCE=..\Utiles\d3dutils.h
# End Source File
# Begin Source File

SOURCE=..\Utiles\ddraw.cpp
# End Source File
# Begin Source File

SOURCE=..\Utiles\ddutil.cpp
# End Source File
# Begin Source File

SOURCE=..\Utiles\ddutil.h
# End Source File
# Begin Source File

SOURCE=..\Utiles\dinput.cpp
# End Source File
# Begin Source File

SOURCE=..\Utiles\directx.cpp
# End Source File
# Begin Source File

SOURCE=..\Utiles\directx.h
# End Source File
# Begin Source File

SOURCE=..\Utiles\dsound.cpp
# End Source File
# Begin Source File

SOURCE=..\Utiles\dsutil.cpp
# End Source File
# Begin Source File

SOURCE=..\Utiles\dsutil.h
# End Source File
# Begin Source File

SOURCE=..\Utiles\enum.cpp
# End Source File
# Begin Source File

SOURCE=..\Utiles\enum.h
# End Source File
# Begin Source File

SOURCE=..\Utiles\erreur.cpp
# End Source File
# Begin Source File

SOURCE=..\Utiles\fichier.cpp
# End Source File
# Begin Source File

SOURCE=..\Utiles\fichier.h
# End Source File
# Begin Source File

SOURCE=..\Utiles\fonctions.h
# End Source File
# Begin Source File

SOURCE=..\Utiles\main.cpp
# End Source File
# Begin Source File

SOURCE=..\Utiles\main.h
# End Source File
# Begin Source File

SOURCE=..\Utiles\resource.h
# End Source File
# Begin Source File

SOURCE=..\Utiles\Resource.rc
# End Source File
# Begin Source File

SOURCE=..\Utiles\wave.cpp
# End Source File
# Begin Source File

SOURCE=..\Utiles\wave.h
# End Source File
# End Group
# Begin Group "Perso"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\fonctions.cpp
# End Source File
# Begin Source File

SOURCE=.\objets.cpp
# End Source File
# Begin Source File

SOURCE=.\objets.h
# End Source File
# Begin Source File

SOURCE=.\Utiles.cpp
# End Source File
# Begin Source File

SOURCE=.\Utiles.h
# End Source File
# End Group
# Begin Source File

SOURCE="..\Utiles\\martin\données\directx.ico"
# End Source File
# End Target
# End Project
