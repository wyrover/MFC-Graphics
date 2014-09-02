# Microsoft Developer Studio Generated NMAKE File, Based on DesignDlg.dsp
!IF $(CFG)" == "
CFG=DesignDlg - Win32 Debug
!MESSAGE No configuration specified. Defaulting to DesignDlg - Win32 Debug.
!ENDIF 

!IF "$(CFG)" != "DesignDlg - Win32 Release" && "$(CFG)" != "DesignDlg - Win32 Debug"
!MESSAGE 指定的配置 "$(CFG)" 无效.
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "DesignDlg.mak" CFG="DesignDlg - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "DesignDlg - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "DesignDlg - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF $(OS)" == "Windows_NT
NULL=
!ELSE 
NULL=nul
!ENDIF 

CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "DesignDlg - Win32 Release"

OUTDIR=.\Release
INTDIR=.\Release
# 开始自定义宏
OutDir=.\Release
# 结束自定义宏

ALL : "$(OUTDIR)\DesignDlg.exe"


CLEAN :
	-@erase "$(INTDIR)\DesignDlg.obj"
	-@erase "$(INTDIR)\DesignDlg.pch"
	-@erase "$(INTDIR)\DesignDlg.res"
	-@erase "$(INTDIR)\DesignDlgDlg.obj"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(OUTDIR)\DesignDlg.exe"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\DesignDlg.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 
MTL_PROJ=/nologo /D "NDEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\DesignDlg.res" /d "NDEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\DesignDlg.bsc" 
BSC32_SBRS= \
	
LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:no /pdb:"$(OUTDIR)\DesignDlg.pdb" /machine:I386 /out:"$(OUTDIR)\DesignDlg.exe" 
LINK32_OBJS= \
	"$(INTDIR)\DesignDlg.obj" \
	"$(INTDIR)\DesignDlgDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\DesignDlg.res"

"$(OUTDIR)\DesignDlg.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ELSEIF  "$(CFG)" == "DesignDlg - Win32 Debug"

OUTDIR=.\Debug
INTDIR=.\Debug
# 开始自定义宏
OutDir=.\Debug
# 结束自定义宏

ALL : "$(OUTDIR)\DesignDlg.exe" "$(OUTDIR)\DesignDlg.bsc"


CLEAN :
	-@erase "$(INTDIR)\DesignDlg.obj"
	-@erase "$(INTDIR)\DesignDlg.pch"
	-@erase "$(INTDIR)\DesignDlg.res"
	-@erase "$(INTDIR)\DesignDlg.sbr"
	-@erase "$(INTDIR)\DesignDlgDlg.obj"
	-@erase "$(INTDIR)\DesignDlgDlg.sbr"
	-@erase "$(INTDIR)\StdAfx.obj"
	-@erase "$(INTDIR)\StdAfx.sbr"
	-@erase "$(INTDIR)\vc60.idb"
	-@erase "$(INTDIR)\vc60.pdb"
	-@erase "$(OUTDIR)\DesignDlg.bsc"
	-@erase "$(OUTDIR)\DesignDlg.exe"
	-@erase "$(OUTDIR)\DesignDlg.ilk"
	-@erase "$(OUTDIR)\DesignDlg.pdb"

"$(OUTDIR)" :
    if not exist "$(OUTDIR)/$(NULL)" mkdir "$(OUTDIR)"

CPP_PROJ=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\DesignDlg.pch" /Yu"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 
MTL_PROJ=/nologo /D "_DEBUG" /mktyplib203 /win32 
RSC_PROJ=/l 0x804 /fo"$(INTDIR)\DesignDlg.res" /d "_DEBUG" /d "_AFXDLL" 
BSC32=bscmake.exe
BSC32_FLAGS=/nologo /o"$(OUTDIR)\DesignDlg.bsc" 
BSC32_SBRS= \
	"$(INTDIR)\DesignDlg.sbr" \
	"$(INTDIR)\DesignDlgDlg.sbr" \
	"$(INTDIR)\StdAfx.sbr"

"$(OUTDIR)\DesignDlg.bsc" : "$(OUTDIR)" $(BSC32_SBRS)
    $(BSC32) @<<
  $(BSC32_FLAGS) $(BSC32_SBRS)
<<

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:windows /incremental:yes /pdb:"$(OUTDIR)\DesignDlg.pdb" /debug /machine:I386 /out:"$(OUTDIR)\DesignDlg.exe" /pdbtype:sept 
LINK32_OBJS= \
	"$(INTDIR)\DesignDlg.obj" \
	"$(INTDIR)\DesignDlgDlg.obj" \
	"$(INTDIR)\StdAfx.obj" \
	"$(INTDIR)\DesignDlg.res"

"$(OUTDIR)\DesignDlg.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

!ENDIF 

.c{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.c{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cpp{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

.cxx{$(INTDIR)}.sbr::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<


!IF "$(NO_EXTERNAL_DEPS)" != "1"
!IF EXISTS("DesignDlg.dep")
!INCLUDE "DesignDlg.dep"
!ELSE 
!MESSAGE Warning: cannot find "DesignDlg.dep"
!ENDIF 
!ENDIF 


!IF "$(CFG)" == "DesignDlg - Win32 Release" || "$(CFG)" == "DesignDlg - Win32 Debug"
SOURCE=.\DesignDlg.cpp

!IF  "$(CFG)" == "DesignDlg - Win32 Release"


"$(INTDIR)\DesignDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\DesignDlg.pch"


!ELSEIF  "$(CFG)" == "DesignDlg - Win32 Debug"


"$(INTDIR)\DesignDlg.obj"	"$(INTDIR)\DesignDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\DesignDlg.pch"


!ENDIF 

SOURCE=.\DesignDlg.rc

"$(INTDIR)\DesignDlg.res" : $(SOURCE) "$(INTDIR)"
	$(RSC) $(RSC_PROJ) $(SOURCE)


SOURCE=.\DesignDlgDlg.cpp

!IF  "$(CFG)" == "DesignDlg - Win32 Release"


"$(INTDIR)\DesignDlgDlg.obj" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\DesignDlg.pch"


!ELSEIF  "$(CFG)" == "DesignDlg - Win32 Debug"


"$(INTDIR)\DesignDlgDlg.obj"	"$(INTDIR)\DesignDlgDlg.sbr" : $(SOURCE) "$(INTDIR)" "$(INTDIR)\DesignDlg.pch"


!ENDIF 

SOURCE=.\StdAfx.cpp

!IF  "$(CFG)" == "DesignDlg - Win32 Release"

CPP_SWITCHES=/nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /Fp"$(INTDIR)\DesignDlg.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\DesignDlg.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ELSEIF  "$(CFG)" == "DesignDlg - Win32 Debug"

CPP_SWITCHES=/nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR"$(INTDIR)\\" /Fp"$(INTDIR)\DesignDlg.pch" /Yc"stdafx.h" /Fo"$(INTDIR)\\" /Fd"$(INTDIR)\\" /FD /GZ /c 

"$(INTDIR)\StdAfx.obj"	"$(INTDIR)\StdAfx.sbr"	"$(INTDIR)\DesignDlg.pch" : $(SOURCE) "$(INTDIR)"
	$(CPP) @<<
  $(CPP_SWITCHES) $(SOURCE)
<<


!ENDIF 


!ENDIF 

