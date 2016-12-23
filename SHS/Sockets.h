#pragma once

#include <winsock2.h>
#include <ws2tcpip.h>
#include <exception>
#include <string>

#define IPv4_ANY "0.0.0.0"
#define IPv6_ANY "::"
#define DEFAULT_PORT "80"
#define DEFAULT_BUFFER_SIZE 1024

namespace sockets {
	WSADATA wsaData;

	class SocketException : std::exception {
		public:
		std::string message;
		int errorCode;

		SocketException(std::string message, int errorCode) : message(message), errorCode(errorCode) {};

		const char* what() const override {
			return message.c_str();
		}
	};

	class ServerSocket {
		private:
		std::string ip;
		std::string port;

		SOCKET listenSocket;

		public:
		ServerSocket(std::string ip, std::string port);

		~ServerSocket();

		void listen(int backlog = SOMAXCONN);
		ClientConnection accept(int bufferSize = DEFAULT_BUFFER_SIZE);

	};

	class ClientConnection {
		friend class ServerSocket;

		private:
		SOCKET clientSocket;

		char* writeBuffer;
		int writeBufferLength;

		char* recieveBuffer;
		int recieveBufferLength;

		ClientConnection(SOCKET& cs, int writeBuffer, int recieveBuffer);

		public:
		void write(char[] data);
		char* read();
	};

	void init() {
		int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
		if (iResult != 0) throw SocketException("Error on WSAStartup. Error: ", iResult);
	}
}