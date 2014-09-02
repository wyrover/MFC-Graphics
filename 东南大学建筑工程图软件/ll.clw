; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSectionSet
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ll.h"

ClassCount=10
Class1=CLlApp
Class2=CLlDlg

ResourceCount=6
Resource2=IDD_LL_DIALOG
Class4=CSetDialog
Class3=CMyStatic
Resource1=IDR_MAINFRAME
Class5=CElementListBox
Class6=CProPertyListBox
Resource3=IDD_SETSECTION
Resource4=IDD_LL_DIALOG_SECTION
Class7=CSectionDraw
Class8=CSectionStatic
Resource5=IDD_SHOW_DIALOG
Class9=CSectionSet
Class10=CInputDialog
Resource6=IDD_INPUT

[CLS:CLlApp]
Type=0
HeaderFile=ll.h
ImplementationFile=ll.cpp
Filter=N

[CLS:CLlDlg]
Type=0
HeaderFile=llDlg.h
ImplementationFile=llDlg.cpp
Filter=M
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_DRAW


[DLG:IDD_LL_DIALOG]
Type=1
Class=CLlDlg
ControlCount=1
Control1=IDC_DRAW,static,1342177287

[CLS:CMyStatic]
Type=0
HeaderFile=MyStatic.h
ImplementationFile=MyStatic.cpp
BaseClass=CStatic
Filter=W
VirtualFilter=WC
LastObject=CMyStatic

[DLG:IDD_SHOW_DIALOG]
Type=1
Class=CSetDialog
ControlCount=38
Control1=IDC_LISTDEFAULT,listbox,1350631681
Control2=IDC_ADDDEFAULT,button,1342242816
Control3=IDC_DELETE,button,1342242816
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_LISTDESTX,listbox,1352728833
Control7=IDC_STATIC,static,1342308352
Control8=IDC_LISTDESTY,listbox,1352728833
Control9=IDC_STATIC,static,1342308352
Control10=IDC_ADDSPEC,button,1342242816
Control11=IDC_SPEC,edit,1350631552
Control12=IDC_SETX,button,1342308361
Control13=IDC_SETY,button,1342177289
Control14=IDC_STATIC,button,1342177287
Control15=IDC_STATIC,button,1342177287
Control16=IDC_NONE,button,1342308361
Control17=IDC_PILLAR,button,1342177289
Control18=IDC_PRIMARYBEAM,button,1342177289
Control19=IDC_SECONDARYBEAM,button,1342177289
Control20=IDC_POLYZONE,button,1342177289
Control21=IDC_STATIC,button,1342177287
Control22=IDC_STATIC,static,1342308352
Control23=IDC_ELEMENTLIST,listbox,1352730881
Control24=IDC_PROPERTYLIST,listbox,1352728833
Control25=IDC_STATIC,static,1342308352
Control26=IDC_STATIC,button,1342177287
Control27=IDC_STATIC,static,1342308352
Control28=IDC_STATIC,static,1342308352
Control29=IDC_STATIC,static,1342308352
Control30=IDC_STATIC,static,1342308352
Control31=IDC_STATIC,static,1342308352
Control32=IDC_STATIC,static,1342308352
Control33=IDC_ZHOUXIANFIG,static,1342177287
Control34=IDC_ZHULIANGFIG,static,1342177287
Control35=IDC_JIEMIANXIANFIG,static,1342177287
Control36=IDC_CILIANGFIG,static,1342177287
Control37=IDC_ZHUFIG,static,1342177287
Control38=IDC_NEIBUQUYUFIG,static,1342177287

[CLS:CSetDialog]
Type=0
HeaderFile=SetDialog.h
ImplementationFile=SetDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=CSetDialog
VirtualFilter=dWC

[CLS:CElementListBox]
Type=0
HeaderFile=ElementListBox.h
ImplementationFile=ElementListBox.cpp
BaseClass=CListBox
Filter=W
VirtualFilter=bWC

[CLS:CProPertyListBox]
Type=0
HeaderFile=ProPertyListBox.h
ImplementationFile=ProPertyListBox.cpp
BaseClass=CListBox
Filter=W
VirtualFilter=bWC

[DLG:IDD_LL_DIALOG_SECTION]
Type=1
Class=CSectionDraw
ControlCount=1
Control1=IDC_DRAW,static,1342177287

[DLG:IDD_SETSECTION]
Type=1
Class=CSectionSet
ControlCount=25
Control1=IDC_STATIC,static,1342308352
Control2=IDC_STATIC,static,1342308352
Control3=IDC_HORPOLEDIS,edit,1350631552
Control4=IDC_STATIC,button,1342177287
Control5=IDC_STATIC,button,1342177287
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_POLEFIG,static,1342177287
Control11=IDC_BRIDGEFIG,static,1342177287
Control12=IDC_FLITCHFIG,static,1342177287
Control13=IDC_BOARDFILLFIG,static,1342177287
Control14=IDC_HEIPOLEDIS,edit,1350631552
Control15=IDC_STATIC,static,1342308352
Control16=IDC_BEAMPARAMS,SysListView32,1350631429
Control17=IDC_STATIC,static,1342308352
Control18=IDC_BOARDPARAMS,SysListView32,1350631429
Control19=IDC_ADDBEAMPARAM,button,1342242816
Control20=IDC_ADDBOARDPARAM,button,1342242816
Control21=IDC_STATIC,static,1342308352
Control22=IDC_VERPOLEDIS,edit,1350631552
Control23=IDC_STATIC,button,1342177287
Control24=IDC_APPLYCHANGE,button,1342242816
Control25=IDC_REMOVECLIP,button,1342242816

[CLS:CSectionDraw]
Type=0
HeaderFile=SectionDraw.h
ImplementationFile=SectionDraw.cpp
BaseClass=CDialog
Filter=D
LastObject=CSectionDraw
VirtualFilter=dWC

[CLS:CSectionStatic]
Type=0
HeaderFile=SectionStatic.h
ImplementationFile=SectionStatic.cpp
BaseClass=CStatic
Filter=W
VirtualFilter=WC

[CLS:CSectionSet]
Type=0
HeaderFile=SectionSet.h
ImplementationFile=SectionSet.cpp
BaseClass=CDialog
Filter=D
LastObject=CSectionSet
VirtualFilter=dWC

[DLG:IDD_INPUT]
Type=1
Class=CInputDialog
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_PARAM1,static,1342308352
Control4=IDC_PAR1,edit,1350631552
Control5=IDC_PARAM2,static,1342308352
Control6=IDC_PAR2,edit,1350631552
Control7=IDC_PARAM3,static,1342308352
Control8=IDC_PAR3,edit,1350631552
Control9=IDC_COMBO,combobox,1075904514

[CLS:CInputDialog]
Type=0
HeaderFile=InputDialog.h
ImplementationFile=InputDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=CInputDialog
VirtualFilter=dWC

