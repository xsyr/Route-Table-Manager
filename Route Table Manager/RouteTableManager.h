#pragma once
#include "RouteTableEntry.h"


class RouteTableManager
{
private:
	PIP_ADAPTER_ADDRESSES _AdapterAddresses;
	PMIB_IPFORWARDTABLE _RouteTable;
	std::vector<RouteTableEntry*> _Entries;

	void GetRouteTableEntries();
	PIP_ADAPTER_ADDRESSES GetAdapterAddressByInterfaceId(PIP_ADAPTER_ADDRESSES head, DWORD index);


public:
	RouteTableManager(void);

	size_t GetEntryCount() const{
		return this->_Entries.size();
	}

	RouteTableEntry* operator [](int index){
		if( index + 1 > this->GetEntryCount() ){
			throw std::exception("Out of range.");
		}

		return this->_Entries[index];
	}

	void Update(){
		this->GetRouteTableEntries();
	}

	virtual ~RouteTableManager(void);
};

