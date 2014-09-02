// SectionSet.cpp : implementation file
//

#include "stdafx.h"
#include "ll.h"
#include "SectionSet.h"
#include "globaldefs.h"
#include "InputDialog.h"
#include "SectionDraw.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSectionSet dialog


CSectionSet::CSectionSet(CWnd* pParent /*=NULL*/)
	: CDialog(CSectionSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSectionSet)
	m_horpoledis = _T("");
	m_heipoledis = _T("");
	m_verpoledis = _T("");
	//}}AFX_DATA_INIT
}


void CSectionSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSectionSet)
	DDX_Control(pDX, IDC_BOARDPARAMS, m_boardtemp);
	DDX_Control(pDX, IDC_BEAMPARAMS, m_beamtemp);
	DDX_Text(pDX, IDC_HORPOLEDIS, m_horpoledis);
	DDX_Text(pDX, IDC_HEIPOLEDIS, m_heipoledis);
	DDX_Text(pDX, IDC_VERPOLEDIS, m_verpoledis);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSectionSet, CDialog)
	//{{AFX_MSG_MAP(CSectionSet)
	ON_BN_CLICKED(IDC_ADDBEAMPARAM, OnAddbeamparam)
	ON_BN_CLICKED(IDC_ADDBOARDPARAM, OnAddboardparam)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_APPLYCHANGE, OnApplychange)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_REMOVECLIP, OnRemoveclip)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSectionSet message handlers

void CSectionSet::OnAddbeamparam() 
{
	// TODO: Add your control notification handler code here
	Params pa;
	pa.paramtype=PARAM_ADDBEAM;
	CInputDialog dlg(this);
	dlg.ppa=&pa;
	if(IDOK == dlg.DoModal())
	{
		int index=m_beamtemp.GetItemCount();
		m_beamtemp.InsertItem(index,pa.param1);
		m_beamtemp.SetItemText(index,1,pa.param2);
		m_beamtemp.SetItemText(index,2,pa.param3);
		archtemplate* curd=new archtemplate;
		curd->type=ELEMENT_BEAM;
		curd->tempname=pa.param1;
		curd->tem.beam.beamheight=atoi(pa.param2);
		curd->tem.beam.beamwidth=atoi(pa.param3);
		m_beamtemp.SetItemData(index,(DWORD)curd);
	}
}

void CSectionSet::OnAddboardparam() 
{
	// TODO: Add your control notification handler code here
	Params pa;
	pa.paramtype=PARAM_ADDBOARD;
	CInputDialog dlg(this);
	dlg.ppa=&pa;
	if(IDOK == dlg.DoModal())
	{
		int index=m_boardtemp.GetItemCount();
		m_boardtemp.InsertItem(index,pa.param1);
		m_boardtemp.SetItemText(index,1,pa.param2);
		archtemplate* curd=new archtemplate;
		curd->type=ELEMENT_BOARD;
		curd->tempname=pa.param1;
		curd->tem.board.boardthick=atoi(pa.param2);
		m_boardtemp.SetItemData(index,(DWORD)curd);
	}	
}

BOOL CSectionSet::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	m_beamtemp.InsertColumn(0,"Ä£°åÃû³Æ",LVCFMT_LEFT,80);
	m_beamtemp.InsertColumn(1,"Áº¸ß",LVCFMT_LEFT,80);
	m_beamtemp.InsertColumn(2,"Áº¿í",LVCFMT_LEFT,80);

	m_boardtemp.InsertColumn(0,"Ä£°åÃû³Æ",LVCFMT_LEFT,80);
	m_boardtemp.InsertColumn(1,"°åºñ",LVCFMT_LEFT,80);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSectionSet::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
}

void CSectionSet::OnApplychange() 
{
	// TODO: Add your control notification handler code here
	((CSectionDraw*)SectionVer)->Update();
	((CSectionDraw*)SectionHor)->Update();
}

void CSectionSet::OnDestroy() 
{
	int i;
	for(i=0;i<m_beamtemp.GetItemCount();i++)
	{
		delete (archtemplate*)m_beamtemp.GetItemData(i);
	}
	for(i=0;i<m_boardtemp.GetItemCount();i++)
	{
		delete (archtemplate*)m_boardtemp.GetItemData(i);
	}	

	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
}

void CSectionSet::OnRemoveclip() 
{
	// TODO: Add your control notification handler code here
	CSectionDraw* draw;
	int i;

	draw=(CSectionDraw*)SectionVer;
	for(i=0;i< draw->Scissorsline.GetSize();i++)
	{
		delete draw->Scissorsline[i];
	}
	draw->Invalidate(FALSE);

	draw=(CSectionDraw*)SectionHor;
	for(i=0;i< draw->Scissorsline.GetSize();i++)
	{
		delete draw->Scissorsline[i];
	}
	draw->Invalidate(FALSE);
}
