#include "Sockets.h"

sockets::ClientConnection::ClientConnection(SOCKET& cs) : clientSocket(cs), closed(false), writeBuffer(std::vector<char>()), readBuffer(std::vector<char>()) {

}

void sockets::ClientConnection::close() {
	closed = true;
	closesocket(clientSocket);
	return;
}

void sockets::ClientConnection::read(size_t amount = DEFAULT_BUFFER_SIZE) {
	if(closed) throw SocketException("Error - connection has been closed. ", -1);
	
	readBuffer.reserve(amount);
	int iResult = recv(clientSocket, readBuffer.data(), readBuffer.capacity(), 0);

	if (iResult > 0) return;
	else if (iResult == 0) { 
		close();
		return;
	}
	else if (iResult < 0) throw SocketException("Error on recv. Error: ", WSAGetLastError());
}

void sockets::ClientConnection::send() {
	send(writeBuffer.capacity());
}

void sockets::ClientConnection::send(int amount) {
	if (closed) throw SocketException("Error - connection has been closed. ", -1);

	int iResult = ::send(clientSocket, writeBuffer.data(), amount, 0);

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