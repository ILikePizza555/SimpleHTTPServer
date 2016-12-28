#include "Sockets.h"

sockets::ServerSocket::ServerSocket(std::string ip, std::string port) : ip(ip), port(port) {
	//Define iResult for calling the result functions
	int iResult = 0;

	//Get the socket hints about this machine
	addrinfo hints, *serverinfo;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET6;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	if ((iResult = getaddrinfo(ip.c_str(), port.c_str(), &hints, &serverinfo)) != 0)
		throw SocketException("Error on getaddrinfo. Error: ", iResult);

	//Create the socket
	if ((listenSocket = socket(serverinfo->ai_family, serverinfo->ai_socktype, serverinfo->ai_protocol)) == INVALID_SOCKET)
		throw SocketException("Error on socket. Error: ", WSAGetLastError());

	//Enable ipv4 mapping on the socket
	int no = 0;
	if ((iResult = setsockopt(listenSocket, IPPROTO_IPV6, IPV6_V6ONLY, (char *)&no, sizeof(no))) == SOCKET_ERROR)
		throw SocketException("Error on setsockopt. Error: ", WSAGetLastError());

	//Bind the socket
	if ((iResult = bind(listenSocket, serverinfo->ai_addr, (int)serverinfo->ai_addrlen)) == SOCKET_ERROR)
		throw SocketException("Error on bind. Error: ", WSAGetLastError());

	//Free memory we don't need
	freeaddrinfo(serverinfo);
}

sockets::ServerSocket::~ServerSocket() {
	closesocket(listenSocket);
}

void sockets::ServerSocket::listen(int backlog = SOMAXCONN) {
	if (::listen(listenSocket, backlog) == SOCKET_ERROR) throw SocketException("Error on listen. Error: ", WSAGetLastError());
}

sockets::ClientConnection sockets::ServerSocket::accept(int bufferSize = DEFAULT_BUFFER_SIZE) {
	SOCKET clientSocket = INVALID_SOCKET;

	if ((clientSocket = ::accept(listenSocket, NULL, NULL)) == INVALID_SOCKET) throw SocketException("Error on accept. Error: ", WSAGetLastError());

	sockets::ClientConnection rv(clientSocket);
	return rv;
}