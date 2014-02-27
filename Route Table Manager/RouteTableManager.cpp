#include "stdafx.h"
#include "RouteTableManager.h"


RouteTableManager::RouteTableManager(void)
	:_AdapterAddresses(NULL), _RouteTable(NULL)
{
	this->GetRouteTableEntries();
}

PIP_ADAPTER_ADDRESSES RouteTableManager::GetAdapterAddressByInterfaceId(
	PIP_ADAPTER_ADDRESSES head, 
	DWORD index){
		while (head)
		{
			if(head->IfIndex == index)
				break;

			head = head->Next;
		}

		return head;
	}

void RouteTableManager::GetRouteTableEntries(){
	if( NULL != this->_RouteTable ){
		free(this->_RouteTable);
	}

	if( NULL != this->_AdapterAddresses ){
		free(this->_AdapterAddresses);
	}

	for (auto begin = this->_Entries.begin(); begin != this->_Entries.end(); begin++)
	{
		delete *begin;
	}

	this->_Entries.clear();

	DWORD dwSize = 0;
	DWORD ret = NO_ERROR;
	ret = GetIpForwardTable(this->_RouteTable, &dwSize, TRUE);
	if(ERROR_INSUFFICIENT_BUFFER == ret){
		this->_RouteTable = (PMIB_IPFORWARDTABLE)calloc(1, dwSize);
		ret = GetIpForwardTable(this->_RouteTable, &dwSize, TRUE);
		if( NO_ERROR != ret){
			throw std::exception("GetIpForwardTable() failed.");
		}
	}

	ret = GetAdaptersAddresses(AF_INET, 0, NULL, this->_AdapterAddresses, &dwSize);
	if( ERROR_BUFFER_OVERFLOW == ret){
		this->_AdapterAddresses = (PIP_ADAPTER_ADDRESSES)calloc(1, dwSize);
		ret = GetAdaptersAddresses(AF_INET, 0, NULL, this->_AdapterAddresses, &dwSize);
		if( NO_ERROR != ret){
			throw std::exception("GetAdaptersAddresses() failed.");
		}
	}

	for (int i = 0; i < this->_RouteTable->dwNumEntries; i++)
	{
		PMIB_IPFORWARDROW entry = &this->_RouteTable->table[i];
		PIP_ADAPTER_ADDRESSES adapter = 
			GetAdapterAddressByInterfaceId(this->_AdapterAddresses, entry->dwForwardIfIndex);
		this->_Entries.push_back(new RouteTableEntry(entry, adapter));
	}

}

RouteTableManager::~RouteTableManager(void)
{
	free(this->_RouteTable);
	free(this->_AdapterAddresses);
	for (auto begin = this->_Entries.begin(); begin != this->_Entries.end(); begin++)
	{
		delete *begin;
	}
}
