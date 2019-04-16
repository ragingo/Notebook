#include "StdAfx.h"
#include "NetworkInterface.h"

boost::shared_array<IP_ADAPTER_ADDRESSES> NetworkInterface::m_adapters_addrs;

NetworkInterface::NetworkInterface()
{
}


NetworkInterface::~NetworkInterface()
{
}

NetworkInterface::NetworkInterface(const PIP_ADAPTER_ADDRESSES& adapter_addrs)
{
	this->m_adapter_addrs = boost::shared_ptr<IP_ADAPTER_ADDRESSES>(adapter_addrs);
}

NetworkInterface::NetworkInterface(const boost::shared_ptr<IP_ADAPTER_ADDRESSES>& adapter_addrs)
{
	this->m_adapter_addrs = adapter_addrs;
}

NetworkInterfaceCollection NetworkInterface::GetAllNetworkInterfaces()
{
	if (m_adapters_addrs)
	{
		m_adapters_addrs.reset();
	}

	NetworkInterfaceCollection results;

	DWORD adapterCount;
 
	ULONG ret = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, NULL, &adapterCount);

	if (ret != ERROR_BUFFER_OVERFLOW)
	{
		return results;
	}

	auto adapters = new IP_ADAPTER_ADDRESSES[adapterCount];

	m_adapters_addrs = boost::shared_array<IP_ADAPTER_ADDRESSES>(adapters);
 
	ret = GetAdaptersAddresses(AF_UNSPEC, GAA_FLAG_INCLUDE_PREFIX, NULL, adapters, &adapterCount);

	if (ret != ERROR_SUCCESS)
	{
		return results;
	}

	if (adapterCount == 0)
	{
		return results;
	}
 
	auto adapter = adapters;

	while (adapter)
	{
		NetworkInterfacePtr nic(new NetworkInterface(new IP_ADAPTER_ADDRESSES(*adapter)));
		results.push_back(nic);

		adapter = adapter->Next;
	}

	return results;
}

const boost::shared_ptr<IP_ADAPTER_ADDRESSES> NetworkInterface::GetAdapterInfo() const
{
	return this->m_adapter_addrs;
}
