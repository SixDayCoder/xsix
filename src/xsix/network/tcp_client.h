#pragma  once

#include "xsix/network/network.h"
#include "xsix/buffer.h"

namespace xsix
{
	class TCPClient
	{
	public:

		TCPClient() : m_ClientSock(), 
					  m_SendBuffer(), 
					  m_RecvBuffer(), 
					  m_bQuit(false) {}

		virtual ~TCPClient();
	
	public:

		bool Connect(const NetAddr& addr);

		void PushSendBytes(const char* src, int32_t n);

		void Run();

		const NetAddr& GetRemoteAddr() const { return m_RemoteAddr; }

	private:

		void RunSend();

		void RunRecv();

	private:

		xsix::ClientSocket m_ClientSock;

		xsix::buffer m_SendBuffer;

		xsix::buffer m_RecvBuffer;

		xsix::NetAddr m_RemoteAddr;

		bool m_bQuit;
	};
}