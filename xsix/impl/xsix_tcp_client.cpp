#include "xsix/xsix_tcp_client.h"
#include "xsix/xsix_timestamp.h"
#include <stdio.h>

namespace xsix
{

	TCPClient::~TCPClient()
	{
		m_ClientSock.Close();
	}

	bool TCPClient::Connect(const NetAddr& addr)
	{
		m_RemoteAddr = addr;
		return m_ClientSock.Connect(addr);
	}

	void TCPClient::PushSendBytes(const char* src, int32_t n)
	{
		if (!src || n <= 0)
		{
			return;
		}

		if (!m_ClientSock.IsValid())
		{
			return;
		}

		m_SendBuffer.Write(src, n);
	}

	void TCPClient::Run()
	{
		while (!m_bQuit)
		{
			Timestamp::PrintNow();
			RunSend();
			RunRecv();	
		}
	}

	void TCPClient::RunSend()
	{
		if (!m_ClientSock.IsValid())
		{
			m_bQuit = true;
			return;
		}

		if (m_SendBuffer.Len() <= 0)
		{
			return;
		}

		char szSendBytes[1024] = { 0 };
		int32_t n = m_SendBuffer.Read(szSendBytes, 1024);
		if (n > 0)
		{
			int32_t rc = m_ClientSock.SendBytes(szSendBytes, n);
			if (rc == SOCKET_ERROR)
			{
				m_bQuit = true;
				return;
			}
		}	
	}

	void TCPClient::RunRecv()
	{
		if (!m_ClientSock.IsValid())
		{
			m_bQuit = true;
			return;
		}

		char szRecvBytes[1024] = { 0 };
		int32_t rc = m_ClientSock.RecvBytes(szRecvBytes, 1024);
		if (rc == SOCKET_ERROR)
		{
			m_bQuit = true;
			return;
		}

		if (rc == 0)
		{
			m_bQuit = true;
			printf("remote(%s:%d) close\n", m_RemoteAddr.IP().c_str(), m_RemoteAddr.Port());
			return;
		}

		//TODO:Recv CallBack
		m_RecvBuffer.Write(szRecvBytes, rc);
		szRecvBytes[rc] = '\0';
		printf("recv : %s\n", szRecvBytes);
	}

}

