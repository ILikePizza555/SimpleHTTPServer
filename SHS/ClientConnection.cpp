#include "Sockets.h"

sockets::ClientConnection::ClientConnection(SOCKET& cs, size_t bufferSize) : clientSocket(cs), closed(false), buffer(bufferSize) {}

void sockets::ClientConnection::close() {
	closed = true;
	closesocket(clientSocket);
	return;
}

void sockets::ClientConnection::read() {
	//Check if the connection is closed
	if(closed) throw SocketException("Error - connection has been closed. ", -1);
	
	//Read the socket
	int iResult = recv(clientSocket, buffer.data(), buffer.getLength(), 0);

	//Check results
	if (iResult > 0) return;
	else if (iResult == 0) { 
		close();
		return;
	}
	else if (iResult < 0) throw SocketException("Error on recv. Error: ", WSAGetLastError());
}

void sockets::ClientConnection::send() {
	if (closed) throw SocketException("Error - connection has been closed. ", -1);

	int iResult = ::send(clientSocket, buffer.data(), buffer.getLength(), 0);

	if(iResult == SOCKET_ERROR) throw SocketException("Error on send. Error: ", WSAGetLastError());
}

bool sockets::ClientConnection::isClosed() {
	return closed;
}

void sockets::ClientConnection::shutdown() {
	if(closed) return;

	::shutdown(clientSocket, SD_SEND);
	close();
}