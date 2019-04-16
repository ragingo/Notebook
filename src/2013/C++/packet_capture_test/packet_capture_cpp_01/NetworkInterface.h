#pragma once

class NetworkInterface;

typedef boost::shared_ptr<NetworkInterface> NetworkInterfacePtr;
typedef std::vector<NetworkInterfacePtr> NetworkInterfaceCollection;

class NetworkInterface
{
private:
	NetworkInterface();

	NetworkInterface(const PIP_ADAPTER_ADDRESSES& adapter_addrs);

	NetworkInterface(const boost::shared_ptr<IP_ADAPTER_ADDRESSES>& adapter_addrs);

	boost::shared_ptr<IP_ADAPTER_ADDRESSES> m_adapter_addrs;
	static boost::shared_array<IP_ADAPTER_ADDRESSES> m_adapters_addrs;

public:
	~NetworkInterface();

	static NetworkInterfaceCollection GetAllNetworkInterfaces();

	const boost::shared_ptr<IP_ADAPTER_ADDRESSES> GetAdapterInfo() const;
};
