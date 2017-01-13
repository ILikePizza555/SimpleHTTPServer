#include "Sockets.h"

sockets::ClientConnection::ClientConnection(SOCKET& cs, sockaddr_storage addr, socklen_t len) : clientSocket(cs), addr(addr), len(len), closed(false)
{
}

void sockets::ClientConnection::close()
{
	closed = true;
	closesocket(clientSocket);
	return;
}

bool sockets::ClientConnection::isClosed() const
{
	return closed;
}

std::string sockets::ClientConnection::getIp()
{
	char ipstr[INET6_ADDRSTRLEN];

	if (addr.ss_family == AF_INET)
	{
		//IPv4
		sockaddr_in* s = (sockaddr_in*)&addr;
		inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
	}
	else
	{
		//IPv6
		sockaddr_in6* s = (sockaddr_in6 *)&addr;
		inet_ntop(AF_INET6, &s->sin6_addr, ipstr, sizeof ipstr);
	}

	return std::string(ipstr);
}

void sockets::ClientConnection::shutdown()
{
	if (closed) return;

	::shutdown(clientSocket, SD_SEND);
	close();
}
