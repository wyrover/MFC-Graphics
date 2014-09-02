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
			SetWindowText("���������");
			SetDlgItemText(IDC_PARAM1,"����������");
			SetDlgItemText(IDC_PARAM2,"����");
			SetDlgItemText(IDC_PARAM3,"����");
			break;

		case PARAM_ADDBOARD:
			SetWindowText("��Ӱ�����");
			SetDlgItemText(IDC_PARAM1,"�巽������");
			SetDlgItemText(IDC_PARAM2,"���");
			GetDlgItem(IDC_PARAM3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_PAR3)->ShowWindow(SW_HIDE);
			break;

		case PARAM_MODBEAMSECTION:
			SetWindowText("�޸����������");
			SetDlgItemText(IDC_PARAM1,"��ǰ������");
			SetDlgItemText(IDC_PAR1,ppa->param1);
			SetDlgItemText(IDC_PARAM2,"Ŀ��������");
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
			SetWindowText("�޸İ�������");
			SetDlgItemText(IDC_PARAM1,"��ǰ�巽��");
			SetDlgItemText(IDC_PAR1,ppa->param1);
			SetDlgItemText(IDC_PARAM2,"Ŀ�İ巽��");
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
			SetWindowText("�޸Ĳ��");
			SetDlgItemText(IDC_PARAM1,"��ǰ���");
			SetDlgItemText(IDC_PAR1,ppa->param1);
			SetDlgItemText(IDC_PARAM2,"Ŀ�Ĳ��");
			GetDlgItem(IDC_PARAM3)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_PAR3)->ShowWindow(SW_HIDE);
			break;

		case PARAM_MODDISTANCE:
			SetWindowText("�޸ļ��");
			SetDlgItemText(IDC_PARAM1,"��ǰ���");
			SetDlgItemText(IDC_PAR1,ppa->param1);
			SetDlgItemText(IDC_PARAM2,"Ŀ�ļ��");
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
