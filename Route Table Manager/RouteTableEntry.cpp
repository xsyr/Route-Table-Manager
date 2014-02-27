#include "stdafx.h"
#include "RouteTableEntry.h"


RouteTableEntry::RouteTableEntry(PMIB_IPFORWARDROW entry, PIP_ADAPTER_ADDRESSES adapter)
	:_Entry(entry), _Adapter(adapter)
{
	Ipv4AddressToString(this->_DestNetworkIP, 20, entry->dwForwardDest);
	Ipv4AddressToString(this->_SubnetMark, 20, entry->dwForwardMask);
	Ipv4AddressToString(this->_GatewayIP, 20, entry->dwForwardNextHop);

	sockaddr_in* unicastAddr = ((sockaddr_in*)adapter->FirstUnicastAddress->Address.lpSockaddr);

	Ipv4AddressToString(this->_InterfaceIP, 20, unicastAddr->sin_addr.S_un.S_addr);

	StringCchPrintf(this->_Metric, 5, TEXT("%d"), entry->dwForwardMetric1);
}

TCHAR* RouteTableEntry::Ipv4AddressToString(TCHAR* buffer, DWORD dwSize, DWORD ip){
		sockaddr_in addr;
		addr.sin_addr.S_un.S_addr = ip;
		addr.sin_family = AF_INET;
		addr.sin_port = 0;

		WSAAddressToString(
			(LPSOCKADDR)&addr, 
			sizeof(sockaddr_in),
			NULL,
			buffer,
			&dwSize
		);

		return buffer;
	}

RouteTableEntry::~RouteTableEntry(void)
{
}
