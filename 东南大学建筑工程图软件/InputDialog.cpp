// InputDialog.cpp : implementation file
//

#include "stdafx.h"
#include "ll.h"
#include "InputDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#include "globaldefs.h"
/////////////////////////////////////////////////////////////////////////////
// CInputDialog dialog


CInputDialog::CInputDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CInputDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	ppa=NULL;
}


void CInputDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputDialog)
	DDX_Control(pDX, IDC_COMBO, m_combo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputDialog, CDialog)
	//{{AFX_MSG_MAP(CInputDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputDialog message handlers

void CInputDialog::OnOK() 
{
	// TODO: Add extra validation here

	GetDlgItemText(IDC_PAR1,ppa->param1);
	if(ppa->paramtype == PARAM_MODBEAMSECTION || ppa->paramtype == PARAM_MODBOARDSECTION)
	{
		GetDlgItemText(IDC_COMBO,ppa->param2);
	}
	else
	{
		GetDlgItemText(IDC_PAR2,ppa->param2);
	}
	GetDlgItemText(IDC_PAR3,ppa->param3);

	CDialog::OnOK();
}

BOOL CInputDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	int i;

	switch(ppa->paramtype)
	{
		case PARAM_ADDBEAM:
			SetWindowText("添加梁配置");
			SetDlgItemText(IDC_PARAM1,"梁方案名称");
			SetDlgItemText(IDC_PARAM2,"梁高");
			SetDlgItemText(IDC_PARAM3,"梁宽");
			break;

		case PARAM_ADDBOARD:
			SetWindowText("添加板配置");
			SetDlgItemText(IDC_PARAM1,"板方案名称");
			SetDlgItemText(IDC_PARAM2,"板厚");
			GetDlgItem(IDC_PARAM3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_PAR3)->ShowWindow(SW_HIDE);
			break;

		case PARAM_MODBEAMSECTION:
			SetWindowText("修改梁截面参数");
			SetDlgItemText(IDC_PARAM1,"当前梁方案");
			SetDlgItemText(IDC_PAR1,ppa->param1);
			SetDlgItemText(IDC_PARAM2,"目的梁方案");
			GetDlgItem(IDC_PAR2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COMBO)->ShowWindow(SW_SHOW);
			for(i=0;i<ppa->chooselist.GetSize();i++)
			{
				m_combo.AddString(ppa->chooselist[i]);
			}
			GetDlgItem(IDC_PAR3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_PARAM3)->ShowWindow(SW_HIDE);			
			break;

		case PARAM_MODBOARDSECTION:
			SetWindowText("修改板截面参数");
			SetDlgItemText(IDC_PARAM1,"当前板方案");
			SetDlgItemText(IDC_PAR1,ppa->param1);
			SetDlgItemText(IDC_PARAM2,"目的板方案");
			GetDlgItem(IDC_PAR2)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_COMBO)->ShowWindow(SW_SHOW);
			for(i=0;i<ppa->chooselist.GetSize();i++)
			{
				m_combo.AddString(ppa->chooselist[i]);
			}
			GetDlgItem(IDC_PAR3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_PARAM3)->ShowWindow(SW_HIDE);
			break;

		case PARAM_MODLAYERHEIGHT:
			SetWindowText("修改层高");
			SetDlgItemText(IDC_PARAM1,"当前层高");
			SetDlgItemText(IDC_PAR1,ppa->param1);
			SetDlgItemText(IDC_PARAM2,"目的层高");
			GetDlgItem(IDC_PARAM3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_PAR3)->ShowWindow(SW_HIDE);
			break;

		case PARAM_MODDISTANCE:
			SetWindowText("修改间距");
			SetDlgItemText(IDC_PARAM1,"当前间距");
			SetDlgItemText(IDC_PAR1,ppa->param1);
			SetDlgItemText(IDC_PARAM2,"目的间距");
			GetDlgItem(IDC_PARAM3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_PAR3)->ShowWindow(SW_HIDE);
			break;

		default:
			break;
	}
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
