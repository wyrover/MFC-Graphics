// drawsortDlg.h : header file
//

#if !defined(AFX_DRAWSORTDLG_H__D8E61482_7DCF_48BC_908B_691481D9F40E__INCLUDED_)
#define AFX_DRAWSORTDLG_H__D8E61482_7DCF_48BC_908B_691481D9F40E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CDrawsortDlg dialog

#include <vector>
#include <queue>
#include <set>
using namespace std;

class DrawData
{
public:
	vector<int> data;//当前数组
	set<int> stress;//提示要进行操作的数
	int step;//已进行得步数
	DrawData(vector<int>& _data,set<int>& _stress,int _step):data(_data),stress(_stress),step(_step)
	{

	}
};

enum 
{
	TYPE_MAOPAO=0,
	TYPE_MAOPAO1,
	TYPE_MAOPAO2,
	TYPE_KUAISU,
	TYPE_CHARU,
	TYPE_ERFENCHARU,
	TYPE_XIER,

	TYPE_XUANZE,
	TYPE_JINBIAOSAI,
	TYPE_HEAP,
	TYPE_MERGE,
	TYPE_BUCKET,
	TYPE_UNUSED1,
	TYPE_UNUSED2,
	TYPE_MAX
};

class CDrawsortDlg : public CDialog
{
// Construction
public:
	CDrawsortDlg(CWnd* pParent = NULL);	// standard constructor
typedef queue<DrawData> DataQueue;
	vector<DataQueue> toDrawQueue;

	void MAOPAO(const vector<int>& data);
	void MAOPAO1(const vector<int>& data);
	void MAOPAO2(const vector<int>& data);
	void KUAISU(const vector<int>& data);
	void CHARU(const vector<int>& data);
	void ERFENCHARU(const vector<int>& data);
	void XIER(const vector<int>& data);

	void XUANZE(const vector<int>& data);
	void JINBIAOSAI(const vector<int>& data);
	void HEAP(const vector<int>& data);
	void MERGE(const vector<int>& data);
	void BUCKET(const vector<int>& data);

	bool running;
// Dialog Data
	//{{AFX_DATA(CDrawsortDlg)
	enum { IDD = IDD_DRAWSORT_DIALOG };
	CSliderCtrl	m_blocksize;
	CSliderCtrl	m_velocity;
	CListBox	m_typelist;
	CStatic	m_draw;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawsortDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CDrawsortDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnDraw();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnCustomdrawVelocity(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGoon();
	afx_msg void OnPause();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWSORTDLG_H__D8E61482_7DCF_48BC_908B_691481D9F40E__INCLUDED_)
