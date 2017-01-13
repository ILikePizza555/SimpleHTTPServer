#include "Sockets.h"

sockets::ClientConnection::ClientConnection(SOCKET& cs, sockaddr_storage addr, socklen_t len, size_t bufferSize) : clientSocket(cs), addr(addr), len(len), closed(false), buffer(bufferSize)
{
}

void sockets::ClientConnection::close()
{
	closed = true;
	closesocket(clientSocket);
	return;
}

void sockets::ClientConnection::read()
{
	//Check if the connection is closed
	if (closed) throw SocketException("Error - connection has been closed. ", -1);

	//Read the socket
	int iResult = recv(clientSocket, buffer.data(), buffer.getLength(), 0);

	//Check results
	if (iResult > 0) return;
	else if (iResult == 0)
	{
		close();
		return;
	}
	else if (iResult < 0) throw SocketException("Error on recv. Error: ", WSAGetLastError());
}

void sockets::ClientConnection::send()
{
	send(buffer.getLength());
}

void sockets::ClientConnection::send(size_t amount)
{
	if (closed) throw SocketException("Error - connection has been closed. ", -1);
	if (amount > buffer.getLength()) throw std::length_error("Given amount exceeds buffer length!");

	int iResult = ::send(clientSocket, buffer.data(), amount, 0);

	if (iResult == SOCKET_ERROR) throw SocketException("Error on send. Error: ", WSAGetLastError());
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
		auto s = reinterpret_cast<sockaddr_in*>(&addr);
		inet_ntop(AF_INET, &s->sin_addr, ipstr, sizeof ipstr);
	}
	else
	{
		//IPv6
		auto s = reinterpret_cast<sockaddr_in6 *>(&addr);
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
