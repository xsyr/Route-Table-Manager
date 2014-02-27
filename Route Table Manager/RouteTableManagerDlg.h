
// RouteTableManagerDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "RouteTableManager.h"
#include "RouteEntryInfoDlg.h"


// CRouteTableManagerDlg dialog
class CRouteTableManagerDlg : public CDialogEx
{
private:
	CListCtrl _EntryList;

	RouteTableManager _Manager;

	void RefreshRouteEntries();

// Construction
public:
	CRouteTableManagerDlg(CWnd* pParent = NULL);	// standard constructor
	~CRouteTableManagerDlg(){
	}

// Dialog Data
	enum { IDD = IDD_ROUTETABLEMANAGER_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDblclkEntrylist(NMHDR *pNMHDR, LRESULT *pResult);
//	virtual void OnCancel();
	afx_msg void OnClickedBtnDeleteEntry();
	afx_msg void OnClickedBtnAddEntry();
//	afx_msg void OnClickedBtnEditEntry();
	afx_msg void OnClickEntrylist(NMHDR *pNMHDR, LRESULT *pResult);
	CButton _BtnDeleteEntry;
};
