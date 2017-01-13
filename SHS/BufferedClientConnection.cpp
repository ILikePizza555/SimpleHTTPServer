#include "Sockets.h"

sockets::BufferedClientConnection::BufferedClientConnection(ClientConnection c, char* buffer, size_t length) : ClientConnection(c), buffer(buffer), length(length)
{
}

void sockets::BufferedClientConnection::assign(const char* string, size_t size) const
{
	if (size > length) throw std::overflow_error("Given string is too large for buffer of size " + std::to_string(length) + "!");
	for (int i = 0; i < size; i++) buffer[i] = string[i];
}

void sockets::BufferedClientConnection::assign(std::string data) const
{
	assign(data.c_str(), data.length());
}

void sockets::BufferedClientConnection::clear() const
{
	std::fill(buffer, buffer + length, 0);
}

void sockets::BufferedClientConnection::read()
{
	//Check if the connection is closed
	if (closed) throw SocketException("Error - connection has been closed. ", -1);

	//Read the socket
	int iResult = recv(clientSocket, buffer, length, 0);

	//Check results
	if (iResult > 0) return;
	else if (iResult == 0)
	{
		close();
		return;
	}
	else if (iResult < 0) throw SocketException("Error on recv. Error: ", WSAGetLastError());
}

char * sockets::BufferedClientConnection::data() const
{
	return buffer;
}

size_t sockets::BufferedClientConnection::getLength() const
{
	return length;
}

void sockets::BufferedClientConnection::operator<<(std::string data) const
{
	assign(data);
}

void sockets::BufferedClientConnection::send() const
{
	send(length);
}

void sockets::BufferedClientConnection::send(size_t amount) const
{
	if (closed) throw SocketException("Error - connection has been closed. ", -1);
	if (amount > length) throw std::length_error("Given amount exceeds buffer length!");

	int iResult = ::send(clientSocket, buffer, amount, 0);

	if (iResult == SOCKET_ERROR) throw SocketException("Error on send. Error: ", WSAGetLastError());
}
