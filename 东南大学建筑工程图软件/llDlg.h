// llDlg.h : header file
//

#if !defined(AFX_LLDLG_H__9E486061_D152_4B79_8591_2CBC1F64583D__INCLUDED_)
#define AFX_LLDLG_H__9E486061_D152_4B79_8591_2CBC1F64583D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxtempl.h>
#include "MyStatic.h"
#include "globaldefs.h"
/////////////////////////////////////////////////////////////////////////////
// CLlDlg dialog

class CLlDlg : public CDialog
{
// Construction
public:
	CLlDlg(CWnd* pParent = NULL);	// standard constructor
//Attributes
	STATE curstate;
	int pillarsindex;
	int beamsindex;
	int rectzonesindex;
	int polyzonesindex;
	int jiemianhorizontal;//ˮƽ���ߣ��ݽؾ�
	int jiemianvertical;//��ֱ���ߣ���ؾ�

	struct 
	{
		ELEMENTTYPE Type;
		//����������
		BOOL IsHorizontal;
		int Index;
	}selectline;

	CDC MyDC;
	CDC* tempdc;
	CBitmap bmp;
	CBitmap* oldbmp;
	CDialog* showdlg;
	CPoint Fixone;//��̽�߻�ͼ  ����Ԫ��ʱ��Ч
	CPoint Moveone;

	CArray<int,int> drawhorizontal;//ˮƽ����Ե�һ���߾�������
	CArray<int,int> drawvertical;//��ֱ����Ե�һ���߾�������
	CArray<PILLAR_PROPERTY*,PILLAR_PROPERTY*> pillars;//����&��������
	CArray<BEAM_PROPERTY*,BEAM_PROPERTY*> beams;//����&��������
	CArray<RECTZONE_PROPERTY*,RECTZONE_PROPERTY*> rectzones;//�����ڲ�ʵ������
	CArray<POLYZONE_PROPERTY*,POLYZONE_PROPERTY*> polyzones;//�����ڲ�ʵ������
	CArray<BASE_ELEMENT*,BASE_ELEMENT*> selelement;//��ǰѡ�е�Ԫ������
	ZONEOUTLINE_PROPERTY* createelement;//��ǰ���ڴ�����Ԫ��

//Operations
	void PhysicalPointToUIPoint(POINT* pt,const RECT& uirect,int x,int y);
	void PhysicalRECTToUIRECT(MYRECT& rt,const CRect& uisize);
	void UIPointToPhysicalPoint(POINT* pt,int x,int y);
// Dialog Data
	//{{AFX_DATA(CLlDlg)
	enum { IDD = IDD_LL_DIALOG };
	CMyStatic	m_draw;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLlDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CLlDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LLDLG_H__9E486061_D152_4B79_8591_2CBC1F64583D__INCLUDED_)
