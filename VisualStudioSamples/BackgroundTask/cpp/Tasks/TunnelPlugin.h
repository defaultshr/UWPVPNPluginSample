//*****************************************************************************
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//*****************************************************************************
#pragma once

namespace Tasks
{
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class TunnelPlugin sealed : Windows::Networking::Vpn::IVpnPlugIn
	{
	public:
		virtual ~TunnelPlugin() {};
		TunnelPlugin(Windows::Networking::Sockets::DatagramSocket^ sock)
		{
			this->dSock = sock;
		}
		virtual void Connect(Windows::Networking::Vpn::VpnChannel^ channel);
		virtual void Disconnect(Windows::Networking::Vpn::VpnChannel^ channel);
		virtual void GetKeepAlivePayload(Windows::Networking::Vpn::VpnChannel ^channel, Windows::Networking::Vpn::VpnPacketBuffer ^*keepAlivePacket);
		virtual void Encapsulate(Windows::Networking::Vpn::VpnChannel ^channel, Windows::Networking::Vpn::VpnPacketBufferList ^packets, Windows::Networking::Vpn::VpnPacketBufferList ^encapulatedPackets);
		virtual void Decapsulate(Windows::Networking::Vpn::VpnChannel ^channel, Windows::Networking::Vpn::VpnPacketBuffer ^encapBuffer, Windows::Networking::Vpn::VpnPacketBufferList ^decapsulatedPackets, Windows::Networking::Vpn::VpnPacketBufferList ^controlPacketsToSend);
	private:
		void ReadMessage(Windows::Networking::Sockets::DatagramSocketMessageReceivedEventArgs^ eventArguments);
		Windows::Storage::Streams::IOutputStream^ oStream;
		Windows::Networking::Sockets::DatagramSocket^ dSock;
		CRITICAL_SECTION lock;
	};
}