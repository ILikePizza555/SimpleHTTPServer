#include "Http.h"

#include <stdio.h>

#define BUFFERLEN_DEFAULT 512

int main(int argc, char* argv) {
	//Start up WinSock

	//Listen on this socket

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