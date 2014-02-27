#pragma once
class RouteTableEntry
{
private:
	PMIB_IPFORWARDROW _Entry;
	PIP_ADAPTER_ADDRESSES _Adapter;
	TCHAR _DestNetworkIP[20];
	TCHAR _SubnetMark[20];
	TCHAR _GatewayIP[20];
	TCHAR _InterfaceIP[20];
	TCHAR _Metric[5];

	TCHAR* Ipv4AddressToString(TCHAR* buffer, DWORD dwSize, DWORD ip);

public:
	RouteTableEntry(PMIB_IPFORWARDROW entry, PIP_ADAPTER_ADDRESSES adapter);

	LPCTSTR GetDestNetworkIP() const{
		return this->_DestNetworkIP;
	}

	LPCTSTR GetSubnetMark() const{
		return this->_SubnetMark;
	}

	LPCTSTR GetGatewayIP() const{
		return this->_GatewayIP;
	}

	LPCTSTR GetInterfaceIP() const{
		return this->_InterfaceIP;
	}

	LPCTSTR GetMetric() const{
		return this->_Metric;
	}

	LPCTSTR GetAdapterName() const{
		return this->_Adapter->FriendlyName;
	}

	PMIB_IPFORWARDROW GetEntry() const{
		return this->_Entry;
	}

	PIP_ADAPTER_ADDRESSES GetAdapterAddress() const{
		return this->_Adapter;
	}

	~RouteTableEntry(void);
};

