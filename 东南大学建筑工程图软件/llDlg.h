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
	int jiemianhorizontal;//水平截线，纵截距
	int jiemianvertical;//竖直截线，横截距

	struct 
	{
		ELEMENTTYPE Type;
		//坐标轴线用
		BOOL IsHorizontal;
		int Index;
	}selectline;

	CDC MyDC;
	CDC* tempdc;
	CBitmap bmp;
	CBitmap* oldbmp;
	CDialog* showdlg;
	CPoint Fixone;//试探线绘图  创建元素时有效
	CPoint Moveone;

	CArray<int,int> drawhorizontal;//水平轴相对第一条线距离阵列
	CArray<int,int> drawvertical;//竖直轴相对第一条线距离阵列
	CArray<PILLAR_PROPERTY*,PILLAR_PROPERTY*> pillars;//明柱&暗柱阵列
	CArray<BEAM_PROPERTY*,BEAM_PROPERTY*> beams;//主梁&次梁阵列
	CArray<RECTZONE_PROPERTY*,RECTZONE_PROPERTY*> rectzones;//方形内部实体阵列
	CArray<POLYZONE_PROPERTY*,POLYZONE_PROPERTY*> polyzones;//方形内部实体阵列
	CArray<BASE_ELEMENT*,BASE_ELEMENT*> selelement;//当前选中的元素阵列
	ZONEOUTLINE_PROPERTY* createelement;//当前正在创建的元素

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
