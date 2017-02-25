# Microsoft Developer Studio Project File - Name="ephem" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Console Application" 0x0103

CFG=ephem - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ephem.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ephem.mak" CFG="ephem - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ephem - Win32 Release" (based on "Win32 (x86) Console Application")
!MESSAGE "ephem - Win32 Debug" (based on "Win32 (x86) Console Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ephem - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /c
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /machine:I386

!ELSEIF  "$(CFG)" == "ephem - Win32 Debug"

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
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_CONSOLE" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:console /debug /machine:I386 /pdbtype:sept

!ENDIF 

# Begin Target

# Name "ephem - Win32 Release"
# Name "ephem - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=..\aa_hadec.c
# End Source File
# Begin Source File

SOURCE=..\altj.c
# End Source File
# Begin Source File

SOURCE=..\altmenus.c
# End Source File
# Begin Source File

SOURCE=..\anomaly.c
# End Source File
# Begin Source File

SOURCE=..\cal_mjd.c
# End Source File
# Begin Source File

SOURCE=..\circum.c
# End Source File
# Begin Source File

SOURCE=..\comet.c
# End Source File
# Begin Source File

SOURCE=..\compiler.c
# End Source File
# Begin Source File

SOURCE=..\constel.c
# End Source File
# Begin Source File

SOURCE=..\ephem.cfg
# End Source File
# Begin Source File

SOURCE=..\eq_ecl.c
# End Source File
# Begin Source File

SOURCE=..\flog.c
# End Source File
# Begin Source File

SOURCE=..\formats.c
# End Source File
# Begin Source File

SOURCE=..\io.c
# End Source File
# Begin Source File

SOURCE=..\listing.c
# End Source File
# Begin Source File

SOURCE=..\main.c
# End Source File
# Begin Source File

SOURCE=..\mainmenu.c
# End Source File
# Begin Source File

SOURCE=..\moon.c
# End Source File
# Begin Source File

SOURCE=..\moonnf.c
# End Source File
# Begin Source File

SOURCE=..\nutation.c
# End Source File
# Begin Source File

SOURCE=..\objx.c
# End Source File
# Begin Source File

SOURCE=..\obliq.c
# End Source File
# Begin Source File

SOURCE=..\parallax.c
# End Source File
# Begin Source File

SOURCE=..\pelement.c
# End Source File
# Begin Source File

SOURCE=..\plans.c
# End Source File
# Begin Source File

SOURCE=..\plot.c
# End Source File
# Begin Source File

SOURCE=..\popup.c
# End Source File
# Begin Source File

SOURCE=..\precess.c
# End Source File
# Begin Source File

SOURCE=..\reduce.c
# End Source File
# Begin Source File

SOURCE=..\refract.c
# End Source File
# Begin Source File

SOURCE=..\riset.c
# End Source File
# Begin Source File

SOURCE=..\riset_c.c
# End Source File
# Begin Source File

SOURCE=..\sel_fld.c
# End Source File
# Begin Source File

SOURCE=..\sex_dec.c
# End Source File
# Begin Source File

SOURCE=..\srch.c
# End Source File
# Begin Source File

SOURCE=..\sun.c
# End Source File
# Begin Source File

SOURCE=..\time.c
# End Source File
# Begin Source File

SOURCE=..\utc_gst.c
# End Source File
# Begin Source File

SOURCE=..\version.c
# End Source File
# Begin Source File

SOURCE=..\watch.c
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=..\astro.h
# End Source File
# Begin Source File

SOURCE=..\circum.h
# End Source File
# Begin Source File

SOURCE=..\screen.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
