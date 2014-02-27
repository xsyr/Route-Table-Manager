#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "RouteTableEntry.h"


// CRouteEntryInfoDlg dialog

class CRouteEntryInfoDlg : public CDialogEx
{
private:
	DWORD _NwIp;
	DWORD _GwIp;
	DWORD _SnMask;
	DWORD _CurrentInterface;
	std::map<int, DWORD> _Interfaces;
	PMIB_IPINTERFACE_ROW _CurrentInterfaceRow;

	RouteTableEntry *_Entry;
	DECLARE_DYNAMIC(CRouteEntryInfoDlg)

public:
	CRouteEntryInfoDlg(CWnd* pParent = NULL, RouteTableEntry* entry = NULL);   // standard constructor
	virtual ~CRouteEntryInfoDlg();

// Dialog Data
	enum { IDD = IDD_ENTRYINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CComboBox _CbInterfaces;
	CEdit _CeditMetric;
	UINT _Metric;
	CIPAddressCtrl _DestNetWorkIP;
	CIPAddressCtrl _GatewayIP;
	CIPAddressCtrl _SubnetMark;
	virtual void OnOK();
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()
public:

	DWORD GetDestNetworkIp() const{
		return this->_NwIp;
	}

	DWORD GetGatewayIp() const{
		return this->_GwIp;
	}

	DWORD GetSubnetMask() const{
		return this->_SnMask;
	}

	DWORD GetMetric() const{
		return this->_Metric;
	}

	DWORD GetInterfaceIndex() const {
		return this->_CurrentInterface;
	}

	bool IsDirty();
	virtual BOOL OnInitDialog();
	CButton _BtnOk;
	afx_msg void OnSelchangeComboInterface();
};
