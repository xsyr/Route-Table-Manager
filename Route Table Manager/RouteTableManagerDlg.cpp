
// RouteTableManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Route Table Manager.h"
#include "RouteTableManagerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
	EnableActiveAccessibility();
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRouteTableManagerDlg dialog



CRouteTableManagerDlg::CRouteTableManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRouteTableManagerDlg::IDD, pParent)
{
	EnableActiveAccessibility();
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRouteTableManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ENTRYLIST, this->_EntryList);
	DDX_Control(pDX, IDC_BTN_DELETE_ENTRY, _BtnDeleteEntry);
}

BEGIN_MESSAGE_MAP(CRouteTableManagerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(NM_DBLCLK, IDC_ENTRYLIST, &CRouteTableManagerDlg::OnDblclkEntrylist)
	ON_BN_CLICKED(IDC_BTN_DELETE_ENTRY, &CRouteTableManagerDlg::OnClickedBtnDeleteEntry)
	ON_BN_CLICKED(IDC_BTN_ADD_ENTRY, &CRouteTableManagerDlg::OnClickedBtnAddEntry)
	ON_NOTIFY(NM_CLICK, IDC_ENTRYLIST, &CRouteTableManagerDlg::OnClickEntrylist)
END_MESSAGE_MAP()


// CRouteTableManagerDlg message handlers

BOOL CRouteTableManagerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	this->_EntryList.InsertColumn(0, TEXT(""), LVCFMT_CENTER, 30);
	this->_EntryList.InsertColumn(1, TEXT("网络目标"), LVCFMT_RIGHT, 110);
	this->_EntryList.InsertColumn(2, TEXT("网络掩码"), LVCFMT_RIGHT, 110);
	this->_EntryList.InsertColumn(3, TEXT("网关"), LVCFMT_RIGHT, 110);
	this->_EntryList.InsertColumn(4, TEXT("接口IP"), LVCFMT_RIGHT, 110);
	this->_EntryList.InsertColumn(5, TEXT("跃点数"), LVCFMT_CENTER, 50);
	this->_EntryList.InsertColumn(6, TEXT("接口名称"), LVCFMT_LEFT, 253);
	this->_EntryList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);

	this->RefreshRouteEntries();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CRouteTableManagerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CRouteTableManagerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CRouteTableManagerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CRouteTableManagerDlg::RefreshRouteEntries(){
	TCHAR buffer[50];

	this->_EntryList.SetRedraw(FALSE);
	this->_EntryList.DeleteAllItems();

	this->_Manager.Update();
	for (int i = 0; i < this->_Manager.GetEntryCount(); i++)
	{
		RouteTableEntry* entry = this->_Manager[i];

		// 当前 Entry 编号
		StringCchPrintf(buffer, ARRAYSIZE(buffer), TEXT("%d"), i +1);
		this->_EntryList.InsertItem(LVIF_TEXT|LVIF_STATE, i, buffer, 0, 0, 0, NULL);
		this->_EntryList.SetItemText(i, 1, entry->GetDestNetworkIP());
		this->_EntryList.SetItemText(i, 2, entry->GetSubnetMark());
		this->_EntryList.SetItemText(i, 3, entry->GetGatewayIP());
		this->_EntryList.SetItemText(i, 4, entry->GetInterfaceIP());
		this->_EntryList.SetItemText(i, 5, entry->GetMetric());
		this->_EntryList.SetItemText(i, 6, entry->GetAdapterName());
	}

	this->_EntryList.SetRedraw(TRUE);
	this->_EntryList.Invalidate();
	this->_EntryList.UpdateWindow();

}

void CRouteTableManagerDlg::OnOK(){
	this->RefreshRouteEntries();
}

void CRouteTableManagerDlg::OnDblclkEntrylist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	auto entry = this->_Manager[pNMItemActivate->iItem];
	CRouteEntryInfoDlg dlg(NULL, entry);
	int ret = dlg.DoModal();
	if( IDOK == ret ){
		bool isDirty = dlg.IsDirty();

		if(isDirty){
			auto row = entry->GetEntry();
			DWORD status = NO_ERROR;
			status = DeleteIpForwardEntry(row);

			if(NO_ERROR == status ){
				//status = CreateIpForwardEntry(row);
				//assert(NO_ERROR == status);

				auto orig = (PMIB_IPFORWARDROW)calloc(1, sizeof(MIB_IPFORWARDROW));
				memcpy(orig, row, sizeof(MIB_IPFORWARDROW));

				row->dwForwardProto = MIB_IPPROTO_NETMGMT;
				row->dwForwardDest = dlg.GetDestNetworkIp();
				row->dwForwardMask = dlg.GetSubnetMask();
				row->dwForwardIfIndex = dlg.GetInterfaceIndex();
				row->dwForwardNextHop = dlg.GetGatewayIp();
				row->dwForwardMetric1 = dlg.GetMetric();
				status = CreateIpForwardEntry(row);
				if(NO_ERROR != status){
					status = CreateIpForwardEntry(orig);
					memcpy(row, orig, sizeof(MIB_IPFORWARDROW));
					this->MessageBox(TEXT("保存失败！"));
				}
				else{
					this->RefreshRouteEntries();
				}

				free(orig);
			}
			else{
				this->MessageBox(TEXT("保存失败！"));
			}
		}
	}

	*pResult = 0;
}


void CRouteTableManagerDlg::OnClickedBtnDeleteEntry()
{
	// TODO: Add your control notification handler code here
	TCHAR message[50];
	POSITION pos = this->_EntryList.GetFirstSelectedItemPosition();

	StringCchPrintf(
		message, 
		50, TEXT("您确定要删除第%d项吗？"), 
		pos
	);

	int ret = this->MessageBox(
		message, 
		NULL, 
		MB_ICONEXCLAMATION | MB_ICONWARNING | MB_OKCANCEL
	);

	if(IDOK == ret){
		auto entry = this->_Manager[(int)pos - 1];
		auto row = entry->GetEntry();
		DWORD status = NO_ERROR;
		status = DeleteIpForwardEntry(row);
		if(NO_ERROR != status){
			this->MessageBox(TEXT("删除失败！"));
		}
		else{
			this->RefreshRouteEntries();
		}
	}
}


void CRouteTableManagerDlg::OnClickedBtnAddEntry()
{
	// TODO: Add your control notification handler code here
	MIB_IPFORWARDROW row;
	CRouteEntryInfoDlg dlg(NULL, NULL);
	int ret = dlg.DoModal();
	if( IDOK == ret ){
		memset(&row, 0, sizeof(MIB_IPFORWARDROW));
		row.dwForwardProto = MIB_IPPROTO_NETMGMT;
		row.dwForwardDest = dlg.GetDestNetworkIp();
		row.dwForwardMask = dlg.GetSubnetMask();
		row.dwForwardIfIndex = dlg.GetInterfaceIndex();
		row.dwForwardNextHop = dlg.GetGatewayIp();
		row.dwForwardMetric1 = dlg.GetMetric();
		DWORD status = CreateIpForwardEntry(&row);
		if(ERROR_INVALID_PARAMETER == status){
			this->MessageBox(TEXT("参数有误！"));
			return ;
		}

		if(NO_ERROR != status){
			this->MessageBox(TEXT("添加失败！"));
			return ;
		}

		this->RefreshRouteEntries();
	}
}



void CRouteTableManagerDlg::OnClickEntrylist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	if( -1 == pNMItemActivate->iItem ){
		this->_BtnDeleteEntry.EnableWindow(FALSE);
	}
	else{
		auto entry = this->_Manager[pNMItemActivate->iItem];
		if(0 == _tcscmp(_T("127.0.0.1"), entry->GetGatewayIP()) )
			this->_BtnDeleteEntry.EnableWindow(FALSE);
		else
			this->_BtnDeleteEntry.EnableWindow(TRUE);
	}

	*pResult = 0;
}
