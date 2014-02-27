// RouteEntryInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Route Table Manager.h"
#include "RouteEntryInfoDlg.h"
#include "afxdialogex.h"



// CRouteEntryInfoDlg dialog

IMPLEMENT_DYNAMIC(CRouteEntryInfoDlg, CDialogEx)

CRouteEntryInfoDlg::CRouteEntryInfoDlg(CWnd* pParent /*=NULL*/, RouteTableEntry* entry)
	: CDialogEx(CRouteEntryInfoDlg::IDD, pParent)
	, _Metric(0), _Entry(entry)
{
	if(NULL != this->_Entry){
		this->_CurrentInterface = this->_Entry->GetAdapterAddress()->IfIndex;
	}

	this->_CurrentInterfaceRow = (PMIB_IPINTERFACE_ROW)calloc(1, sizeof(MIB_IPINTERFACE_ROW));
}

CRouteEntryInfoDlg::~CRouteEntryInfoDlg()
{
	free(this->_CurrentInterfaceRow);
}

void CRouteEntryInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_INTERFACE, _CbInterfaces);
	DDX_Control(pDX, IDC_EDIT_METRIC, _CeditMetric);
	DDX_Text(pDX, IDC_EDIT_METRIC, _Metric);

	this->_CurrentInterfaceRow->Family = AF_INET;
	this->_CurrentInterfaceRow->InterfaceIndex = this->_CurrentInterface;
	int status = GetIpInterfaceEntry(this->_CurrentInterfaceRow);

	DDV_MinMaxUInt(pDX, _Metric, this->_CurrentInterfaceRow->Metric, 512);

	DDX_Control(pDX, IDC_IP_DESTNETWORK, _DestNetWorkIP);
	DDX_Control(pDX, IDC_IP_GATEWAY, _GatewayIP);
	DDX_Control(pDX, IDC_IP_MARK, _SubnetMark);
	DDX_Control(pDX, IDOK, _BtnOk);
}


BEGIN_MESSAGE_MAP(CRouteEntryInfoDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_INTERFACE, &CRouteEntryInfoDlg::OnSelchangeComboInterface)
END_MESSAGE_MAP()


// CRouteEntryInfoDlg message handlers


void CRouteEntryInfoDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	int ret = this->_DestNetWorkIP.GetAddress(this->_NwIp);
	if(4 != ret ){
		this->MessageBox(TEXT("ÇëÌîÐ´Ä¿±êÍøÂçIP"));
		return;
	}
	this->_NwIp = ntohl(this->_NwIp);


	ret = this->_SubnetMark.GetAddress(this->_SnMask);
	if(4 != ret ){
		this->MessageBox(TEXT("ÇëÌîÐ´ÍøÂçÑÚÂë"));
		return;
	}
	this->_SnMask = ntohl(this->_SnMask);


	ret = this->_GatewayIP.GetAddress(this->_GwIp);
	if(4 != ret ){
		this->MessageBox(TEXT("ÇëÌîÐ´Íø¹ØIP"));
		return;
	}
	this->_GwIp = ntohl(this->_GwIp);

	this->_CurrentInterface = this->_Interfaces.find(this->_CbInterfaces.GetCurSel())->second;

	CDialogEx::OnOK();
}

bool CRouteEntryInfoDlg::IsDirty(){
	bool dirty = false;
	if(NULL == this->_Entry){
		dirty = true;
	}

	if( this->_NwIp != this->_Entry->GetEntry()->dwForwardDest
		|| this->_GwIp != this->_Entry->GetEntry()->dwForwardNextHop
		|| this->_SnMask != this->_Entry->GetEntry()->dwForwardMask
		|| this->_Metric != this->_Entry->GetEntry()->dwForwardMetric1
		|| this->_Entry->GetAdapterAddress()->IfIndex 
			!= this->_CurrentInterface
		){
			dirty = true;
	}

	dirty = dirty ? true : this->_Entry->GetAdapterAddress()->IfIndex 
			!= this->_CurrentInterface;

	return dirty;
}

void CRouteEntryInfoDlg::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialogEx::OnCancel();
}


BOOL CRouteEntryInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	if(NULL != this->_Entry){
		TCHAR buffer[20];
		auto entry = this->_Entry->GetEntry();
		auto adapter = this->_Entry->GetAdapterAddress();
		this->_DestNetWorkIP.SetAddress(htonl(entry->dwForwardDest));
		this->_SubnetMark.SetAddress(htonl(entry->dwForwardMask));
		this->_GatewayIP.SetAddress(htonl(entry->dwForwardNextHop));

		StringCchPrintf(buffer, 20, TEXT("%d"), entry->dwForwardMetric1);
		this->_CeditMetric.SetWindowText(buffer);

		this->_BtnOk.SetWindowText(TEXT("±£´æ"));
	}

	DWORD dwSize = 0;
	PIP_ADAPTER_ADDRESSES adapterAddress = NULL, head = NULL;
	DWORD ret = GetAdaptersAddresses(AF_INET, 0, NULL, adapterAddress, &dwSize);
	if( ERROR_BUFFER_OVERFLOW == ret){
		adapterAddress = (PIP_ADAPTER_ADDRESSES)calloc(1, dwSize);
		ret = GetAdaptersAddresses(AF_INET, 0, NULL, adapterAddress, &dwSize);
		if( NO_ERROR != ret){
			throw std::exception("GetAdaptersAddresses() failed.");
		}
		head = adapterAddress;
	}

	for (int i = 0; NULL != head; i++)
	{
		this->_Interfaces.insert(std::make_pair(i, head->IfIndex));
		this->_CbInterfaces.AddString(head->FriendlyName);
		head = head->Next;
	}

	if(NULL != adapterAddress){
		if(NULL != this->_Entry){
			this->_CbInterfaces.SelectString(0, this->_Entry->GetAdapterAddress()->FriendlyName);
		}
		else{
			this->_CbInterfaces.SetCurSel(0);
			this->_CurrentInterface = this->_Interfaces.find(0)->second;
		}

		free(adapterAddress);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CRouteEntryInfoDlg::OnSelchangeComboInterface()
{
	this->_CurrentInterface = this->_Interfaces.find(this->_CbInterfaces.GetCurSel())->second;
	// TODO: Add your control notification handler code here
}
