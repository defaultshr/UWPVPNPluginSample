//*********************************************************
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//*********************************************************

#include "pch.h"
#include "TunnelPlugin.h"

using namespace Tasks;

using namespace Platform;
using namespace Windows::Foundation;
using namespace Concurrency;
using namespace Windows::Networking;
using namespace Windows::Networking::Sockets;
using namespace Windows::Storage::Streams;
using namespace Windows::UI::Core;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::Security::Cryptography;
using namespace Windows::Networking::Vpn;
using namespace Platform::Collections;
using namespace Windows::Storage;

// This VPN Plugin callback is triggered from back-ground task when connect is done on the VPN profile.
void TunnelPlugin::Connect(Windows::Networking::Vpn::VpnChannel^ channel)
{
	this->dSock = ref new DatagramSocket();
	channel->AssociateTransport(this->dSock, nullptr);

	auto settings = ApplicationData::Current->LocalSettings->Values;
	Platform::String^ tmpStr = dynamic_cast<String^>(settings->Lookup("LocalTunnelPort"));
	Platform::String^ svcName = ref new String(tmpStr->Data(), tmpStr->Length());
	tmpStr = dynamic_cast<String^>(settings->Lookup("LocalTunnelHost"));
	HostName^ tunnelV4Addr = ref new HostName(ref new String(tmpStr->Data(), tmpStr->Length()));
	// auto result = create_task(dSock->BindEndpointAsync(tunnelV4Addr, svcName));
	auto result = create_task(dSock->BindServiceNameAsync(svcName));
	result.get();

	// Connect to the destination tunnel address on UDP socket.
	tmpStr = dynamic_cast<String^>(settings->Lookup("RemoteTunnelHost"));
	HostName^ remoteTunnelIP = ref new HostName(ref new String(tmpStr->Data(), tmpStr->Length()));
	tmpStr = dynamic_cast<String^>(settings->Lookup("RemoteTunnelPort"));
	Platform::String^ remoteTunnelPort = ref new String(tmpStr->Data(), tmpStr->Length());
	result = create_task(this->dSock->ConnectAsync(remoteTunnelIP, remoteTunnelPort));
	result.get();

	Vector<HostName^>^ localV4Addrs = ref new Vector<HostName^>;
	tmpStr = dynamic_cast<String^>(settings->Lookup("VPNInterfaceV4Addr"));
	localV4Addrs->Append(ref new HostName(ref new String(tmpStr->Data(), tmpStr->Length()))); // Local host name to be bound.

	// IP subnets to be routed via VPN. Only 1 subnet supported in this code. Change code to add more.
	VpnRouteAssignment^ routeScope = ref new VpnRouteAssignment();
	// The subnet mask must match the IP specified. I.e. there must be zero bits after mask.
	tmpStr = dynamic_cast<String^>(settings->Lookup("TunnelSubnetIP"));
	HostName^ routeHost = ref new HostName(ref new String(tmpStr->Data(), tmpStr->Length()));
	int routeMask = _wtoi((dynamic_cast<String^>(settings->Lookup("TunnelSubnetMask")))->Data());
	routeScope->Ipv4InclusionRoutes->Append(ref new VpnRoute(routeHost, routeMask));

	try
	{
		// Enhance the code if IPv6 traffic tunneling and DNS servers are to be specified.
		channel->StartWithMainTransport(localV4Addrs->GetView(), nullptr, nullptr, routeScope, nullptr, 1400, 1500, false, this->dSock);
	}
	catch (Exception^ exc)
	{
		auto type = exc->GetType();
		Platform::String^ str = exc->ToString();
	}
}

// This VPN Plugin callback is triggered from back-ground task when disconnect is done on the VPN profile.
void TunnelPlugin::Disconnect(Windows::Networking::Vpn::VpnChannel^ channel)
{
	channel->Stop(); // TODO: The callback gets called twice on disconnect. 
}

void TunnelPlugin::GetKeepAlivePayload(Windows::Networking::Vpn::VpnChannel ^channel, Windows::Networking::Vpn::VpnPacketBuffer ^*buff)
{

}

// This is just for proof of concept. Simply forwards L3 payload as it is.
// Wireshark capture shows UDP encapsulated L3 payload being sent out.
void TunnelPlugin::Encapsulate(Windows::Networking::Vpn::VpnChannel ^channel, Windows::Networking::Vpn::VpnPacketBufferList ^packets, Windows::Networking::Vpn::VpnPacketBufferList ^encapulatedPackets)
{
	// Right now simply forward whatever comes. 
	while (packets->Size > 0)
	{
		VpnPacketBuffer^ packet = packets->RemoveAtBegin();
		Buffer^ buf = packet->Buffer;
		VpnPacketBufferStatus status = packet->Status;
		encapulatedPackets->Append(packet);
	}
}

// Naive implementation. You may need your own VPN server to send L3 payload encapsulated as it is to see following working.
// Note: Decapsulate is NOT tested. 
void TunnelPlugin::Decapsulate(Windows::Networking::Vpn::VpnChannel ^channel, Windows::Networking::Vpn::VpnPacketBuffer ^encapBuffer, Windows::Networking::Vpn::VpnPacketBufferList ^decapsulatedPackets, Windows::Networking::Vpn::VpnPacketBufferList ^controlPacketsToSend)
{
	// Right now simply forward whatever comes. 
	while (encapBuffer)
	{
		decapsulatedPackets->Append(encapBuffer);
	}
}
