#include "Http.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#define IPv4_ANY "0.0.0.0"
#define IPv6_ANY "::"
#define DEFAULT_PORT "80"
#define BUFFERLEN_DEFAULT 512

int main(int argc, char* argv) {
	int iResult; //Variable to hold the results of the C-Style functions

	//Start up WinSock
	WSADATA wsaData;

	iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed: %d\n", iResult);
		return 1;
	}

	//Create the listen socket
	addrinfo *result = nullptr, hints;
	SOCKET listenSocket = INVALID_SOCKET;

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET6;			//IPv6, we'll enable ipv4 connections later
	hints.ai_socktype = SOCK_STREAM;	//Stream socket
	hints.ai_protocol = IPPROTO_TCP;	//TCP
	hints.ai_flags = AI_PASSIVE;		//We're gona listen on this server

	iResult = getaddrinfo(IPv6_ANY, DEFAULT_PORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %d\n", iResult);
		return 1;
	}

	listenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (listenSocket == INVALID_SOCKET) {
		printf("socket failed: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		return 1;
	}

	//Enable ipv4 mapping for this socket
	int no = 0;
	iResult = setsockopt(listenSocket, IPPROTO_IPV6, IPV6_V6ONLY, (char *) &no, sizeof(no));
	if (iResult == SOCKET_ERROR) {
		printf("setsockopt failed: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	//Bind the socket
	iResult = bind(listenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	//No longer need addrinfo, the socket is created
	freeaddrinfo(result);

	//Listen on this socket
	if (listen(listenSocket, SOMAXCONN) == SOCKET_ERROR) {
		printf("listen failed: %ld\n", WSAGetLastError());
		closesocket(listenSocket);
		WSACleanup();
		return 1;
	}

	//Buffers for writing/recieving
	char recieveBuffer[BUFFERLEN_DEFAULT] = {};
	int recieveBufferLen = BUFFERLEN_DEFAULT;

	char* writeBuffer;
	int writeBufferLen = 0;

	int iSendResult = 0;

	//Begin the networking loop
	while (true) {
		printf("Listening for a connection...\n");

		//Establish a connection
		SOCKET clientSocket = accept(listenSocket, NULL, NULL);
		if (clientSocket == INVALID_SOCKET) {
			printf("accept failed: %d\n", WSAGetLastError());
			closesocket(listenSocket);
			WSACleanup();
			return 1;
		}

		printf("Connection established\n");

		//Begin read loop
		do {
			iResult = recv(clientSocket, recieveBuffer, recieveBufferLen, 0);
			if (iResult > 0) {
				printf("Recieved: %s\n", recieveBuffer);

				//Send 404
				writeBuffer = "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\nServer: Rainbow Dash/0.1 (20% Cooler) (Windows)\r\nConnection:close\r\n\r\n<h1>404 - Not Found</h1>";
				writeBufferLen = strlen(writeBuffer);

				printf("Sending: %s\n", writeBuffer);

				iSendResult = send(clientSocket, writeBuffer, writeBufferLen, 0);
				if (iSendResult == SOCKET_ERROR) {
					printf("Send failed: %d\n", WSAGetLastError());
					closesocket(clientSocket);
				}

				//Close the connection
				shutdown(clientSocket, SD_SEND);
				closesocket(clientSocket);
				iResult = 0;
			}
			else if (iResult == 0) {
				printf("Connection closing\n");
			}
			else {
				printf("recv failed: %d\n");
				closesocket(listenSocket);
				WSACleanup();
				return 1;
			}

		} while(iResult > 0);
	}

	return 0;
}